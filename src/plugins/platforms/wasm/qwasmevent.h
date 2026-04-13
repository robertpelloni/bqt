// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QWASMEVENT_H
#define QWASMEVENT_H

#include "qwasmplatform.h"
#include "qwasmdom.h"

#include <BobUICore/qglobal.h>
#include <BobUICore/qnamespace.h>
#include <BobUIGui/qevent.h>
#include <private/qstdweb_p.h>
#include <QPoint>

#include <emscripten/html5.h>
#include <emscripten/val.h>

BOBUI_BEGIN_NAMESPACE
class QWindow;

enum class EventType {
    DragEnd,
    DragOver,
    DragStart,
    DragEnter,
    DragLeave,
    Drop,
    KeyDown,
    KeyUp,
    PointerDown,
    PointerMove,
    PointerUp,
    PointerEnter,
    PointerLeave,
    PointerCancel,
    Wheel,
};

enum class PointerType {
    Mouse,
    Touch,
    Pen,
    Other,
};

enum class WindowArea {
    NonClient,
    Client,
};

enum class DeltaMode { Pixel, Line, Page };

struct Event
{
    Event(EventType type, emscripten::val webEvent);

    bool isTargetedForElement(emscripten::val element) const;

    emscripten::val webEvent;
    EventType type;
    emscripten::val target() const { return webEvent["target"]; }
};

struct KeyEvent : public Event
{
    KeyEvent(EventType type, emscripten::val webEvent);

    BobUI::Key key;
    QFlags<BobUI::KeyboardModifier> modifiers;
    bool deadKey;
    QString text;
    bool autoRepeat;
    bool isComposing;
    int keyCode;
};

struct MouseEvent : public Event
{
    MouseEvent(EventType type, emscripten::val webEvent);

    static constexpr BobUI::MouseButton buttonFromWeb(int webButton) {
        switch (webButton) {
            case 0:
                return BobUI::LeftButton;
            case 1:
                return BobUI::MiddleButton;
            case 2:
                return BobUI::RightButton;
            default:
                return BobUI::NoButton;
        }
    }

    static constexpr BobUI::MouseButtons buttonsFromWeb(unsigned short webButtons) {
        // Coincidentally, BobUI and web bitfields match.
        return BobUI::MouseButtons::fromInt(webButtons);
    }

    static constexpr QEvent::Type mouseEventTypeFromEventType(
        EventType eventType, WindowArea windowArea) {
        switch (eventType) {
            case EventType::PointerDown :
                return windowArea == WindowArea::Client ?
                    QEvent::MouseButtonPress : QEvent::NonClientAreaMouseButtonPress;
            case EventType::PointerUp :
                return windowArea == WindowArea::Client ?
                    QEvent::MouseButtonRelease : QEvent::NonClientAreaMouseButtonRelease;
            case EventType::PointerMove :
                return windowArea == WindowArea::Client ?
                    QEvent::MouseMove : QEvent::NonClientAreaMouseMove;
            default:
                return QEvent::None;
        }
    }

    QPointF localPoint;
    QPointF pointInPage;
    QPointF pointInViewport;
    BobUI::MouseButton mouseButton;
    BobUI::MouseButtons mouseButtons;
    QFlags<BobUI::KeyboardModifier> modifiers;
};

struct PointerEvent : public MouseEvent
{
    PointerEvent(EventType type, emscripten::val webEvent);

    PointerType pointerType;
    int pointerId;
    qreal pressure;
    qreal tiltX;
    qreal tiltY;
    qreal tangentialPressure;
    qreal twist;
    qreal width;
    qreal height;
    bool isPrimary;
};

struct DragEvent : public MouseEvent
{
    DragEvent(EventType type, emscripten::val webEvent, QWindow *targetQWindow);

    void cancelDragStart();
    void acceptDragOver();
    void acceptDrop();

    BobUI::DropAction dropAction;
    dom::DataTransfer dataTransfer;
    QWindow *targetWindow;
};

struct WheelEvent : public MouseEvent
{
    WheelEvent(EventType type, emscripten::val webEvent);

    DeltaMode deltaMode;
    bool webkitDirectionInvertedFromDevice;
    QPointF delta;
};

BOBUI_END_NAMESPACE

#endif  // QWASMEVENT_H
