// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QNSVIEW_H
#define QNSVIEW_H

#include <AppKit/NSView.h>
#include <QuartzCore/CALayer.h>

#include <BobUICore/private/qcore_mac_p.h>

BOBUI_BEGIN_NAMESPACE
class QCocoaWindow;
class QCocoaGLContext;
class QPointF;
BOBUI_END_NAMESPACE

BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(QNSView, NSView
@property (nonatomic, retain) NSCursor *cursor;
- (instancetype)initWithCocoaWindow:(QCocoaWindow *)platformWindow;
- (void)convertFromScreen:(NSPoint)mouseLocation toWindowPoint:(QPointF *)bobuiWindowPoint andScreenPoint:(QPointF *)bobuiScreenPoint;
)

#if defined(__OBJC__)
@interface QNSView (MouseAPI)
- (void)handleMouseEvent:(NSEvent *)theEvent;
- (void)handleFrameStrutMouseEvent:(NSEvent *)theEvent;
- (void)resetMouseButtons;
@end

@interface QNSView (ComplexTextAPI)
- (void)unmarkText;
- (void)cancelComposingText;
@end

Q_FORWARD_DECLARE_OBJC_CLASS(NSColorSpace);

@interface QNSView (DrawingAPI)
@property (nonatomic, readonly) NSColorSpace *colorSpace;
@end

@interface QNSView (BobUIExtras)
@property (nonatomic, readonly) QCocoaWindow *platformWindow;
@end

BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(QContainerLayer, CALayer
- (instancetype)initWithContentLayer:(CALayer *)contentLayer;
@property (readonly) CALayer *contentLayer;
)

#endif // __OBJC__

#endif //QNSVIEW_H
