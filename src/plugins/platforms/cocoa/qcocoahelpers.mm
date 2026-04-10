// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <AppKit/AppKit.h>

#include <qpa/qplatformtheme.h>

#include "qcocoahelpers.h"
#include "qnsview.h"

#include <qpa/qplatformscreen.h>
#include <private/qguiapplication_p.h>
#include <private/qwindow_p.h>
#include <BobUIGui/private/qcoregraphics_p.h>

#include <algorithm>

BOBUI_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(lcQpaWindow, "bobui.qpa.window");
Q_LOGGING_CATEGORY(lcQpaDrawing, "bobui.qpa.drawing");
Q_LOGGING_CATEGORY(lcQpaMouse, "bobui.qpa.input.mouse", BobUICriticalMsg);
Q_LOGGING_CATEGORY(lcQpaKeys, "bobui.qpa.input.keys", BobUICriticalMsg);
Q_LOGGING_CATEGORY(lcQpaInputMethods, "bobui.qpa.input.methods")
Q_LOGGING_CATEGORY(lcQpaScreen, "bobui.qpa.screen", BobUICriticalMsg);
Q_LOGGING_CATEGORY(lcQpaApplication, "bobui.qpa.application");
Q_LOGGING_CATEGORY(lcQpaClipboard, "bobui.qpa.clipboard")
Q_LOGGING_CATEGORY(lcQpaDialogs, "bobui.qpa.dialogs")
Q_LOGGING_CATEGORY(lcQpaMenus, "bobui.qpa.menus")

//
// Conversion Functions
//

QStringList bobui_mac_NSArrayToQStringList(NSArray<NSString *> *array)
{
    QStringList result;
    for (NSString *string in array)
        result << QString::fromNSString(string);
    return result;
}

NSMutableArray<NSString *> *bobui_mac_QStringListToNSMutableArray(const QStringList &list)
{
    NSMutableArray<NSString *> *result = [NSMutableArray<NSString *> arrayWithCapacity:list.size()];
    for (const QString &string : list)
        [result addObject:string.toNSString()];
    return result;
}

struct dndenum_mapper
{
    NSDragOperation mac_code;
    BobUI::DropAction bobui_code;
    bool BobUI2Mac;
};

static dndenum_mapper dnd_enums[] = {
    { NSDragOperationLink,  BobUI::LinkAction, true },
    { NSDragOperationMove,  BobUI::MoveAction, true },
    { NSDragOperationDelete,  BobUI::MoveAction, true },
    { NSDragOperationCopy,  BobUI::CopyAction, true },
    { NSDragOperationGeneric,  BobUI::CopyAction, false },
    { NSDragOperationEvery, BobUI::ActionMask, false },
    { NSDragOperationNone, BobUI::IgnoreAction, false }
};

NSDragOperation bobui_mac_mapDropAction(BobUI::DropAction action)
{
    for (int i=0; dnd_enums[i].bobui_code; i++) {
        if (dnd_enums[i].BobUI2Mac && (action & dnd_enums[i].bobui_code)) {
            return dnd_enums[i].mac_code;
        }
    }
    return NSDragOperationNone;
}

NSDragOperation bobui_mac_mapDropActions(BobUI::DropActions actions)
{
    NSDragOperation nsActions = NSDragOperationNone;
    for (int i=0; dnd_enums[i].bobui_code; i++) {
        if (dnd_enums[i].BobUI2Mac && (actions & dnd_enums[i].bobui_code))
            nsActions |= dnd_enums[i].mac_code;
    }
    return nsActions;
}

BobUI::DropAction bobui_mac_mapNSDragOperation(NSDragOperation nsActions)
{
    BobUI::DropAction action = BobUI::IgnoreAction;
    for (int i=0; dnd_enums[i].mac_code; i++) {
        if (nsActions & dnd_enums[i].mac_code)
            return dnd_enums[i].bobui_code;
    }
    return action;
}

BobUI::DropActions bobui_mac_mapNSDragOperations(NSDragOperation nsActions)
{
    BobUI::DropActions actions = BobUI::IgnoreAction;

    for (int i=0; dnd_enums[i].mac_code; i++) {
        if (dnd_enums[i].mac_code == NSDragOperationEvery)
            continue;

        if (nsActions & dnd_enums[i].mac_code)
            actions |= dnd_enums[i].bobui_code;
    }
    return actions;
}

/*!
    Returns the view cast to a QNSview if possible.

    If the view is not a QNSView, nil is returned, which is safe to
    send messages to, effectivly making [qnsview_cast(view) message]
    a no-op.

    For extra verbosity and clearer code, please consider checking
    that the platform window is not a foreign window before using
    this cast, via QPlatformWindow::isForeignWindow().

    Do not use this method solely to check for foreign windows, as
    that will make the code harder to read for people not working
    primarily on macOS, who do not know the difference between the
    NSView and QNSView cases.
*/
QNSView *qnsview_cast(NSView *view)
{
    return bobui_objc_cast<QNSView *>(view);
}

//
// Misc
//

// Sets the activation policy for this process to NSApplicationActivationPolicyRegular,
// unless either LSUIElement or LSBackgroundOnly is set in the Info.plist.
void bobui_mac_transformProccessToForegroundApplication()
{
    bool forceTransform = true;
    CFTypeRef value = CFBundleGetValueForInfoDictionaryKey(CFBundleGetMainBundle(),
                                                           CFSTR("LSUIElement"));
    if (value) {
        CFTypeID valueType = CFGetTypeID(value);
        // Officially it's supposed to be a string, a boolean makes sense, so we'll check.
        // A number less so, but OK.
        if (valueType == CFStringGetTypeID())
            forceTransform = !(QString::fromCFString(static_cast<CFStringRef>(value)).toInt());
        else if (valueType == CFBooleanGetTypeID())
            forceTransform = !CFBooleanGetValue(static_cast<CFBooleanRef>(value));
        else if (valueType == CFNumberGetTypeID()) {
            int valueAsInt;
            CFNumberGetValue(static_cast<CFNumberRef>(value), kCFNumberIntType, &valueAsInt);
            forceTransform = !valueAsInt;
        }
    }

    if (forceTransform) {
        value = CFBundleGetValueForInfoDictionaryKey(CFBundleGetMainBundle(),
                                                     CFSTR("LSBackgroundOnly"));
        if (value) {
            CFTypeID valueType = CFGetTypeID(value);
            if (valueType == CFBooleanGetTypeID())
                forceTransform = !CFBooleanGetValue(static_cast<CFBooleanRef>(value));
            else if (valueType == CFStringGetTypeID())
                forceTransform = !(QString::fromCFString(static_cast<CFStringRef>(value)).toInt());
            else if (valueType == CFNumberGetTypeID()) {
                int valueAsInt;
                CFNumberGetValue(static_cast<CFNumberRef>(value), kCFNumberIntType, &valueAsInt);
                forceTransform = !valueAsInt;
            }
        }
    }

    if (forceTransform) {
        [[NSApplication sharedApplication] setActivationPolicy:NSApplicationActivationPolicyRegular];
    }
}

QString bobui_mac_applicationName()
{
    QString appName;
    CFTypeRef string = CFBundleGetValueForInfoDictionaryKey(CFBundleGetMainBundle(), CFSTR("CFBundleName"));
    if (string)
        appName = QString::fromCFString(static_cast<CFStringRef>(string));

    if (appName.isEmpty()) {
        QString arg0 = QGuiApplicationPrivate::instance()->appName();
        if (arg0.contains("/")) {
            QStringList parts = arg0.split(u'/');
            appName = parts.at(parts.count() - 1);
        } else {
            appName = arg0;
        }
    }
    return appName;
}

// -------------------------------------------------------------------------

/*!
    \fn QPointF bobui_mac_flip(const QPointF &pos, const QRectF &reference)
    \fn QRectF bobui_mac_flip(const QRectF &rect, const QRectF &reference)

    Flips the Y coordinate of the point/rect between quadrant I and IV.

    The native coordinate system on macOS uses quadrant I, with origin
    in bottom left, and BobUI uses quadrant IV, with origin in top left.

    By flipping the Y coordinate, we can map the point/rect between
    the two coordinate systems.

    The flip is always in relation to a reference rectangle, e.g.
    the frame of the parent view, or the screen geometry. In the
    latter case the specialized QCocoaScreen::mapFrom/To functions
    should be used instead.
*/
QPointF bobui_mac_flip(const QPointF &pos, const QRectF &reference)
{
    return QPointF(pos.x(), reference.height() - pos.y());
}

QRectF bobui_mac_flip(const QRectF &rect, const QRectF &reference)
{
    return QRectF(bobui_mac_flip(rect.bottomLeft(), reference), rect.size());
}

// -------------------------------------------------------------------------

/*!
  \fn BobUI::MouseButton cocoaButton2BobUIButton(NSInteger buttonNum)

  Returns the BobUI::Button that corresponds to an NSEvent.buttonNumber.

  \note AppKit will use buttonNumber 0 to indicate both "left button"
  and "no button". Only NSEvents that describes mouse press/release
  events (e.g NSEventTypeOtherMouseDown) will contain a valid
  button number.
*/
BobUI::MouseButton cocoaButton2BobUIButton(NSInteger buttonNum)
{
    if (buttonNum >= 0 && buttonNum <= 31)
        return BobUI::MouseButton(1 << buttonNum);
    return BobUI::NoButton;
}

/*!
  \fn BobUI::MouseButton cocoaButton2BobUIButton(NSEvent *event)

  Returns the BobUI::Button that corresponds to an NSEvent.buttonNumber.

  \note AppKit will use buttonNumber 0 to indicate both "left button"
  and "no button". Only NSEvents that describes mouse press/release/dragging
  events (e.g NSEventTypeOtherMouseDown) will contain a valid
  button number.

  \note Wacom tablet might not return the correct button number for NSEvent buttonNumber
  on right clicks. Decide here that the button is the "right" button.
*/
BobUI::MouseButton cocoaButton2BobUIButton(NSEvent *event)
{
    if (cocoaEvent2BobUIMouseEvent(event) == QEvent::MouseMove)
        return BobUI::NoButton;

    switch (event.type) {
    case NSEventTypeRightMouseUp:
    case NSEventTypeRightMouseDown:
        return BobUI::RightButton;

    default:
        break;
    }

    return cocoaButton2BobUIButton(event.buttonNumber);
}

/*!
  \fn QEvent::Type cocoaEvent2BobUIMouseEvent(NSEvent *event)

  Returns the QEvent::Type that corresponds to an NSEvent.type.
*/
QEvent::Type cocoaEvent2BobUIMouseEvent(NSEvent *event)
{
    switch (event.type) {
    case NSEventTypeLeftMouseDown:
    case NSEventTypeRightMouseDown:
    case NSEventTypeOtherMouseDown:
        return QEvent::MouseButtonPress;

    case NSEventTypeLeftMouseUp:
    case NSEventTypeRightMouseUp:
    case NSEventTypeOtherMouseUp:
        return QEvent::MouseButtonRelease;

    case NSEventTypeLeftMouseDragged:
    case NSEventTypeRightMouseDragged:
    case NSEventTypeOtherMouseDragged:
        return QEvent::MouseMove;

    case NSEventTypeMouseMoved:
        return QEvent::MouseMove;

    default:
        break;
    }

    return QEvent::None;
}

/*!
  \fn BobUI::MouseButtons cocoaMouseButtons2BobUIMouseButtons(NSInteger pressedMouseButtons)

  Returns the BobUI::MouseButtons that corresponds to an NSEvent.pressedMouseButtons.
*/
BobUI::MouseButtons cocoaMouseButtons2BobUIMouseButtons(NSInteger pressedMouseButtons)
{
  return static_cast<BobUI::MouseButton>(pressedMouseButtons & BobUI::MouseButtonMask);
}

/*!
  \fn BobUI::MouseButtons currentlyPressedMouseButtons()

  Returns the BobUI::MouseButtons that corresponds to an NSEvent.pressedMouseButtons.
*/
BobUI::MouseButtons currentlyPressedMouseButtons()
{
  return cocoaMouseButtons2BobUIMouseButtons(NSEvent.pressedMouseButtons);
}

QString bobui_mac_removeAmpersandEscapes(QString s)
{
    return QPlatformTheme::removeMnemonics(s).trimmed();
}

NSString *bobui_mac_AppKitString(NSString *table, NSString *key)
{
    static const NSBundle *appKit = [NSBundle bundleForClass:NSApplication.class];
    if (!appKit)
        return key;

    return [appKit localizedStringForKey:key value:nil table:table];
}

BOBUI_END_NAMESPACE

/*! \internal

    This NSView derived class is used to add OK/Cancel
    buttons to NSColorPanel and NSFontPanel. It replaces
    the panel's content view, while reparenting the former
    content view into itself. It also takes care of setting
    the target-action for the OK/Cancel buttons and making
    sure the layout is consistent.
 */
@implementation QNSPanelContentsWrapper {
    NSButton *_okButton;
    NSButton *_cancelButton;
    NSView *_panelContents;
    NSEdgeInsets _panelContentsMargins;
}

@synthesize okButton = _okButton;
@synthesize cancelButton = _cancelButton;
@synthesize panelContents = _panelContents;
@synthesize panelContentsMargins = _panelContentsMargins;

- (instancetype)initWithPanelDelegate:(id<QNSPanelDelegate>)panelDelegate
{
    if ((self = [super initWithFrame:NSZeroRect])) {
        // create OK and Cancel buttons and add these as subviews
        _okButton = [self createButtonWithTitle:QPlatformDialogHelper::Ok];
        _okButton.action = @selector(onOkClicked);
        _okButton.target = panelDelegate;
        _cancelButton = [self createButtonWithTitle:QPlatformDialogHelper::Cancel];
        _cancelButton.action = @selector(onCancelClicked);
        _cancelButton.target = panelDelegate;

        _panelContents = nil;

        _panelContentsMargins = NSEdgeInsetsMake(0, 0, 0, 0);
    }

    return self;
}

- (void)dealloc
{
    [_okButton release];
    _okButton = nil;
    [_cancelButton release];
    _cancelButton = nil;

    _panelContents = nil;

    [super dealloc];
}

- (NSButton *)createButtonWithTitle:(QPlatformDialogHelper::StandardButton)type
{
    NSButton *button = [[NSButton alloc] initWithFrame:NSZeroRect];
    button.buttonType = NSButtonTypeMomentaryLight;
    button.bezelStyle = NSBezelStyleRounded;
    const QString &cleanTitle =
         QPlatformTheme::removeMnemonics(QGuiApplicationPrivate::platformTheme()->standardButtonText(type));
    // FIXME: Not obvious, from Cocoa's documentation, that QString::toNSString() makes a deep copy
    button.title = (NSString *)cleanTitle.toCFString();
    ((NSButtonCell *)button.cell).font =
            [NSFont systemFontOfSize:[NSFont systemFontSizeForControlSize:NSControlSizeRegular]];
    [self addSubview:button];
    return button;
}

- (void)layout
{
    static const CGFloat ButtonMinWidth = 78.0; // 84.0 for Carbon
    static const CGFloat ButtonMinHeight = 32.0;
    static const CGFloat ButtonSpacing = 0.0;
    static const CGFloat ButtonTopMargin = 0.0;
    static const CGFloat ButtonBottomMargin = 7.0;
    static const CGFloat ButtonSideMargin = 9.0;

    NSSize frameSize = self.frame.size;

    [self.okButton sizeToFit];
    NSSize okSizeHint = self.okButton.frame.size;

    [self.cancelButton sizeToFit];
    NSSize cancelSizeHint = self.cancelButton.frame.size;

    const CGFloat buttonWidth = qMin(qMax(ButtonMinWidth,
                                          qMax(okSizeHint.width, cancelSizeHint.width)),
                                     CGFloat((frameSize.width - 2.0 * ButtonSideMargin - ButtonSpacing) * 0.5));
    const CGFloat buttonHeight = qMax(ButtonMinHeight,
                                     qMax(okSizeHint.height, cancelSizeHint.height));

    NSRect okRect = { { frameSize.width - ButtonSideMargin - buttonWidth,
                        ButtonBottomMargin },
                      { buttonWidth, buttonHeight } };
    self.okButton.frame = okRect;
    self.okButton.needsDisplay = YES;

    NSRect cancelRect = { { okRect.origin.x - ButtonSpacing - buttonWidth,
                            ButtonBottomMargin },
                            { buttonWidth, buttonHeight } };
    self.cancelButton.frame = cancelRect;
    self.cancelButton.needsDisplay = YES;

    // The third view should be the original panel contents. Cache it.
    if (!self.panelContents)
        for (NSView *view in self.subviews)
            if (view != self.okButton && view != self.cancelButton) {
                _panelContents = view;
                break;
            }

    const CGFloat buttonBoxHeight = ButtonBottomMargin + buttonHeight + ButtonTopMargin;
    const NSRect panelContentsFrame = NSMakeRect(
                self.panelContentsMargins.left,
                buttonBoxHeight + self.panelContentsMargins.bottom,
                frameSize.width - (self.panelContentsMargins.left + self.panelContentsMargins.right),
                frameSize.height - buttonBoxHeight - (self.panelContentsMargins.top + self.panelContentsMargins.bottom));
    self.panelContents.frame = panelContentsFrame;
    self.panelContents.needsDisplay = YES;

    self.needsDisplay = YES;
    [super layout];
}

@end // QNSPanelContentsWrapper

BOBUI_BEGIN_NAMESPACE

// -------------------------------------------------------------------------

InputMethodQueryResult queryInputMethod(QObject *object, BobUI::InputMethodQueries queries)
{
    if (object) {
        QInputMethodQueryEvent queryEvent(queries | BobUI::ImEnabled);
        if (QCoreApplication::sendEvent(object, &queryEvent)) {
            if (queryEvent.value(BobUI::ImEnabled).toBool()) {
                InputMethodQueryResult result;
                static QMetaEnum queryEnum = QMetaEnum::fromType<BobUI::InputMethodQuery>();
                for (int i = 0; i < queryEnum.keyCount(); ++i) {
                    auto query = BobUI::InputMethodQuery(queryEnum.value(i));
                    if (queries & query)
                        result.insert(query, queryEvent.value(query));
                }
                return result;
            }
        }
    }
    return {};
}

// -------------------------------------------------------------------------

QDebug operator<<(QDebug debug, const NSRange &range)
{
    if (range.location == NSNotFound) {
        QDebugStateSaver saver(debug);
        debug.nospace() << "{NSNotFound, " << range.length << "}";
    } else {
        debug << NSStringFromRange(range);
    }
    return debug;
}

QDebug operator<<(QDebug debug, SEL selector)
{
    debug << NSStringFromSelector(selector);
    return debug;
}

BOBUI_END_NAMESPACE
