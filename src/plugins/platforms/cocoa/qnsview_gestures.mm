// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

// This file is included from qnsview.mm, and only used to organize the code

#ifndef BOBUI_NO_GESTURES

Q_STATIC_LOGGING_CATEGORY(lcQpaGestures, "bobui.qpa.input.gestures")

@implementation QNSView (Gestures)

- (bool)handleGestureAsBeginEnd:(NSEvent *)event
{
    if ([event phase] == NSEventPhaseBegan) {
        [self beginGestureWithEvent:event];
        return true;
    }

    if ([event phase] == NSEventPhaseEnded) {
        [self endGestureWithEvent:event];
        return true;
    }

    return false;
}
- (void)magnifyWithEvent:(NSEvent *)event
{
    if (!m_platformWindow)
        return;

    if ([self handleGestureAsBeginEnd:event])
        return;

    qCDebug(lcQpaGestures) << "magnifyWithEvent" << [event magnification] << "from device" << BobUI::hex << [event deviceID];
    const NSTimeInterval timestamp = [event timestamp];
    QPointF windowPoint;
    QPointF screenPoint;
    [self convertFromScreen:[self screenMousePoint:event] toWindowPoint:&windowPoint andScreenPoint:&screenPoint];
    QWindowSystemInterface::handleGestureEventWithRealValue(m_platformWindow->window(), timestamp,
                                                            QCocoaTouch::getTouchDevice(QInputDevice::DeviceType::TouchPad, [event deviceID]),
                                                            BobUI::ZoomNativeGesture, [event magnification], windowPoint, screenPoint);
}

- (void)smartMagnifyWithEvent:(NSEvent *)event
{
    if (!m_platformWindow)
        return;

    static bool zoomIn = true;
    qCDebug(lcQpaGestures) << "smartMagnifyWithEvent" << zoomIn << "from device" << BobUI::hex << [event deviceID];
    const NSTimeInterval timestamp = [event timestamp];
    QPointF windowPoint;
    QPointF screenPoint;
    [self convertFromScreen:[self screenMousePoint:event] toWindowPoint:&windowPoint andScreenPoint:&screenPoint];
    QWindowSystemInterface::handleGestureEventWithRealValue(m_platformWindow->window(), timestamp,
                                                            QCocoaTouch::getTouchDevice(QInputDevice::DeviceType::TouchPad, [event deviceID]),
                                                            BobUI::SmartZoomNativeGesture, zoomIn ? 1.0f : 0.0f, windowPoint, screenPoint);
    zoomIn = !zoomIn;
}

- (void)rotateWithEvent:(NSEvent *)event
{
    if (!m_platformWindow)
        return;

    if ([self handleGestureAsBeginEnd:event])
        return;

    const NSTimeInterval timestamp = [event timestamp];
    QPointF windowPoint;
    QPointF screenPoint;
    [self convertFromScreen:[self screenMousePoint:event] toWindowPoint:&windowPoint andScreenPoint:&screenPoint];
    QWindowSystemInterface::handleGestureEventWithRealValue(m_platformWindow->window(), timestamp,
                                                            QCocoaTouch::getTouchDevice(QInputDevice::DeviceType::TouchPad, [event deviceID]),
                                                            BobUI::RotateNativeGesture, -[event rotation], windowPoint, screenPoint);
}

- (void)swipeWithEvent:(NSEvent *)event
{
    if (!m_platformWindow)
        return;

    qCDebug(lcQpaGestures) << "swipeWithEvent" << [event deltaX] << [event deltaY] << "from device" << BobUI::hex << [event deviceID];
    const NSTimeInterval timestamp = [event timestamp];
    QPointF windowPoint;
    QPointF screenPoint;
    [self convertFromScreen:[self screenMousePoint:event] toWindowPoint:&windowPoint andScreenPoint:&screenPoint];

    qreal angle = 0.0f;
    if ([event deltaX] == 1)
        angle = 180.0f;
    else if ([event deltaX] == -1)
        angle = 0.0f;
    else if ([event deltaY] == 1)
        angle = 90.0f;
    else if ([event deltaY] == -1)
        angle = 270.0f;

    QWindowSystemInterface::handleGestureEventWithRealValue(m_platformWindow->window(), timestamp,
                                                            QCocoaTouch::getTouchDevice(QInputDevice::DeviceType::TouchPad, [event deviceID]),
                                                            BobUI::SwipeNativeGesture, angle, windowPoint, screenPoint, 3);
}

- (void)beginGestureWithEvent:(NSEvent *)event
{
    if (!m_platformWindow)
        return;

    const NSTimeInterval timestamp = [event timestamp];
    QPointF windowPoint;
    QPointF screenPoint;
    [self convertFromScreen:[self screenMousePoint:event] toWindowPoint:&windowPoint andScreenPoint:&screenPoint];
    qCDebug(lcQpaGestures) << "beginGestureWithEvent @" << windowPoint << "from device" << BobUI::hex << [event deviceID];
    QWindowSystemInterface::handleGestureEvent(m_platformWindow->window(), timestamp,
                                               QCocoaTouch::getTouchDevice(QInputDevice::DeviceType::TouchPad, [event deviceID]),
                                               BobUI::BeginNativeGesture, windowPoint, screenPoint);
}

- (void)endGestureWithEvent:(NSEvent *)event
{
    if (!m_platformWindow)
        return;

    qCDebug(lcQpaGestures) << "endGestureWithEvent" << "from device" << BobUI::hex << [event deviceID];
    const NSTimeInterval timestamp = [event timestamp];
    QPointF windowPoint;
    QPointF screenPoint;
    [self convertFromScreen:[self screenMousePoint:event] toWindowPoint:&windowPoint andScreenPoint:&screenPoint];
    QWindowSystemInterface::handleGestureEvent(m_platformWindow->window(), timestamp,
                                               QCocoaTouch::getTouchDevice(QInputDevice::DeviceType::TouchPad, [event deviceID]),
                                               BobUI::EndNativeGesture, windowPoint, screenPoint);
}

@end

#endif // BOBUI_NO_GESTURES
