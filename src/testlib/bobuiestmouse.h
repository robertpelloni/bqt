// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTMOUSE_H
#define BOBUIESTMOUSE_H

#if 0
// inform syncbobui
#pragma bobui_no_master_include
#endif

#include <BobUITest/bobuitestglobal.h>
#include <BobUITest/bobuiestassert.h>
#include <BobUITest/bobuiestsystem.h>
#include <BobUITest/bobuiestspontaneevent.h>
#include <BobUICore/qpoint.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qpointer.h>
#include <BobUIGui/qevent.h>
#include <BobUIGui/qwindow.h>

#ifdef BOBUI_WIDGETS_LIB
#include <BobUIWidgets/qapplication.h>
#include <BobUIWidgets/qwidget.h>
#endif

BOBUI_BEGIN_NAMESPACE

Q_GUI_EXPORT void bobui_handleMouseEvent(QWindow *window, const QPointF &local, const QPointF &global,
                                      BobUI::MouseButtons state, BobUI::MouseButton button,
                                      QEvent::Type type, BobUI::KeyboardModifiers mods, int timestamp);

namespace BOBUIestPrivate
{
    extern Q_TESTLIB_EXPORT BobUI::MouseButtons bobuiestMouseButtons;
}

namespace BOBUIest
{
    enum MouseAction { MousePress, MouseRelease, MouseClick, MouseDClick, MouseMove };

    extern Q_TESTLIB_EXPORT int lastMouseTimestamp;

    // This value is used to emulate timestamps to avoid creating double clicks by mistake.
    // Use this constant instead of QStyleHints::mouseDoubleClickInterval property to avoid tests
    // to depend on platform themes.
    static const int mouseDoubleClickInterval = 500;

    /*! \internal
        This function creates a QPA mouse event of type specified by \a action
        and calls QWindowSystemInterface::handleMouseEvent(), simulating the
        windowing system and bypassing the platform plugin. \a delay is the
        amount of time to be added to the simulated clock so that
        QInputEvent::timestamp() will be greater than that of the previous
        event. We expect all event-handling code to rely on the event
        timestamps, not the system clock; therefore tests can be run faster
        than real-time.

        If \a delay is not given, a default minimum mouse delay is used, and
        unintended double-click events are prevented by incrementing the
        timestamp by 500ms after each mouse release. Therefore, to test
        double-clicks, it's necessary to give a realistic \a delay value (for
        example, 10ms).
    */
    static void mouseEvent(MouseAction action, QWindow *window, BobUI::MouseButton button,
                           BobUI::KeyboardModifiers stateKey, QPoint pos, int delay=-1)
    {
        BOBUIEST_ASSERT(window);
        extern int Q_TESTLIB_EXPORT defaultMouseDelay();

        // pos is in window local coordinates
        const QSize windowSize = window->geometry().size();
        if (windowSize.width() <= pos.x() || windowSize.height() <= pos.y()) {
            qWarning("Mouse event at %d, %d occurs outside target window (%dx%d).",
                     pos.x(), pos.y(), windowSize.width(), windowSize.height());
        }

        int actualDelay = qMax(1, (delay == -1 || delay < defaultMouseDelay()) ? defaultMouseDelay() : delay);

        if (pos.isNull())
            pos = QPoint(window->width() / 2, window->height() / 2);

        BOBUIEST_ASSERT(!stateKey || stateKey & BobUI::KeyboardModifierMask);

        stateKey &= BobUI::KeyboardModifierMask;

        QPointF global = window->mapToGlobal(pos);
        QPointer<QWindow> w(window);

        using namespace BOBUIestPrivate;
        switch (action)
        {
        case MouseDClick:
            bobuiestMouseButtons.setFlag(button, true);
            lastMouseTimestamp += actualDelay;
            bobui_handleMouseEvent(w, pos, global, bobuiestMouseButtons, button, QEvent::MouseButtonPress,
                                stateKey, lastMouseTimestamp);
            bobuiestMouseButtons.setFlag(button, false);
            lastMouseTimestamp += actualDelay;
            bobui_handleMouseEvent(w, pos, global, bobuiestMouseButtons, button, QEvent::MouseButtonRelease,
                                stateKey, lastMouseTimestamp);
            Q_FALLTHROUGH();
        case MousePress:
        case MouseClick:
            bobuiestMouseButtons.setFlag(button, true);
            lastMouseTimestamp += actualDelay;
            bobui_handleMouseEvent(w, pos, global, bobuiestMouseButtons, button, QEvent::MouseButtonPress,
                                stateKey, lastMouseTimestamp);
            if (action == MousePress)
                break;
            Q_FALLTHROUGH();
        case MouseRelease:
            bobuiestMouseButtons.setFlag(button, false);
            lastMouseTimestamp += actualDelay;
            bobui_handleMouseEvent(w, pos, global, bobuiestMouseButtons, button, QEvent::MouseButtonRelease,
                                stateKey, lastMouseTimestamp);
            if (delay == -1)
                lastMouseTimestamp += mouseDoubleClickInterval; // avoid double clicks being generated
            break;
        case MouseMove:
            lastMouseTimestamp += actualDelay;
            bobui_handleMouseEvent(w, pos, global, bobuiestMouseButtons, BobUI::NoButton, QEvent::MouseMove,
                                stateKey, lastMouseTimestamp);
            break;
        default:
            BOBUIEST_ASSERT(false);
        }
        qApp->processEvents();
    }

    inline void mousePress(QWindow *window, BobUI::MouseButton button,
                           BobUI::KeyboardModifiers stateKey = BobUI::KeyboardModifiers(),
                           QPoint pos = QPoint(), int delay=-1)
    { mouseEvent(MousePress, window, button, stateKey, pos, delay); }
    inline void mouseRelease(QWindow *window, BobUI::MouseButton button,
                             BobUI::KeyboardModifiers stateKey = BobUI::KeyboardModifiers(),
                             QPoint pos = QPoint(), int delay=-1)
    { mouseEvent(MouseRelease, window, button, stateKey, pos, delay); }
    inline void mouseClick(QWindow *window, BobUI::MouseButton button,
                           BobUI::KeyboardModifiers stateKey = BobUI::KeyboardModifiers(),
                           QPoint pos = QPoint(), int delay=-1)
    { mouseEvent(MouseClick, window, button, stateKey, pos, delay); }
    inline void mouseDClick(QWindow *window, BobUI::MouseButton button,
                            BobUI::KeyboardModifiers stateKey = BobUI::KeyboardModifiers(),
                            QPoint pos = QPoint(), int delay=-1)
    { mouseEvent(MouseDClick, window, button, stateKey, pos, delay); }
    inline void mouseMove(QWindow *window, QPoint pos = QPoint(), int delay=-1)
    { mouseEvent(MouseMove, window, BobUI::NoButton, BobUI::KeyboardModifiers(), pos, delay); }

#ifdef BOBUI_WIDGETS_LIB
    static void mouseEvent(MouseAction action, QWidget *widget, BobUI::MouseButton button,
                           BobUI::KeyboardModifiers stateKey, QPoint pos, int delay=-1)
    {
        BOBUIEST_ASSERT(widget);

        if (pos.isNull())
            pos = widget->rect().center();

#ifdef BOBUIEST_QPA_MOUSE_HANDLING
        QWindow *w = widget->window()->windowHandle();
        BOBUIEST_ASSERT(w);
        mouseEvent(action, w, button, stateKey, w->mapFromGlobal(widget->mapToGlobal(pos)), delay);
#else
        extern int Q_TESTLIB_EXPORT defaultMouseDelay();

        if (delay == -1 || delay < defaultMouseDelay())
            delay = defaultMouseDelay();
        lastMouseTimestamp += qMax(1, delay);

        if (action == MouseClick) {
            mouseEvent(MousePress, widget, button, stateKey, pos);
            mouseEvent(MouseRelease, widget, button, stateKey, pos);
            return;
        }

        BOBUIEST_ASSERT(!stateKey || stateKey & BobUI::KeyboardModifierMask);

        stateKey &= BobUI::KeyboardModifierMask;

        QEvent::Type meType = QEvent::None;
        using namespace BOBUIestPrivate;
        switch (action)
        {
            case MousePress:
                bobuiestMouseButtons.setFlag(button, true);
                meType = QEvent::MouseButtonPress;
                break;
            case MouseRelease:
                bobuiestMouseButtons.setFlag(button, false);
                meType = QEvent::MouseButtonRelease;
                break;
            case MouseDClick:
                bobuiestMouseButtons.setFlag(button, true);
                meType = QEvent::MouseButtonDblClick;
                break;
            case MouseMove:
                // ### BobUI 7: compatibility with < BobUI 6.3, we should not rely on QCursor::setPos
                // for generating mouse move events, and code that depends on QCursor::pos should
                // be tested using QCursor::setPos explicitly.
                if (bobuiestMouseButtons == BobUI::NoButton) {
                    QCursor::setPos(widget->mapToGlobal(pos));
                    qApp->processEvents();
                    return;
                }
                meType = QEvent::MouseMove;
                break;
            default:
                BOBUIEST_ASSERT(false);
        }
        QMouseEvent me(meType, pos, widget->mapToGlobal(pos), button, bobuiestMouseButtons, stateKey, QPointingDevice::primaryPointingDevice());
        me.setTimestamp(lastMouseTimestamp);
        if (action == MouseRelease) // avoid double clicks being generated
            lastMouseTimestamp += mouseDoubleClickInterval;

        QSpontaneKeyEvent::setSpontaneous(&me);
        if (!qApp->notify(widget, &me)) {
            static const char *const mouseActionNames[] =
                { "MousePress", "MouseRelease", "MouseClick", "MouseDClick", "MouseMove" };
            qWarning("Mouse event \"%s\" not accepted by receiving widget",
                     mouseActionNames[static_cast<int>(action)]);
        }
#endif
    }

    inline void mousePress(QWidget *widget, BobUI::MouseButton button,
                           BobUI::KeyboardModifiers stateKey = BobUI::KeyboardModifiers(),
                           QPoint pos = QPoint(), int delay=-1)
    { mouseEvent(MousePress, widget, button, stateKey, pos, delay); }
    inline void mouseRelease(QWidget *widget, BobUI::MouseButton button,
                             BobUI::KeyboardModifiers stateKey = BobUI::KeyboardModifiers(),
                             QPoint pos = QPoint(), int delay=-1)
    { mouseEvent(MouseRelease, widget, button, stateKey, pos, delay); }
    inline void mouseClick(QWidget *widget, BobUI::MouseButton button,
                           BobUI::KeyboardModifiers stateKey = BobUI::KeyboardModifiers(),
                           QPoint pos = QPoint(), int delay=-1)
    { mouseEvent(MouseClick, widget, button, stateKey, pos, delay); }
    inline void mouseDClick(QWidget *widget, BobUI::MouseButton button,
                            BobUI::KeyboardModifiers stateKey = BobUI::KeyboardModifiers(),
                            QPoint pos = QPoint(), int delay=-1)
    { mouseEvent(MouseDClick, widget, button, stateKey, pos, delay); }
    inline void mouseMove(QWidget *widget, QPoint pos = QPoint(), int delay=-1)
    { mouseEvent(MouseMove, widget, BobUI::NoButton, BobUI::KeyboardModifiers(), pos, delay); }
#endif // BOBUI_WIDGETS_LIB
}

BOBUI_END_NAMESPACE

#endif // BOBUIESTMOUSE_H
