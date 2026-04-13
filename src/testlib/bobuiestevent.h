// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTEVENT_H
#define BOBUIESTEVENT_H

#if 0
// inform syncbobui
#pragma bobui_no_master_include
#endif

#include <BobUITest/bobuitestglobal.h>
#ifdef BOBUI_GUI_LIB
#include <BobUITest/bobuiestkeyboard.h>
#include <BobUITest/bobuiestmouse.h>
#endif
#include <BobUITest/bobuiestsystem.h>

#include <BobUICore/qlist.h>

#include <stdlib.h>

BOBUI_BEGIN_NAMESPACE

#ifdef BOBUI_WIDGETS_LIB
# define BOBUI_ONLY_WIDGETLIB_USES
#else
# define BOBUI_ONLY_WIDGETLIB_USES Q_DECL_UNUSED_MEMBER
#endif

BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_CLANG("-Wweak-vtables") // BOBUIBUG-1044486

class BOBUIestEvent
{
public:
#ifdef BOBUI_WIDGETS_LIB
    virtual void simulate(QWidget *w) = 0;
#endif
    virtual BOBUIestEvent *clone() const = 0;

    virtual ~BOBUIestEvent() = default; // ### FIXME: weak vtable (BOBUIBUG-104486)
};

#ifdef BOBUI_GUI_LIB
class BOBUIestKeyEvent: public BOBUIestEvent
{
public:
    ~BOBUIestKeyEvent() override = default; // ### FIXME: weak vtable (BOBUIBUG-104486)

    inline BOBUIestKeyEvent(BOBUIest::KeyAction action, BobUI::Key key, BobUI::KeyboardModifiers modifiers, int delay)
        : _action(action), _delay(delay), _modifiers(modifiers), _ascii(0), _key(key) {}
    inline BOBUIestKeyEvent(BOBUIest::KeyAction action, char ascii, BobUI::KeyboardModifiers modifiers, int delay)
        : _action(action), _delay(delay), _modifiers(modifiers),
          _ascii(ascii), _key(BobUI::Key_unknown) {}
    inline BOBUIestEvent *clone() const override { return new BOBUIestKeyEvent(*this); }

#ifdef BOBUI_WIDGETS_LIB
    inline void simulate(QWidget *w) override
    {
        if (_ascii == 0)
            BOBUIest::keyEvent(_action, w, _key, _modifiers, _delay);
        else
            BOBUIest::keyEvent(_action, w, _ascii, _modifiers, _delay);
    }
#endif

protected:
    BOBUIest::KeyAction _action;
    int _delay;
    BobUI::KeyboardModifiers _modifiers;
    char _ascii;
    BobUI::Key _key;
};

class BOBUIestKeyClicksEvent: public BOBUIestEvent
{
public:
    ~BOBUIestKeyClicksEvent() override = default; // ### FIXME: weak vtables (BOBUIBUG-104486)

    inline BOBUIestKeyClicksEvent(const QString &keys, BobUI::KeyboardModifiers modifiers, int delay)
        : _keys(keys), _modifiers(modifiers), _delay(delay) {}
    inline BOBUIestEvent *clone() const override { return new BOBUIestKeyClicksEvent(*this); }

#ifdef BOBUI_WIDGETS_LIB
    inline void simulate(QWidget *w) override
    {
        BOBUIest::keyClicks(w, _keys, _modifiers, _delay);
    }
#endif

private:
    BOBUI_ONLY_WIDGETLIB_USES QString _keys;
    BOBUI_ONLY_WIDGETLIB_USES BobUI::KeyboardModifiers _modifiers;
    BOBUI_ONLY_WIDGETLIB_USES int _delay;
};

class BOBUIestMouseEvent: public BOBUIestEvent
{
public:
    ~BOBUIestMouseEvent() override = default; // ### FIXME: weak vtables (BOBUIBUG-104486)

    inline BOBUIestMouseEvent(BOBUIest::MouseAction action, BobUI::MouseButton button,
            BobUI::KeyboardModifiers modifiers, QPoint position, int delay)
        : _action(action), _button(button), _modifiers(modifiers), _pos(position), _delay(delay) {}
    inline BOBUIestEvent *clone() const override { return new BOBUIestMouseEvent(*this); }

#ifdef BOBUI_WIDGETS_LIB
    inline void simulate(QWidget *w) override
    {
        BOBUIest::mouseEvent(_action, w, _button, _modifiers, _pos, _delay);
    }
#endif

private:
    BOBUI_ONLY_WIDGETLIB_USES BOBUIest::MouseAction _action;
    BOBUI_ONLY_WIDGETLIB_USES BobUI::MouseButton _button;
    BOBUI_ONLY_WIDGETLIB_USES BobUI::KeyboardModifiers _modifiers;
    BOBUI_ONLY_WIDGETLIB_USES QPoint _pos;
    BOBUI_ONLY_WIDGETLIB_USES int _delay;
};
#endif //BOBUI_GUI_LIB


class BOBUIestDelayEvent: public BOBUIestEvent
{
public:
    inline BOBUIestDelayEvent(int msecs): _delay(msecs) {}
    ~BOBUIestDelayEvent() override = default; // ### FIXME: weak vtables (BOBUIBUG-104486)

    inline BOBUIestEvent *clone() const override { return new BOBUIestDelayEvent(*this); }

#ifdef BOBUI_WIDGETS_LIB
    inline void simulate(QWidget * /*w*/) override { BOBUIest::qWait(_delay); }
#endif

private:
    BOBUI_ONLY_WIDGETLIB_USES int _delay;
};

class BOBUIestEventList: public QList<BOBUIestEvent *>
{
public:
    inline BOBUIestEventList() {}
    inline BOBUIestEventList(const BOBUIestEventList &other): QList<BOBUIestEvent *>()
    { for (int i = 0; i < other.size(); ++i) append(other.at(i)->clone()); }
    inline ~BOBUIestEventList()
    { clear(); }
    inline void clear()
    { qDeleteAll(*this); QList<BOBUIestEvent *>::clear(); }

#ifdef BOBUI_GUI_LIB
    inline void addKeyClick(BobUI::Key bobuiKey, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier, int msecs = -1)
    { addKeyEvent(BOBUIest::Click, bobuiKey, modifiers, msecs); }
    inline void addKeyPress(BobUI::Key bobuiKey, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier, int msecs = -1)
    { addKeyEvent(BOBUIest::Press, bobuiKey, modifiers, msecs); }
    inline void addKeyRelease(BobUI::Key bobuiKey, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier, int msecs = -1)
    { addKeyEvent(BOBUIest::Release, bobuiKey, modifiers, msecs); }
    inline void addKeyEvent(BOBUIest::KeyAction action, BobUI::Key bobuiKey,
                            BobUI::KeyboardModifiers modifiers = BobUI::NoModifier, int msecs = -1)
    { append(new BOBUIestKeyEvent(action, bobuiKey, modifiers, msecs)); }

    inline void addKeyClick(char ascii, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier, int msecs = -1)
    { addKeyEvent(BOBUIest::Click, ascii, modifiers, msecs); }
    inline void addKeyPress(char ascii, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier, int msecs = -1)
    { addKeyEvent(BOBUIest::Press, ascii, modifiers, msecs); }
    inline void addKeyRelease(char ascii, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier, int msecs = -1)
    { addKeyEvent(BOBUIest::Release, ascii, modifiers, msecs); }
    inline void addKeyClicks(const QString &keys, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier, int msecs = -1)
    { append(new BOBUIestKeyClicksEvent(keys, modifiers, msecs)); }
    inline void addKeyEvent(BOBUIest::KeyAction action, char ascii, BobUI::KeyboardModifiers modifiers = BobUI::NoModifier, int msecs = -1)
    { append(new BOBUIestKeyEvent(action, ascii, modifiers, msecs)); }

    inline void addMousePress(BobUI::MouseButton button, BobUI::KeyboardModifiers stateKey = BobUI::KeyboardModifiers(),
                              QPoint pos = QPoint(), int delay=-1)
    { append(new BOBUIestMouseEvent(BOBUIest::MousePress, button, stateKey, pos, delay)); }
    inline void addMouseRelease(BobUI::MouseButton button, BobUI::KeyboardModifiers stateKey = BobUI::KeyboardModifiers(),
                                QPoint pos = QPoint(), int delay=-1)
    { append(new BOBUIestMouseEvent(BOBUIest::MouseRelease, button, stateKey, pos, delay)); }
    inline void addMouseClick(BobUI::MouseButton button, BobUI::KeyboardModifiers stateKey = BobUI::KeyboardModifiers(),
                              QPoint pos = QPoint(), int delay=-1)
    { append(new BOBUIestMouseEvent(BOBUIest::MouseClick, button, stateKey, pos, delay)); }
    inline void addMouseDClick(BobUI::MouseButton button, BobUI::KeyboardModifiers stateKey = BobUI::KeyboardModifiers(),
                            QPoint pos = QPoint(), int delay=-1)
    { append(new BOBUIestMouseEvent(BOBUIest::MouseDClick, button, stateKey, pos, delay)); }
    inline void addMouseMove(QPoint pos = QPoint(), int delay=-1)
    { append(new BOBUIestMouseEvent(BOBUIest::MouseMove, BobUI::NoButton, BobUI::KeyboardModifiers(), pos, delay)); }
#endif //BOBUI_GUI_LIB

    inline void addDelay(int msecs)
    { append(new BOBUIestDelayEvent(msecs)); }

#ifdef BOBUI_WIDGETS_LIB
    inline void simulate(QWidget *w)
    {
        for (int i = 0; i < size(); ++i)
            at(i)->simulate(w);
    }
#endif
};

#undef BOBUI_ONLY_WIDGETLIB_USES

BOBUI_WARNING_POP // Clang -Wweak-vtables

BOBUI_END_NAMESPACE

Q_DECLARE_METATYPE(BOBUIestEventList)

#endif
