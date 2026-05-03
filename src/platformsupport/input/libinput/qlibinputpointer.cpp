// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qlibinputpointer_p.h"
#include <libinput.h>
#include <BobUICore/QEvent>
#include <BobUIGui/QGuiApplication>
#include <BobUIGui/QScreen>
#include <BobUIGui/private/qguiapplication_p.h>
#include <BobUIGui/private/qinputdevicemanager_p.h>
#include <qpa/qwindowsysteminterface.h>
#include <private/qhighdpiscaling_p.h>

BOBUI_BEGIN_NAMESPACE

Q_STATIC_LOGGING_CATEGORY(qLcLibInputMouse, "bobui.qpa.input.mouse")

QLibInputPointer::QLibInputPointer()
    : m_buttons(BobUI::NoButton)
{
}

void QLibInputPointer::processButton(libinput_event_pointer *e)
{
    const uint32_t b = libinput_event_pointer_get_button(e);
    const bool pressed = libinput_event_pointer_get_button_state(e) == LIBINPUT_BUTTON_STATE_PRESSED;

    BobUI::MouseButton button = BobUI::NoButton;
    switch (b) {
    case 0x110: button = BobUI::LeftButton; break;    // BTN_LEFT
    case 0x111: button = BobUI::RightButton; break;
    case 0x112: button = BobUI::MiddleButton; break;
    case 0x113: button = BobUI::ExtraButton1; break;  // AKA BobUI::BackButton
    case 0x114: button = BobUI::ExtraButton2; break;  // AKA BobUI::ForwardButton
    case 0x115: button = BobUI::ExtraButton3; break;  // AKA BobUI::TaskButton
    case 0x116: button = BobUI::ExtraButton4; break;
    case 0x117: button = BobUI::ExtraButton5; break;
    case 0x118: button = BobUI::ExtraButton6; break;
    case 0x119: button = BobUI::ExtraButton7; break;
    case 0x11a: button = BobUI::ExtraButton8; break;
    case 0x11b: button = BobUI::ExtraButton9; break;
    case 0x11c: button = BobUI::ExtraButton10; break;
    case 0x11d: button = BobUI::ExtraButton11; break;
    case 0x11e: button = BobUI::ExtraButton12; break;
    case 0x11f: button = BobUI::ExtraButton13; break;
    }

    m_buttons.setFlag(button, pressed);

    QEvent::Type type = pressed ? QEvent::MouseButtonPress : QEvent::MouseButtonRelease;
    BobUI::KeyboardModifiers mods = QGuiApplicationPrivate::inputDeviceManager()->keyboardModifiers();

    qCDebug(qLcLibInputMouse) << "processButton" << type << button << m_buttons << m_pos << mods;
    QWindowSystemInterface::handleMouseEvent(nullptr, m_pos, m_pos, m_buttons, button, type, mods);
}

void QLibInputPointer::processMotion(libinput_event_pointer *e)
{
    const double dx = libinput_event_pointer_get_dx(e);
    const double dy = libinput_event_pointer_get_dy(e);
    QScreen * const primaryScreen = QGuiApplication::primaryScreen();
    const QRect g = QHighDpi::toNativePixels(primaryScreen->virtualGeometry(), primaryScreen);

    m_pos.setX(qBound(g.left(), qRound(m_pos.x() + dx), g.right()));
    m_pos.setY(qBound(g.top(), qRound(m_pos.y() + dy), g.bottom()));

    BobUI::KeyboardModifiers mods = QGuiApplicationPrivate::inputDeviceManager()->keyboardModifiers();

    qCDebug(qLcLibInputMouse) << "processMotion" << m_pos << mods;
    QWindowSystemInterface::handleMouseEvent(nullptr, m_pos, m_pos, m_buttons,
                                             BobUI::NoButton, QEvent::MouseMove, mods);
}

void QLibInputPointer::processAbsMotion(libinput_event_pointer *e)
{
    QScreen * const primaryScreen = QGuiApplication::primaryScreen();
    const QRect g = QHighDpi::toNativePixels(primaryScreen->virtualGeometry(), primaryScreen);

    const double x = libinput_event_pointer_get_absolute_x_transformed(e, g.width());
    const double y = libinput_event_pointer_get_absolute_y_transformed(e, g.height());

    m_pos.setX(qBound(g.left(), qRound(g.left() + x), g.right()));
    m_pos.setY(qBound(g.top(), qRound(g.top() + y), g.bottom()));

    BobUI::KeyboardModifiers mods = QGuiApplicationPrivate::inputDeviceManager()->keyboardModifiers();

    qCDebug(qLcLibInputMouse) << "processAbsMotion" << m_pos << mods;
    QWindowSystemInterface::handleMouseEvent(nullptr, m_pos, m_pos, m_buttons,
                                             BobUI::NoButton, QEvent::MouseMove, mods);

}

void QLibInputPointer::processAxis(libinput_event_pointer *e)
{
    double value; // default axis value is 15 degrees per wheel click
    QPoint angleDelta;
#if !BOBUI_CONFIG(libinput_axis_api)
    value = libinput_event_pointer_get_axis_value(e);
    if (libinput_event_pointer_get_axis(e) == LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL)
        angleDelta.setY(qRound(value));
    else
        angleDelta.setX(qRound(value));
#else
    if (libinput_event_pointer_has_axis(e, LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL)) {
#if BOBUI_CONFIG(libinput_hires_wheel_support)
        value = libinput_event_pointer_get_scroll_value_v120(e, LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL);
#else
        value = libinput_event_pointer_get_axis_value(e, LIBINPUT_POINTER_AXIS_SCROLL_VERTICAL);
#endif
        angleDelta.setY(qRound(value));
    }
    if (libinput_event_pointer_has_axis(e, LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL)) {
#if BOBUI_CONFIG(libinput_hires_wheel_support)
        value = libinput_event_pointer_get_scroll_value_v120(e, LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL);
#else
        value = libinput_event_pointer_get_axis_value(e, LIBINPUT_POINTER_AXIS_SCROLL_HORIZONTAL);
#endif
        angleDelta.setX(qRound(value));
    }
#endif
#if BOBUI_CONFIG(libinput_hires_wheel_support)
    const int factor = -1;
#else
    const int factor = -8;
#endif
    angleDelta *= factor;
    BobUI::KeyboardModifiers mods = QGuiApplicationPrivate::inputDeviceManager()->keyboardModifiers();
    qCDebug(qLcLibInputMouse) << "processAxis" << angleDelta << m_pos << mods;
    QWindowSystemInterface::handleWheelEvent(nullptr, m_pos, m_pos, QPoint(), angleDelta, mods);
}

void QLibInputPointer::setPos(const QPoint &pos)
{
    QScreen * const primaryScreen = QGuiApplication::primaryScreen();
    const QRect g = QHighDpi::toNativePixels(primaryScreen->virtualGeometry(), primaryScreen);

    m_pos.setX(qBound(g.left(), pos.x(), g.right()));
    m_pos.setY(qBound(g.top(), pos.y(), g.bottom()));
    qCDebug(qLcLibInputMouse) << "setPos" << pos << "bounded:" << m_pos;
}

BOBUI_END_NAMESPACE
