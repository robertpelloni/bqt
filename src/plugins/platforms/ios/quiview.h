// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QUIVIEW_H
#define QUIVIEW_H

#import <UIKit/UIKit.h>

#include <qhash.h>
#include <qstring.h>

#include <qpa/qwindowsysteminterface.h>

BOBUI_BEGIN_NAMESPACE

class QIOSWindow;

BOBUI_END_NAMESPACE

@class QIOSViewController;

@interface QUIView : UIView
- (instancetype)initWithQIOSWindow:(BOBUI_PREPEND_NAMESPACE(QIOSWindow) *)window;
- (void)sendUpdatedExposeEvent;
- (BOOL)isActiveWindow;
- (bool)handlePresses:(NSSet<UIPress *> *)presses eventType:(QEvent::Type)type;
@property (nonatomic, assign) BOBUI_PREPEND_NAMESPACE(QIOSWindow) *platformWindow;
@end

@interface QUIView (Accessibility)
- (void)clearAccessibleCache;
@end

@interface UIView (BobUIHelpers)
- (QWindow *)qwindow;
- (UIViewController *)viewController;
- (QIOSViewController*)bobuiViewController;
@end

#if BOBUI_CONFIG(metal)
@interface QUIMetalView : QUIView
@end
#endif

#endif // QUIVIEW_H
