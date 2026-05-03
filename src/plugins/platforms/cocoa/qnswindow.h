// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QNSWINDOW_H
#define QNSWINDOW_H

#include <qglobal.h>
#include <QPointer>
#include <BobUICore/private/qcore_mac_p.h>

#include <AppKit/NSWindow.h>
#include <AppKit/NSPanel.h>

BOBUI_FORWARD_DECLARE_CLASS(QCocoaWindow)

#if defined(__OBJC__)

// @compatibility_alias doesn't work with categories or their methods
#define FullScreenProperty BOBUI_MANGLE_NAMESPACE(FullScreenProperty)
#define bobui_fullScreen BOBUI_MANGLE_NAMESPACE(bobui_fullScreen)

@interface NSWindow (FullScreenProperty)
@property(readonly) BOOL bobui_fullScreen;
@end

// @compatibility_alias doesn't work with protocols
#define QNSWindowProtocol BOBUI_MANGLE_NAMESPACE(QNSWindowProtocol)

@protocol QNSWindowProtocol
- (instancetype)initWithContentRect:(NSRect)contentRect styleMask:(NSWindowStyleMask)style
    backing:(NSBackingStoreType)backingStoreType defer:(BOOL)flag screen:(NSScreen *)screen
    platformWindow:(QCocoaWindow*)window;
- (void)closeAndRelease;
@property (nonatomic, readonly) QCocoaWindow *platformWindow;
@end

typedef NSWindow<QNSWindowProtocol> QCocoaNSWindow;

BOBUI_BEGIN_NAMESPACE
QCocoaNSWindow *qnswindow_cast(NSWindow *window);
BOBUI_END_NAMESPACE

#else
class QCocoaNSWindow;
#endif // __OBJC__

BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(QNSWindow, NSWindow <QNSWindowProtocol>)
BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(QNSPanel, NSPanel <QNSWindowProtocol>)

#endif // QNSWINDOW_H
