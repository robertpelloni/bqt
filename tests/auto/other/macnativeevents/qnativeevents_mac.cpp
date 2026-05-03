// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "qnativeevents.h"
#include <CoreGraphics/CoreGraphics.h>
#include <BobUICore>

//  ************************************************************
//  Quartz
//  ************************************************************

static BobUI::KeyboardModifiers getModifiersFromQuartzEvent(CGEventRef inEvent)
{
    BobUI::KeyboardModifiers m;
    CGEventFlags flags = CGEventGetFlags(inEvent);
    if (flags & kCGEventFlagMaskShift || flags & kCGEventFlagMaskAlphaShift)
        m |= BobUI::ShiftModifier;
    if (flags & kCGEventFlagMaskControl)
        m |= BobUI::ControlModifier;
    if (flags & kCGEventFlagMaskAlternate)
        m |= BobUI::AltModifier;
    if (flags & kCGEventFlagMaskCommand)
        m |= BobUI::MetaModifier;
    return m;
}

static void setModifiersFromQNativeEvent(CGEventRef inEvent, const QNativeEvent &event)
{
    CGEventFlags flags = CGEventFlags(0);
    if (event.modifiers.testFlag(BobUI::ShiftModifier))
        flags = CGEventFlags(flags | kCGEventFlagMaskShift);
    if (event.modifiers.testFlag(BobUI::ControlModifier))
        flags = CGEventFlags(flags | kCGEventFlagMaskControl);
    if (event.modifiers.testFlag(BobUI::AltModifier))
        flags = CGEventFlags(flags | kCGEventFlagMaskAlternate);
    if (event.modifiers.testFlag(BobUI::MetaModifier))
        flags = CGEventFlags(flags | kCGEventFlagMaskCommand);
    CGEventSetFlags(inEvent, flags);
}

static QPoint getMouseLocationFromQuartzEvent(CGEventRef inEvent)
{
    CGPoint pos = CGEventGetLocation(inEvent);
    QPoint tmp;
    tmp.setX(pos.x);
    tmp.setY(pos.y);
    return tmp;
}

static QChar getCharFromQuartzEvent(CGEventRef inEvent)
{
    UniCharCount count = 0;
    UniChar c;
    CGEventKeyboardGetUnicodeString(inEvent, 1, &count, &c);
    return QChar(c);
}

static CGEventRef EventHandler_Quartz(CGEventTapProxy proxy, CGEventType type, CGEventRef inEvent, void *refCon)
{
    Q_UNUSED(proxy);
    QNativeInput *nativeInput = static_cast<QNativeInput *>(refCon);
    switch (type){
        case kCGEventKeyDown:{
            QNativeKeyEvent e;
            e.modifiers = getModifiersFromQuartzEvent(inEvent);
            e.nativeKeyCode = CGEventGetIntegerValueField(inEvent, kCGKeyboardEventKeycode);
            e.character = getCharFromQuartzEvent(inEvent);
            e.press = true;
            nativeInput->notify(&e);
            break;
        }
        case kCGEventKeyUp:{
            QNativeKeyEvent e;
            e.modifiers = getModifiersFromQuartzEvent(inEvent);
            e.nativeKeyCode = CGEventGetIntegerValueField(inEvent, kCGKeyboardEventKeycode);
            e.character = getCharFromQuartzEvent(inEvent);
            e.press = false;
            nativeInput->notify(&e);
            break;
        }
        case kCGEventLeftMouseDown:{
            QNativeMouseButtonEvent e;
            e.modifiers = getModifiersFromQuartzEvent(inEvent);
            e.globalPos = getMouseLocationFromQuartzEvent(inEvent);
            e.clickCount = CGEventGetIntegerValueField(inEvent, kCGMouseEventClickState);
            e.button = BobUI::LeftButton;
            nativeInput->notify(&e);
            break;
        }
        case kCGEventLeftMouseUp:{
            QNativeMouseButtonEvent e;
            e.modifiers = getModifiersFromQuartzEvent(inEvent);
            e.globalPos = getMouseLocationFromQuartzEvent(inEvent);
            e.clickCount = 0;
            e.button = BobUI::LeftButton;
            nativeInput->notify(&e);
            break;
        }
        case kCGEventRightMouseDown:{
            QNativeMouseButtonEvent e;
            e.modifiers = getModifiersFromQuartzEvent(inEvent);
            e.globalPos = getMouseLocationFromQuartzEvent(inEvent);
            e.clickCount = CGEventGetIntegerValueField(inEvent, kCGMouseEventClickState);
            e.button = BobUI::RightButton;
            nativeInput->notify(&e);
            break;
        }
        case kCGEventRightMouseUp:{
            QNativeMouseButtonEvent e;
            e.modifiers = getModifiersFromQuartzEvent(inEvent);
            e.globalPos = getMouseLocationFromQuartzEvent(inEvent);
            e.clickCount = 0;
            e.button = BobUI::RightButton;
            nativeInput->notify(&e);
            break;
        }
        case kCGEventMouseMoved:{
            QNativeMouseMoveEvent e;
            e.modifiers = getModifiersFromQuartzEvent(inEvent);
            e.globalPos = getMouseLocationFromQuartzEvent(inEvent);
            nativeInput->notify(&e);
            break;
        }
        case kCGEventLeftMouseDragged:{
            QNativeMouseDragEvent e;
            e.modifiers = getModifiersFromQuartzEvent(inEvent);
            e.globalPos = getMouseLocationFromQuartzEvent(inEvent);
            e.clickCount = CGEventGetIntegerValueField(inEvent, kCGMouseEventClickState);
            e.button = BobUI::LeftButton;
            nativeInput->notify(&e);
            break;
        }
        case kCGEventScrollWheel:{
            QNativeMouseWheelEvent e;
            e.modifiers = getModifiersFromQuartzEvent(inEvent);
            e.delta = CGEventGetIntegerValueField(inEvent, kCGScrollWheelEventDeltaAxis1);
            e.globalPos = getMouseLocationFromQuartzEvent(inEvent);
            nativeInput->notify(&e);
            break;
        }
        case kCGEventFlagsChanged:{
            QNativeModifierEvent e;
            e.modifiers = getModifiersFromQuartzEvent(inEvent);
            e.nativeKeyCode = CGEventGetIntegerValueField(inEvent, kCGKeyboardEventKeycode);
            nativeInput->notify(&e);
            break;
        }

    }

    return inEvent;
}

BobUI::Native::Status insertEventHandler_Quartz(QNativeInput *nativeInput)
{
    uid_t uid = geteuid();
    if (uid != 0)
        qWarning("MacNativeEvents: You must be root to listen for key events!");

    CFMachPortRef port = CGEventTapCreate(kCGHIDEventTap,
        kCGHeadInsertEventTap, kCGEventTapOptionListenOnly,
        kCGEventMaskForAllEvents, EventHandler_Quartz, nativeInput);

    CFRunLoopSourceRef eventSrc = CFMachPortCreateRunLoopSource(NULL, port, 0);
    CFRunLoopAddSource(CFRunLoopGetMain(), eventSrc, kCFRunLoopCommonModes);

    return BobUI::Native::Success;
}

BobUI::Native::Status removeEventHandler_Quartz()
{
    return BobUI::Native::Success; // ToDo:
}

BobUI::Native::Status sendNativeKeyEvent_Quartz(const QNativeKeyEvent &event)
{
    CGEventRef e = CGEventCreateKeyboardEvent(0, (uint)event.nativeKeyCode, event.press);
    setModifiersFromQNativeEvent(e, event);
    CGEventPost(kCGHIDEventTap, e);
    CFRelease(e);
    return BobUI::Native::Success;
}

BobUI::Native::Status sendNativeMouseMoveEvent_Quartz(const QNativeMouseMoveEvent &event)
{
    CGPoint pos;
    pos.x = event.globalPos.x();
    pos.y = event.globalPos.y();

    CGEventRef e = CGEventCreateMouseEvent(0, kCGEventMouseMoved, pos, kCGMouseButtonLeft /* ignored */);
    setModifiersFromQNativeEvent(e, event);
    CGEventPost(kCGHIDEventTap, e);
    CFRelease(e);
    return BobUI::Native::Success;
}

BobUI::Native::Status sendNativeMouseButtonEvent_Quartz(const QNativeMouseButtonEvent &event)
{
    CGPoint pos;
    pos.x = event.globalPos.x();
    pos.y = event.globalPos.y();

    CGEventType type = kCGEventNull;
    if (event.button == BobUI::LeftButton)
        type = (event.clickCount > 0) ? kCGEventLeftMouseDown : kCGEventLeftMouseUp;
    else if (event.button == BobUI::RightButton)
        type = (event.clickCount > 0) ? kCGEventRightMouseDown : kCGEventRightMouseUp;
    else
        type = (event.clickCount > 0) ? kCGEventOtherMouseDown : kCGEventOtherMouseUp;

    // The mouseButton argument to CGEventCreateMouseEvent() is ignored unless the type
    // is kCGEventOtherSomething, so defaulting to kCGMouseButtonLeft is fine.
    CGMouseButton mouseButton = (type == kCGEventOtherMouseDown || type == kCGEventOtherMouseUp) ?
        kCGMouseButtonCenter : kCGMouseButtonLeft;

    CGEventRef e = CGEventCreateMouseEvent(0, type, pos, mouseButton);
    setModifiersFromQNativeEvent(e, event);
    CGEventSetIntegerValueField(e, kCGMouseEventClickState, event.clickCount);
    CGEventPost(kCGHIDEventTap, e);
    CFRelease(e);
    return BobUI::Native::Success;
}

BobUI::Native::Status sendNativeMouseDragEvent_Quartz(const QNativeMouseDragEvent &event)
{
    CGPoint pos;
    pos.x = event.globalPos.x();
    pos.y = event.globalPos.y();

    CGEventType type = kCGEventNull;
    if (event.button == BobUI::LeftButton)
        type = kCGEventLeftMouseDragged;
    else if (event.button == BobUI::RightButton)
        type = kCGEventRightMouseDragged;
    else
        type = kCGEventOtherMouseDragged;

    // The mouseButton argument to CGEventCreateMouseEvent() is ignored unless the type
    // is kCGEventOtherSomething, so defaulting to kCGMouseButtonLeft is fine.
    CGMouseButton mouseButton = type == kCGEventOtherMouseDragged ? kCGMouseButtonCenter : kCGMouseButtonLeft;

    CGEventRef e = CGEventCreateMouseEvent(0, type, pos, mouseButton);
    setModifiersFromQNativeEvent(e, event);
    CGEventPost(kCGHIDEventTap, e);
    CFRelease(e);
    return BobUI::Native::Success;
}

BobUI::Native::Status sendNativeMouseWheelEvent_Quartz(const QNativeMouseWheelEvent &event)
{
    CGPoint pos;
    pos.x = event.globalPos.x();
    pos.y = event.globalPos.y();

    CGEventRef e = CGEventCreateScrollWheelEvent(0, kCGScrollEventUnitPixel, 1, 0);
    CGEventSetIntegerValueField(e, kCGScrollWheelEventDeltaAxis1, event.delta);
    CGEventSetLocation(e, pos);
    setModifiersFromQNativeEvent(e, event);
    CGEventPost(kCGHIDEventTap, e);
    CFRelease(e);

    return BobUI::Native::Success;
}

BobUI::Native::Status sendNativeModifierEvent_Quartz(const QNativeModifierEvent &event)
{
    CGEventRef e = CGEventCreateKeyboardEvent(0, (uint)event.nativeKeyCode, 0);
    CGEventSetType(e, kCGEventFlagsChanged);
    setModifiersFromQNativeEvent(e, event);
    CGEventPost(kCGHIDEventTap, e);
    CFRelease(e);
    return BobUI::Native::Success;
}

//  ************************************************************
//  QNativeInput methods:
//  ************************************************************

BobUI::Native::Status QNativeInput::sendNativeMouseButtonEvent(const QNativeMouseButtonEvent &event)
{
    return sendNativeMouseButtonEvent_Quartz(event);
}

BobUI::Native::Status QNativeInput::sendNativeMouseMoveEvent(const QNativeMouseMoveEvent &event)
{
    return sendNativeMouseMoveEvent_Quartz(event);
}

BobUI::Native::Status QNativeInput::sendNativeMouseDragEvent(const QNativeMouseDragEvent &event)
{
    return sendNativeMouseDragEvent_Quartz(event);
}

BobUI::Native::Status QNativeInput::sendNativeMouseWheelEvent(const QNativeMouseWheelEvent &event)
{
    return sendNativeMouseWheelEvent_Quartz(event);
}

BobUI::Native::Status QNativeInput::sendNativeKeyEvent(const QNativeKeyEvent &event)
{
    return sendNativeKeyEvent_Quartz(event);
}

BobUI::Native::Status QNativeInput::sendNativeModifierEvent(const QNativeModifierEvent &event)
{
    return sendNativeModifierEvent_Quartz(event);
}

BobUI::Native::Status QNativeInput::subscribeForNativeEvents()
{
    return insertEventHandler_Quartz(this);
}

BobUI::Native::Status QNativeInput::unsubscribeForNativeEvents()
{
    return removeEventHandler_Quartz();
}

// Some BobUI to Mac mappings:
int QNativeKeyEvent::Key_A = 0;
int QNativeKeyEvent::Key_B = 11;
int QNativeKeyEvent::Key_C = 8;
int QNativeKeyEvent::Key_1 = 18;
int QNativeKeyEvent::Key_Backspace = 51;
int QNativeKeyEvent::Key_Enter = 36;
int QNativeKeyEvent::Key_Del = 117;

