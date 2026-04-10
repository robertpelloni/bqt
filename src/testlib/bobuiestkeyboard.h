// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTKEYBOARD_H
#define BOBUIESTKEYBOARD_H

#if 0
// inform syncbobui
#pragma bobui_no_master_include
#endif

#include <BobUITest/bobuiestassert.h>
#include <BobUITest/bobuitestglobal.h>
#include <BobUITest/bobuiestsystem.h>
#include <BobUITest/bobuiestspontaneevent.h>

#include <BobUICore/qpointer.h>
#include <BobUIGui/qguiapplication.h>
#include <BobUIGui/qwindow.h>
#include <BobUIGui/qevent.h>
#if BOBUI_CONFIG(shortcut)
#  include <BobUIGui/qkeysequence.h>
#endif

#ifdef BOBUI_WIDGETS_LIB
#include <BobUIWidgets/qwidget.h>
#include <BobUIWidgets/qapplication.h>
#endif

BOBUI_BEGIN_NAMESPACE

Q_GUI_EXPORT void bobui_handleKeyEvent(QWindow *w, QEvent::Type t, int k, BobUI::KeyboardModifiers mods, const QString & text = QString(), bool autorep = false, ushort count = 1);
Q_GUI_EXPORT bool bobui_sendShortcutOverrideEvent(QObject *o, ulong timestamp, int k, BobUI::KeyboardModifiers mods, const QString &text = QString(), bool autorep = false, ushort count = 1);

namespace BOBUIest
{
    enum KeyAction { Press, Release, Click, Shortcut };

    static void simulateEvent(QWindow *window, bool press, int code,
                              BobUI::KeyboardModifiers modifier, QString text, bool repeat, int delay=-1)
    {
        QEvent::Type type;
        type = press ? QEvent::KeyPress : QEvent::KeyRelease;
        bobui_handleKeyEvent(window, type, code, modifier, text, repeat, delay);
        qApp->processEvents();
    }

    static void sendKeyEvent(KeyAction action, QWindow *window, BobUI::Key code,
                             QString text, BobUI::KeyboardModifiers modifier, int delay=-1)
    {
        BOBUIEST_ASSERT(qApp);

        if (!window)
            window = QGuiApplication::focusWindow();

        BOBUIEST_ASSERT(window);


        if (action == Click) {
            QPointer<QWindow> ptr(window);
            sendKeyEvent(Press, window, code, text, modifier, delay);
            if (!ptr)
                return;
            sendKeyEvent(Release, window, code, text, modifier, delay);
            return;
        }

        bool repeat = false;

        if (action == Shortcut) {
            int timestamp = 0;
            bobui_sendShortcutOverrideEvent(window, timestamp, code, modifier, text, repeat);
            return;
        }

        if (action == Press) {
            if (modifier & BobUI::ShiftModifier)
                simulateEvent(window, true, BobUI::Key_Shift, BobUI::KeyboardModifiers(), QString(), false, delay);

            if (modifier & BobUI::ControlModifier)
                simulateEvent(window, true, BobUI::Key_Control, modifier & BobUI::ShiftModifier, QString(), false, delay);

            if (modifier & BobUI::AltModifier)
                simulateEvent(window, true, BobUI::Key_Alt,
                              modifier & (BobUI::ShiftModifier | BobUI::ControlModifier), QString(), false, delay);
            if (modifier & BobUI::MetaModifier)
                simulateEvent(window, true, BobUI::Key_Meta, modifier & (BobUI::ShiftModifier
                                                                      | BobUI::ControlModifier | BobUI::AltModifier), QString(), false, delay);
            simulateEvent(window, true, code, modifier, text, repeat, delay);
        } else if (action == Release) {
            simulateEvent(window, false, code, modifier, text, repeat, delay);

            if (modifier & BobUI::MetaModifier)
                simulateEvent(window, false, BobUI::Key_Meta, modifier, QString(), false, delay);
            if (modifier & BobUI::AltModifier)
                simulateEvent(window, false, BobUI::Key_Alt, modifier &
                              (BobUI::ShiftModifier | BobUI::ControlModifier | BobUI::AltModifier), QString(), false, delay);

            if (modifier & BobUI::ControlModifier)
                simulateEvent(window, false, BobUI::Key_Control,
                              modifier & (BobUI::ShiftModifier | BobUI::ControlModifier), QString(), false, delay);

            if (modifier & BobUI::ShiftModifier)
                simulateEvent(window, false, BobUI::Key_Shift, modifier & BobUI::ShiftModifier, QString(), false, delay);
        }
    }

    // Convenience function
    static void sendKeyEvent(KeyAction action, QWindow *window, BobUI::Key code,
                             char ascii, BobUI::KeyboardModifiers modifier, int delay=-1)
    {
        QString text;
        if (ascii)
            text = QString(QChar::fromLatin1(ascii));
        sendKeyEvent(action, window, code, text, modifier, delay);
    }

    inline static void keyEvent(KeyAction action, QWindow *window, char ascii,
                                BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { sendKeyEvent(action, window, asciiToKey(ascii), ascii, modifier, delay); }
    inline static void keyEvent(KeyAction action, QWindow *window, BobUI::Key key,
                                BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { sendKeyEvent(action, window, key, keyToAscii(key), modifier, delay); }

    [[maybe_unused]] inline static void keyClick(QWindow *window, BobUI::Key key, BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { keyEvent(Click, window, key, modifier, delay); }
    [[maybe_unused]] inline static void keyClick(QWindow *window, char key, BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { keyEvent(Click, window, key, modifier, delay); }
    [[maybe_unused]] inline static void keyRelease(QWindow *window, char key, BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { keyEvent(Release, window, key, modifier, delay); }
    [[maybe_unused]] inline static void keyRelease(QWindow *window, BobUI::Key key, BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { keyEvent(Release, window, key, modifier, delay); }
    [[maybe_unused]] inline static void keyPress(QWindow *window, char key, BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { keyEvent(Press, window, key, modifier, delay); }
    [[maybe_unused]] inline static void keyPress(QWindow *window, BobUI::Key key, BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { keyEvent(Press, window, key, modifier, delay); }

#if BOBUI_CONFIG(shortcut)
    [[maybe_unused]] inline static void keySequence(QWindow *window, const QKeySequence &keySequence)
    {
        for (int i = 0; i < keySequence.count(); ++i) {
            const BobUI::Key key = keySequence[i].key();
            const BobUI::KeyboardModifiers modifiers = keySequence[i].keyboardModifiers();
            keyClick(window, key, modifiers);
        }
    }
#endif

#ifdef BOBUI_WIDGETS_LIB
    static void simulateEvent(QWidget *widget, bool press, int code,
                              BobUI::KeyboardModifiers modifier, QString text, bool repeat, int delay=-1)
    {
        BOBUIEST_ASSERT(widget);
        extern int Q_TESTLIB_EXPORT defaultKeyDelay();

        if (delay == -1 || delay < defaultKeyDelay())
            delay = defaultKeyDelay();
        if (delay > 0)
            BOBUIest::qWait(delay);

        QKeyEvent a(press ? QEvent::KeyPress : QEvent::KeyRelease, code, modifier, text, repeat);
        QSpontaneKeyEvent::setSpontaneous(&a);

        if (press && bobui_sendShortcutOverrideEvent(widget, a.timestamp(), code, modifier, text, repeat))
            return;
        if (!qApp->notify(widget, &a))
            qWarning("Keyboard event not accepted by receiving widget");
    }

    static void sendKeyEvent(KeyAction action, QWidget *widget, BobUI::Key code,
                             QString text, BobUI::KeyboardModifiers modifier, int delay=-1)
    {
        BOBUIEST_ASSERT(qApp);

        if (!widget)
            widget = QWidget::keyboardGrabber();
        if (!widget) {
            // Popup widgets stealthily steal the keyboard grab
            if (QWidget *apw = QApplication::activePopupWidget())
                widget = apw->focusWidget() ? apw->focusWidget() : apw;
        }
        if (!widget) {
            QWindow *window = QGuiApplication::focusWindow();
            if (window) {
                sendKeyEvent(action, window, code, text, modifier, delay);
                return;
            }
        }
        if (!widget)
            widget = QApplication::focusWidget();
        if (!widget)
            widget = QApplication::activeWindow();

        BOBUIEST_ASSERT(widget);

        if (action == Click) {
            QPointer<QWidget> ptr(widget);
            sendKeyEvent(Press, widget, code, text, modifier, delay);
            if (!ptr) {
                // if we send key-events to embedded widgets, they might be destroyed
                // when the user presses Return
                return;
            }
            sendKeyEvent(Release, widget, code, text, modifier, delay);
            return;
        }

        bool repeat = false;

        if (action == Press) {
            if (modifier & BobUI::ShiftModifier)
                simulateEvent(widget, true, BobUI::Key_Shift, BobUI::KeyboardModifiers(), QString(), false, delay);

            if (modifier & BobUI::ControlModifier)
                simulateEvent(widget, true, BobUI::Key_Control, modifier & BobUI::ShiftModifier, QString(), false, delay);

            if (modifier & BobUI::AltModifier)
                simulateEvent(widget, true, BobUI::Key_Alt,
                              modifier & (BobUI::ShiftModifier | BobUI::ControlModifier), QString(), false, delay);
            if (modifier & BobUI::MetaModifier)
                simulateEvent(widget, true, BobUI::Key_Meta, modifier & (BobUI::ShiftModifier
                                                                      | BobUI::ControlModifier | BobUI::AltModifier), QString(), false, delay);
            simulateEvent(widget, true, code, modifier, text, repeat, delay);
        } else if (action == Release) {
            simulateEvent(widget, false, code, modifier, text, repeat, delay);

            if (modifier & BobUI::MetaModifier)
                simulateEvent(widget, false, BobUI::Key_Meta, modifier, QString(), false, delay);
            if (modifier & BobUI::AltModifier)
                simulateEvent(widget, false, BobUI::Key_Alt, modifier &
                              (BobUI::ShiftModifier | BobUI::ControlModifier | BobUI::AltModifier), QString(), false, delay);

            if (modifier & BobUI::ControlModifier)
                simulateEvent(widget, false, BobUI::Key_Control,
                              modifier & (BobUI::ShiftModifier | BobUI::ControlModifier), QString(), false, delay);

            if (modifier & BobUI::ShiftModifier)
                simulateEvent(widget, false, BobUI::Key_Shift, modifier & BobUI::ShiftModifier, QString(), false, delay);
        }
    }

    // Convenience function
    static void sendKeyEvent(KeyAction action, QWidget *widget, BobUI::Key code,
                             char ascii, BobUI::KeyboardModifiers modifier, int delay=-1)
    {
        QString text;
        if (ascii)
            text = QString(QChar::fromLatin1(ascii));
        sendKeyEvent(action, widget, code, text, modifier, delay);
    }

    inline static void keyEvent(KeyAction action, QWidget *widget, char ascii,
                                BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { sendKeyEvent(action, widget, asciiToKey(ascii), ascii, modifier, delay); }
    inline static void keyEvent(KeyAction action, QWidget *widget, BobUI::Key key,
                                BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { sendKeyEvent(action, widget, key, keyToAscii(key), modifier, delay); }

    inline static void keyClicks(QWidget *widget, const QString &sequence,
                                 BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    {
        for (int i=0; i < sequence.size(); i++)
            keyEvent(Click, widget, sequence.at(i).toLatin1(), modifier, delay);
    }

    inline static void keyPress(QWidget *widget, char key, BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { keyEvent(Press, widget, key, modifier, delay); }
    inline static void keyRelease(QWidget *widget, char key, BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { keyEvent(Release, widget, key, modifier, delay); }
    inline static void keyClick(QWidget *widget, char key, BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { keyEvent(Click, widget, key, modifier, delay); }
    inline static void keyPress(QWidget *widget, BobUI::Key key, BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { keyEvent(Press, widget, key, modifier, delay); }
    inline static void keyRelease(QWidget *widget, BobUI::Key key, BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { keyEvent(Release, widget, key, modifier, delay); }
    inline static void keyClick(QWidget *widget, BobUI::Key key, BobUI::KeyboardModifiers modifier = BobUI::NoModifier, int delay=-1)
    { keyEvent(Click, widget, key, modifier, delay); }

#if BOBUI_CONFIG(shortcut)
    inline static void keySequence(QWidget *widget, const QKeySequence &keySequence)
    {
        for (int i = 0; i < keySequence.count(); ++i) {
            const BobUI::Key key = keySequence[i].key();
            const BobUI::KeyboardModifiers modifiers = keySequence[i].keyboardModifiers();
            keyClick(widget, key, modifiers);
        }
    }
#endif

#endif // BOBUI_WIDGETS_LIB

}

BOBUI_END_NAMESPACE

#endif // BOBUIESTKEYBOARD_H
