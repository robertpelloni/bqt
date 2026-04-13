// Copyright (C) 2011 - 2012 Research In Motion
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQNXKEYTRANSLATOR_H
#define QQNXKEYTRANSLATOR_H

#include <sys/keycodes.h>

BOBUI_BEGIN_NAMESPACE

int bobuiKeyForPrivateUseQnxKey( int key )
{
    switch (key) {
    case KEYCODE_PAUSE:       return BobUI::Key_Pause;
    case KEYCODE_SCROLL_LOCK: return BobUI::Key_ScrollLock;
    case KEYCODE_PRINT:       return BobUI::Key_Print;
    case KEYCODE_SYSREQ:      return BobUI::Key_SysReq;
//    case KEYCODE_BREAK:
    case KEYCODE_ESCAPE:      return BobUI::Key_Escape;
    case KEYCODE_BACKSPACE:   return BobUI::Key_Backspace;
    case KEYCODE_TAB:         return BobUI::Key_Tab;
    case KEYCODE_BACK_TAB:    return BobUI::Key_Backtab;
    case KEYCODE_RETURN:      return BobUI::Key_Return;
    case KEYCODE_CAPS_LOCK:   return BobUI::Key_CapsLock;
    case KEYCODE_LEFT_SHIFT:  return BobUI::Key_Shift;
    case KEYCODE_RIGHT_SHIFT: return BobUI::Key_Shift;
    case KEYCODE_LEFT_CTRL:   return BobUI::Key_Control;
    case KEYCODE_RIGHT_CTRL:  return BobUI::Key_Control;
    case KEYCODE_LEFT_ALT:    return BobUI::Key_Alt;
    case KEYCODE_RIGHT_ALT:   return BobUI::Key_Alt;
    case KEYCODE_MENU:        return BobUI::Key_Menu;
    case KEYCODE_LEFT_HYPER:  return BobUI::Key_Hyper_L;
    case KEYCODE_RIGHT_HYPER: return BobUI::Key_Hyper_R;
    case KEYCODE_INSERT:      return BobUI::Key_Insert;
    case KEYCODE_HOME:        return BobUI::Key_Home;
    case KEYCODE_PG_UP:       return BobUI::Key_PageUp;
    case KEYCODE_DELETE:      return BobUI::Key_Delete;
    case KEYCODE_END:         return BobUI::Key_End;
    case KEYCODE_PG_DOWN:     return BobUI::Key_PageDown;
    case KEYCODE_LEFT:        return BobUI::Key_Left;
    case KEYCODE_RIGHT:       return BobUI::Key_Right;
    case KEYCODE_UP:          return BobUI::Key_Up;
    case KEYCODE_DOWN:        return BobUI::Key_Down;
    case KEYCODE_NUM_LOCK:    return BobUI::Key_NumLock;
    case KEYCODE_KP_PLUS:     return BobUI::Key_Plus;
    case KEYCODE_KP_MINUS:    return BobUI::Key_Minus;
    case KEYCODE_KP_MULTIPLY: return BobUI::Key_Asterisk;
    case KEYCODE_KP_DIVIDE:   return BobUI::Key_Slash;
    case KEYCODE_KP_ENTER:    return BobUI::Key_Enter;
    case KEYCODE_KP_HOME:     return BobUI::Key_Home;
    case KEYCODE_KP_UP:       return BobUI::Key_Up;
    case KEYCODE_KP_PG_UP:    return BobUI::Key_PageUp;
    case KEYCODE_KP_LEFT:     return BobUI::Key_Left;
    case KEYCODE_KP_FIVE:     return BobUI::Key_Clear;
    case KEYCODE_KP_RIGHT:    return BobUI::Key_Right;
    case KEYCODE_KP_END:      return BobUI::Key_End;
    case KEYCODE_KP_DOWN:     return BobUI::Key_Down;
    case KEYCODE_KP_PG_DOWN:  return BobUI::Key_PageDown;
    case KEYCODE_KP_INSERT:   return BobUI::Key_Insert;
    case KEYCODE_KP_DELETE:   return BobUI::Key_Delete;
    case KEYCODE_F1:          return BobUI::Key_F1;
    case KEYCODE_F2:          return BobUI::Key_F2;
    case KEYCODE_F3:          return BobUI::Key_F3;
    case KEYCODE_F4:          return BobUI::Key_F4;
    case KEYCODE_F5:          return BobUI::Key_F5;
    case KEYCODE_F6:          return BobUI::Key_F6;
    case KEYCODE_F7:          return BobUI::Key_F7;
    case KEYCODE_F8:          return BobUI::Key_F8;
    case KEYCODE_F9:          return BobUI::Key_F9;
    case KEYCODE_F10:         return BobUI::Key_F10;
    case KEYCODE_F11:         return BobUI::Key_F11;
    case KEYCODE_F12:         return BobUI::Key_F12;

    // See keycodes.h for more, but these are all the basics. And printables are already included.

    default:
#if defined(QQNXEVENTTHREAD_DEBUG)
        qDebug("QQNX: unknown key for translation: %d", key);
#endif
        break;
    }

    return BobUI::Key_unknown;
}

QString keyStringForPrivateUseQnxKey( int key )
{
    switch (key) {
    case KEYCODE_ESCAPE:    return QStringLiteral("\x1B");
    case KEYCODE_BACKSPACE: return QStringLiteral("\b");
    case KEYCODE_TAB:       return QStringLiteral("\t");
    case KEYCODE_RETURN:    return QStringLiteral("\r");
    case KEYCODE_DELETE:    return QStringLiteral("\x7F");
    case KEYCODE_KP_ENTER:  return QStringLiteral("\r");
    }

    return QString();
}

bool isKeypadKey( int key )
{
    switch (key)
    {
    case KEYCODE_KP_PLUS:
    case KEYCODE_KP_MINUS:
    case KEYCODE_KP_MULTIPLY:
    case KEYCODE_KP_DIVIDE:
    case KEYCODE_KP_ENTER:
    case KEYCODE_KP_HOME:
    case KEYCODE_KP_UP:
    case KEYCODE_KP_PG_UP:
    case KEYCODE_KP_LEFT:
    case KEYCODE_KP_FIVE:
    case KEYCODE_KP_RIGHT:
    case KEYCODE_KP_END:
    case KEYCODE_KP_DOWN:
    case KEYCODE_KP_PG_DOWN:
    case KEYCODE_KP_INSERT:
    case KEYCODE_KP_DELETE:
        return true;
    default:
        break;
    }

    return false;
}

BOBUI_END_NAMESPACE

#endif // QQNXKEYTRANSLATOR_H
