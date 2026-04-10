// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef Q_NATIVE_INPUT
#define Q_NATIVE_INPUT

#include <BobUICore>

BOBUI_BEGIN_NAMESPACE
namespace BobUI {
namespace Native {
    enum Status {Success, Failure};
}}
BOBUI_END_NAMESPACE

// ----------------------------------------------------------------------------
// Declare a set of native events that can be used to communicate with
// client applications in an platform independent way
// ----------------------------------------------------------------------------

class QNativeEvent
{
public:
    static const int eventId = 1;

    QNativeEvent(BobUI::KeyboardModifiers modifiers = BobUI::NoModifier);
    virtual ~QNativeEvent() {}
    virtual int id() const { return eventId; }
    virtual QString toString() const = 0;
    BobUI::KeyboardModifiers modifiers; // Yields for mouse events too.
};

class QNativeMouseEvent : public QNativeEvent {
public:
    static const int eventId = 2;

    QNativeMouseEvent() {}
    QNativeMouseEvent(QPoint globalPos, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier);
    virtual ~QNativeMouseEvent() {}
    virtual int id() const { return eventId; }

    QPoint globalPos;
};

class QNativeMouseMoveEvent : public QNativeMouseEvent {
public:
    static const int eventId = 4;

    QNativeMouseMoveEvent() {}
    QNativeMouseMoveEvent(QPoint globalPos, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier);
    virtual ~QNativeMouseMoveEvent() {}
    virtual int id() const { return eventId; }
    virtual QString toString() const;
};

class QNativeMouseButtonEvent : public QNativeMouseEvent {
public:
    static const int eventId = 8;

    QNativeMouseButtonEvent() {}
    QNativeMouseButtonEvent(QPoint globalPos, BobUI::MouseButton button, int clickCount, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier);
    virtual ~QNativeMouseButtonEvent() {}
    virtual int id() const { return eventId; }
    virtual QString toString() const;

    BobUI::MouseButton button;
    int clickCount;
};

class QNativeMouseDragEvent : public QNativeMouseButtonEvent {
public:
    static const int eventId = 16;

    QNativeMouseDragEvent() {}
    QNativeMouseDragEvent(QPoint globalPos, BobUI::MouseButton button, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier);
    virtual ~QNativeMouseDragEvent() {}
    virtual int id() const { return eventId; }
    virtual QString toString() const;
};

class QNativeMouseWheelEvent : public QNativeMouseEvent {
public:
    static const int eventId = 32;

    QNativeMouseWheelEvent() {}
    QNativeMouseWheelEvent(QPoint globalPos, int delta, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier);
    virtual ~QNativeMouseWheelEvent() {}
    virtual int id() const { return eventId; }
    virtual QString toString() const;

    int delta;
};

class QNativeKeyEvent : public QNativeEvent {
    public:
    static const int eventId = 64;

    QNativeKeyEvent() {}
    QNativeKeyEvent(int nativeKeyCode, bool press, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier);
    QNativeKeyEvent(int nativeKeyCode, bool press, QChar character, BobUI::KeyboardModifiers modifiers);
    virtual ~QNativeKeyEvent() {}
    virtual int id() const { return eventId; }
    virtual QString toString() const;

    int nativeKeyCode;
    bool press;
    QChar character;

    // Some BobUI to Native mappings:
    static int Key_A;
    static int Key_B;
    static int Key_C;
    static int Key_1;
    static int Key_Backspace;
    static int Key_Enter;
    static int Key_Del;
};

class QNativeModifierEvent : public QNativeEvent {
public:
    static const int eventId = 128;

    QNativeModifierEvent(BobUI::KeyboardModifiers modifiers = BobUI::NoModifier, int nativeKeyCode = 0);
    virtual ~QNativeModifierEvent() {}
    virtual int id() const { return eventId; }
    virtual QString toString() const;

    int nativeKeyCode;
};

// ----------------------------------------------------------------------------
// Declare a set of related output / input functions for convenience:
// ----------------------------------------------------------------------------

QDebug operator<<(QDebug d, QNativeEvent *e);
QDebug operator<<(QDebug d, const QNativeEvent &e);

BOBUIextStream &operator<<(BOBUIextStream &s, QNativeEvent *e);
BOBUIextStream &operator<<(BOBUIextStream &s, QNativeMouseEvent *e);
BOBUIextStream &operator<<(BOBUIextStream &s, QNativeMouseMoveEvent *e);
BOBUIextStream &operator<<(BOBUIextStream &s, QNativeMouseButtonEvent *e);
BOBUIextStream &operator<<(BOBUIextStream &s, QNativeMouseDragEvent *e);
BOBUIextStream &operator<<(BOBUIextStream &s, QNativeMouseWheelEvent *e);
BOBUIextStream &operator<<(BOBUIextStream &s, QNativeKeyEvent *e);
BOBUIextStream &operator<<(BOBUIextStream &s, QNativeModifierEvent *e);

BOBUIextStream &operator>>(BOBUIextStream &s, QNativeMouseMoveEvent *e);
BOBUIextStream &operator>>(BOBUIextStream &s, QNativeMouseButtonEvent *e);
BOBUIextStream &operator>>(BOBUIextStream &s, QNativeMouseDragEvent *e);
BOBUIextStream &operator>>(BOBUIextStream &s, QNativeMouseWheelEvent *e);
BOBUIextStream &operator>>(BOBUIextStream &s, QNativeKeyEvent *e);
BOBUIextStream &operator>>(BOBUIextStream &s, QNativeModifierEvent *e);

// ----------------------------------------------------------------------------
// Declare the main class that is supposed to be sub-classed by components
// that are to receive native events
// ----------------------------------------------------------------------------

class QNativeInput
{
    public:
    QNativeInput(bool subscribe = true);
    virtual ~QNativeInput();

    // Callback methods. Should be implemented by interested sub-classes:
    void notify(QNativeEvent *event);
    virtual void nativeEvent(QNativeEvent *event);
    virtual void nativeMousePressEvent(QNativeMouseButtonEvent *) {}
    virtual void nativeMouseReleaseEvent(QNativeMouseButtonEvent *) {}
    virtual void nativeMouseMoveEvent(QNativeMouseMoveEvent *) {}
    virtual void nativeMouseDragEvent(QNativeMouseDragEvent *) {}
    virtual void nativeMouseWheelEvent(QNativeMouseWheelEvent *) {}
    virtual void nativeKeyPressEvent(QNativeKeyEvent *) {}
    virtual void nativeKeyReleaseEvent(QNativeKeyEvent *) {}
    virtual void nativeModifierEvent(QNativeModifierEvent *) {}

    // The following methods will differ in implementation from OS to OS:
    static BobUI::Native::Status sendNativeMouseButtonEvent(const QNativeMouseButtonEvent &event);
    static BobUI::Native::Status sendNativeMouseMoveEvent(const QNativeMouseMoveEvent &event);
    static BobUI::Native::Status sendNativeMouseDragEvent(const QNativeMouseDragEvent &event);
    static BobUI::Native::Status sendNativeMouseWheelEvent(const QNativeMouseWheelEvent &event);
    static BobUI::Native::Status sendNativeKeyEvent(const QNativeKeyEvent &event);
    static BobUI::Native::Status sendNativeModifierEvent(const QNativeModifierEvent &event);
    // sendNativeEvent will NOT differ from OS to OS.
    static BobUI::Native::Status sendNativeEvent(const QNativeEvent &event);

    // The following methods will differ in implementation from OS to OS:
    BobUI::Native::Status subscribeForNativeEvents();
    BobUI::Native::Status unsubscribeForNativeEvents();
};

#endif // Q_NATIVE_INPUT
