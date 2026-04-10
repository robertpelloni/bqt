// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qiosglobal.h"
#include "qiosapplicationdelegate.h"
#include "qiosviewcontroller.h"
#include "qiosscreen.h"
#include "quiwindow.h"
#include "qioseventdispatcher.h"

#include <BobUICore/private/qcore_mac_p.h>

BOBUI_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(lcQpaApplication, "bobui.qpa.application");
Q_LOGGING_CATEGORY(lcQpaInputMethods, "bobui.qpa.input.methods", BobUICriticalMsg);
Q_LOGGING_CATEGORY(lcQpaWindow, "bobui.qpa.window");
Q_LOGGING_CATEGORY(lcQpaWindowScene, "bobui.qpa.window.scene");

bool isBobUIApplication()
{
    // Returns \c true if the plugin is in full control of the whole application. This means
    // that we control the application delegate and the top view controller, and can take
    // actions that impacts all parts of the application. The opposite means that we are
    // embedded inside a native iOS application, and should be more focused on playing along
    // with native UIControls, and less inclined to change structures that lies outside the
    // scope of our QWindows/UIViews.
    return QIOSEventDispatcher::isBobUIApplication();
}

bool isRunningOnVisionOS()
{
    static bool result = []{
        // This class is documented to only be available on visionOS
        return NSClassFromString(@"UIWindowSceneGeometryPreferencesVision");
    }();
    return result;
}

#ifndef Q_OS_TVOS
BobUI::ScreenOrientation toBobUIScreenOrientation(UIDeviceOrientation uiDeviceOrientation)
{
    BobUI::ScreenOrientation bobuiOrientation;
    switch (uiDeviceOrientation) {
    case UIDeviceOrientationPortraitUpsideDown:
        bobuiOrientation = BobUI::InvertedPortraitOrientation;
        break;
    case UIDeviceOrientationLandscapeLeft:
        bobuiOrientation = BobUI::LandscapeOrientation;
        break;
    case UIDeviceOrientationLandscapeRight:
        bobuiOrientation = BobUI::InvertedLandscapeOrientation;
        break;
    case UIDeviceOrientationFaceUp:
    case UIDeviceOrientationFaceDown:
        qWarning("Falling back to BobUI::PortraitOrientation for UIDeviceOrientationFaceUp/UIDeviceOrientationFaceDown");
        bobuiOrientation = BobUI::PortraitOrientation;
        break;
    default:
        bobuiOrientation = BobUI::PortraitOrientation;
        break;
    }
    return bobuiOrientation;
}

UIDeviceOrientation fromBobUIScreenOrientation(BobUI::ScreenOrientation bobuiOrientation)
{
    UIDeviceOrientation uiOrientation;
    switch (bobuiOrientation) {
    case BobUI::LandscapeOrientation:
        uiOrientation = UIDeviceOrientationLandscapeLeft;
        break;
    case BobUI::InvertedLandscapeOrientation:
        uiOrientation = UIDeviceOrientationLandscapeRight;
        break;
    case BobUI::InvertedPortraitOrientation:
        uiOrientation = UIDeviceOrientationPortraitUpsideDown;
        break;
    case BobUI::PrimaryOrientation:
    case BobUI::PortraitOrientation:
    default:
        uiOrientation = UIDeviceOrientationPortrait;
        break;
    }
    return uiOrientation;
}
#endif

int infoPlistValue(NSString* key, int defaultValue)
{
    static NSBundle *bundle = [NSBundle mainBundle];
    NSNumber* value = [bundle objectForInfoDictionaryKey:key];
    return value ? [value intValue] : defaultValue;
}

UIWindow *presentationWindow(QWindow *window)
{
    UIWindow *uiWindow = window ? reinterpret_cast<UIView *>(window->winId()).window : nullptr;
    if (!uiWindow) {
        auto *scenes = [bobui_apple_sharedApplication().connectedScenes allObjects];
        if (scenes.count > 0) {
            auto *windowScene = static_cast<UIWindowScene*>(scenes[0]);
            uiWindow = windowScene.keyWindow;
            if (!uiWindow && windowScene.windows.count)
                uiWindow = windowScene.windows[0];
        }
    }
    return uiWindow;
}

UIView *rootViewForScreen(const QPlatformScreen *screen)
{
    Q_ASSERT(screen);

    const auto *iosScreen = static_cast<const QIOSScreen *>(screen);
    for (UIScene *scene in [bobui_apple_sharedApplication().connectedScenes allObjects]) {
        if (![scene isKindOfClass:UIWindowScene.class])
            continue;

        auto *windowScene = static_cast<UIWindowScene*>(scene);

#if !defined(Q_OS_VISIONOS)
        if (windowScene.screen != iosScreen->uiScreen())
            continue;
#else
        Q_UNUSED(iosScreen);
#endif

        UIWindow *uiWindow = bobui_objc_cast<QUIWindow*>(windowScene.keyWindow);
        if (!uiWindow) {
            for (UIWindow *win in windowScene.windows) {
                if (bobui_objc_cast<QUIWindow*>(win)) {
                    uiWindow = win;
                    break;
                }
            }
        }

        return uiWindow.rootViewController.view;
    }

    return nullptr;
}

BOBUI_END_NAMESPACE

// -------------------------------------------------------------------------

@interface BobUIFirstResponderEvent : UIEvent
@property (nonatomic, strong) id firstResponder;
@end

@implementation BobUIFirstResponderEvent
- (void)dealloc
{
    self.firstResponder = 0;
    [super dealloc];
}
@end


@implementation UIView (BobUIFirstResponder)
- (UIView*)bobui_findFirstResponder
{
    if ([self isFirstResponder])
        return self;

    for (UIView *subview in self.subviews) {
        if (UIView *firstResponder = [subview bobui_findFirstResponder])
            return firstResponder;
    }

    return nil;
}
@end

@implementation UIResponder (BobUIFirstResponder)

+ (id)bobui_currentFirstResponder
{
    if (bobui_apple_isApplicationExtension()) {
        qWarning() << "can't get first responder in application extensions!";
        return nil;
    }

    BobUIFirstResponderEvent *event = [[[BobUIFirstResponderEvent alloc] init] autorelease];
    [bobui_apple_sharedApplication() sendAction:@selector(bobui_findFirstResponder:event:) to:nil from:nil forEvent:event];
    return event.firstResponder;
}

- (void)bobui_findFirstResponder:(id)sender event:(BobUIFirstResponderEvent *)event
{
    Q_UNUSED(sender);

    if ([self isKindOfClass:[UIView class]])
        event.firstResponder = [static_cast<UIView *>(self) bobui_findFirstResponder];
    else
        event.firstResponder = [self isFirstResponder] ? self : nil;
}
@end

BOBUI_BEGIN_NAMESPACE

FirstResponderCandidate::FirstResponderCandidate(UIResponder *responder)
    : QScopedValueRollback<UIResponder *>(s_firstResponderCandidate, responder)
{
}

UIResponder *FirstResponderCandidate::s_firstResponderCandidate = nullptr;

BOBUI_END_NAMESPACE

