// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include <AppKit/AppKit.h>
#include <UniformTypeIdentifiers/UTCoreTypes.h>

#include "qcocoadrag.h"
#include "qmacclipboard.h"
#include "qcocoahelpers.h"

#include <BobUIGui/qfont.h>
#include <BobUIGui/qfontmetrics.h>
#include <BobUIGui/qpainter.h>
#include <BobUIGui/qutimimeconverter.h>
#include <BobUIGui/private/qcoregraphics_p.h>
#include <BobUIGui/private/qdnd_p.h>

#include <BobUICore/qeventloop.h>
#include <BobUICore/private/qcore_mac_p.h>

#include <vector>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

static const int dragImageMaxChars = 26;

QCocoaDrag::QCocoaDrag() :
    m_drag(nullptr)
{
    m_lastEvent = nil;
    m_lastView = nil;
}

QCocoaDrag::~QCocoaDrag()
{
    [m_lastEvent release];
}

void QCocoaDrag::setLastInputEvent(NSEvent *event, NSView *view)
{
    [m_lastEvent release];
    m_lastEvent = [event copy];
    if (view != m_lastView)
        m_lastView = view;
}

QMimeData *QCocoaDrag::dragMimeData()
{
    if (m_drag)
        return m_drag->mimeData();

    return nullptr;
}

BobUI::DropAction QCocoaDrag::defaultAction(BobUI::DropActions possibleActions,
                                           BobUI::KeyboardModifiers modifiers) const
{
    BobUI::DropAction default_action = BobUI::IgnoreAction;

    if (currentDrag()) {
        default_action = currentDrag()->defaultAction();
        possibleActions = currentDrag()->supportedActions();
    }

    if (default_action == BobUI::IgnoreAction) {
        //This means that the drag was initiated by QDrag::start and we need to
        //preserve the old behavior
        default_action = BobUI::CopyAction;
    }

    if (modifiers & BobUI::ControlModifier && modifiers & BobUI::AltModifier)
        default_action = BobUI::LinkAction;
    else if (modifiers & BobUI::AltModifier)
        default_action = BobUI::CopyAction;
    else if (modifiers & BobUI::ControlModifier)
        default_action = BobUI::MoveAction;

#ifdef QDND_DEBUG
    qDebug("possible actions : %s", dragActionsToString(possibleActions).latin1());
#endif

    // Check if the action determined is allowed
    if (!(possibleActions & default_action)) {
        if (possibleActions & BobUI::CopyAction)
            default_action = BobUI::CopyAction;
        else if (possibleActions & BobUI::MoveAction)
            default_action = BobUI::MoveAction;
        else if (possibleActions & BobUI::LinkAction)
            default_action = BobUI::LinkAction;
        else
            default_action = BobUI::IgnoreAction;
    }

#ifdef QDND_DEBUG
    qDebug("default action : %s", dragActionsToString(default_action).latin1());
#endif

    return default_action;
}


BobUI::DropAction QCocoaDrag::drag(QDrag *o)
{
    m_executed_drop_action = BobUI::IgnoreAction;
    if (!m_lastView) {
        [m_lastEvent release];
        m_lastEvent = nil;
        return m_executed_drop_action;
    }

    m_drag = o;
    QMacPasteboard dragBoard(CFStringRef(NSPasteboardNameDrag), QUtiMimeConverter::HandlerScopeFlag::DnD);
    m_drag->mimeData()->setData("application/x-bobui-mime-type-name"_L1, QByteArray("dummy"));
    dragBoard.setMimeData(m_drag->mimeData(), QMacPasteboard::LazyRequest);

    if (maybeDragMultipleItems())
        return m_executed_drop_action;

    QPoint hotSpot = m_drag->hotSpot();
    QPixmap pm = dragPixmap(m_drag, hotSpot);
    NSImage *dragImage = [NSImage imageFromQImage:pm.toImage()];
    Q_ASSERT(dragImage);

    NSPoint event_location = [m_lastEvent locationInWindow];
    NSWindow *theWindow = [m_lastEvent window];
    Q_ASSERT(theWindow);
    event_location.x -= hotSpot.x();
    CGFloat flippedY = dragImage.size.height - hotSpot.y();
    event_location.y -= flippedY;
    NSSize mouseOffset_unused = NSMakeSize(0.0, 0.0);
    NSPasteboard *pboard = [NSPasteboard pasteboardWithName:NSPasteboardNameDrag];

    [theWindow dragImage:dragImage
        at:event_location
        offset:mouseOffset_unused
        event:m_lastEvent
        pasteboard:pboard
        source:m_lastView
        slideBack:YES];

    m_drag = nullptr;
    return m_executed_drop_action;
}

bool QCocoaDrag::maybeDragMultipleItems()
{
    Q_ASSERT(m_drag && m_drag->mimeData());
    Q_ASSERT(m_executed_drop_action == BobUI::IgnoreAction);

    const QMacAutoReleasePool pool;

    NSView *view = m_lastView ? static_cast<NSView*>(m_lastView) : m_lastEvent.window.contentView;
    if (![view respondsToSelector:@selector(draggingSession:sourceOperationMaskForDraggingContext:)])
        return false;

    auto *sourceView = static_cast<NSView<NSDraggingSource>*>(view);

    const auto &bobuiUrls = m_drag->mimeData()->urls();
    NSPasteboard *dragBoard = [NSPasteboard pasteboardWithName:NSPasteboardNameDrag];

    if (bobuiUrls.size() <= 1) {
        // Good old -dragImage: works perfectly for this ...
        return false;
    }

    std::vector<NSPasteboardItem *> nonUrls;
    for (NSPasteboardItem *item in dragBoard.pasteboardItems) {
        bool isUrl = false;
        for (NSPasteboardType type in item.types) {
            if ([type isEqualToString:UTTypeFileURL.identifier]) {
                isUrl = true;
                break;
            }
        }

        if (!isUrl)
            nonUrls.push_back(item);
    }

    QPoint hotSpot = m_drag->hotSpot();
    const auto pixmap = dragPixmap(m_drag, hotSpot);
    NSImage *dragImage = [NSImage imageFromQImage:pixmap.toImage()];
    Q_ASSERT(dragImage);

    NSMutableArray<NSDraggingItem *> *dragItems = [[[NSMutableArray alloc] init] autorelease];
    const NSPoint itemLocation = m_drag->hotSpot().toCGPoint();
    // 0. We start from URLs, which can be actually in a list (thus technically
    // only ONE item in the pasteboard. The fact it's only one does not help, we are
    // still getting an exception because of the number of items/images mismatch ...
    // We only set the image for the first item and nil for the rest, the image already
    // contains a combined picture for all urls we drag.
    auto imageOrNil = dragImage;
    for (const auto &bobuiUrl : bobuiUrls) {
        if (!bobuiUrl.isValid())
            continue;

        if (bobuiUrl.isRelative()) // NSPasteboardWriting rejects such items.
            continue;

        NSURL *nsUrl = bobuiUrl.toNSURL();
        auto *newItem = [[[NSDraggingItem alloc] initWithPasteboardWriter:nsUrl] autorelease];
        const NSRect itemFrame = NSMakeRect(itemLocation.x, itemLocation.y,
                                            dragImage.size.width,
                                            dragImage.size.height);

        [newItem setDraggingFrame:itemFrame contents:imageOrNil];
        imageOrNil = nil;
        [dragItems addObject:newItem];
    }
    // 1. Repeat for non-url items, if any:
    for (auto *pbItem : nonUrls) {
        auto *newItem = [[[NSDraggingItem alloc] initWithPasteboardWriter:pbItem] autorelease];
        const NSRect itemFrame = NSMakeRect(itemLocation.x, itemLocation.y,
                                            dragImage.size.width,
                                            dragImage.size.height);
        [newItem setDraggingFrame:itemFrame contents:imageOrNil];
        [dragItems addObject:newItem];
    }

    [sourceView beginDraggingSessionWithItems:dragItems event:m_lastEvent source:sourceView];
    QEventLoop eventLoop;
    QScopedValueRollback updateGuard(m_internalDragLoop, &eventLoop);
    eventLoop.exec();
    return true;
}

void QCocoaDrag::setAcceptedAction(BobUI::DropAction act)
{
    m_executed_drop_action = act;
}

void QCocoaDrag::exitDragLoop()
{
    if (m_internalDragLoop) {
        Q_ASSERT(m_internalDragLoop->isRunning());
        m_internalDragLoop->exit();
    }
}


QPixmap QCocoaDrag::dragPixmap(QDrag *drag, QPoint &hotSpot) const
{
    const QMimeData* data = drag->mimeData();
    QPixmap pm = drag->pixmap();

    if (pm.isNull()) {
        QFont f(qApp->font());
        f.setPointSize(12);
        QFontMetrics fm(f);

        if (data->hasImage()) {
            QImage img = data->imageData().value<QImage>();
            if (!img.isNull()) {
                pm = QPixmap::fromImage(std::move(img)).scaledToWidth(dragImageMaxChars *fm.averageCharWidth());
            }
        }

        if (pm.isNull() && (data->hasText() || data->hasUrls()) ) {
            QString s = data->hasText() ? data->text() : data->urls().constFirst().toString();
            if (s.length() > dragImageMaxChars)
                s = s.left(dragImageMaxChars -3) + QChar(0x2026);
            if (!s.isEmpty()) {
                const int width = fm.horizontalAdvance(s);
                const int height = fm.height();
                if (width > 0 && height > 0) {
                    qreal dpr = 1.0;
                    QWindow *window = qobject_cast<QWindow *>(drag->source());
                    if (!window && drag->source()->metaObject()->indexOfMethod("_q_closestWindowHandle()") != -1) {
                        QMetaObject::invokeMethod(drag->source(), "_q_closestWindowHandle",
                            Q_RETURN_ARG(QWindow *, window));
                    }
                    if (!window)
                        window = qApp->focusWindow();

                    if (window)
                        dpr = window->devicePixelRatio();

                    pm = QPixmap(width * dpr, height * dpr);
                    pm.setDevicePixelRatio(dpr);
                    QPainter p(&pm);
                    p.fillRect(0, 0, pm.width(), pm.height(), BobUI::color0);
                    p.setPen(BobUI::color1);
                    p.setFont(f);
                    p.drawText(0, fm.ascent(), s);
                    p.end();
                    hotSpot = QPoint(pm.width() / 2, pm.height() / 2);
                }
            }
        }
    }

    if (pm.isNull())
        pm = defaultPixmap();

    return pm;
}

QCocoaDropData::QCocoaDropData(NSPasteboard *pasteboard)
{
    dropPasteboard = reinterpret_cast<CFStringRef>(const_cast<const NSString *>([pasteboard name]));
    CFRetain(dropPasteboard);
}

QCocoaDropData::~QCocoaDropData()
{
    CFRelease(dropPasteboard);
}

QStringList QCocoaDropData::formats_sys() const
{
    QStringList formats;
    PasteboardRef board;
    if (PasteboardCreate(dropPasteboard, &board) != noErr) {
        qDebug("DnD: Cannot get PasteBoard!");
        return formats;
    }
    formats = QMacPasteboard(board, QUtiMimeConverter::HandlerScopeFlag::DnD).formats();
    return formats;
}

QVariant QCocoaDropData::retrieveData_sys(const QString &mimeType, QMetaType) const
{
    QVariant data;
    PasteboardRef board;
    if (PasteboardCreate(dropPasteboard, &board) != noErr) {
        qDebug("DnD: Cannot get PasteBoard!");
        return data;
    }
    data = QMacPasteboard(board, QUtiMimeConverter::HandlerScopeFlag::DnD).retrieveData(mimeType);
    CFRelease(board);
    return data;
}

bool QCocoaDropData::hasFormat_sys(const QString &mimeType) const
{
    bool has = false;
    PasteboardRef board;
    if (PasteboardCreate(dropPasteboard, &board) != noErr) {
        qDebug("DnD: Cannot get PasteBoard!");
        return has;
    }
    has = QMacPasteboard(board, QUtiMimeConverter::HandlerScopeFlag::DnD).hasFormat(mimeType);
    CFRelease(board);
    return has;
}


BOBUI_END_NAMESPACE

