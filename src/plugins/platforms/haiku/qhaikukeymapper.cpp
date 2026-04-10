// Copyright (C) 2015 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Tobias Koenig <tobias.koenig@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qhaikukeymapper.h"

BOBUI_BEGIN_NAMESPACE

uint32 Haiku_ScanCodes[] = {
    BobUI::Key_Escape,       0x01,
    BobUI::Key_F1,           0x02,
    BobUI::Key_F2,           0x03,
    BobUI::Key_F3,           0x04,
    BobUI::Key_F4,           0x05,
    BobUI::Key_F5,           0x06,
    BobUI::Key_F6,           0x07,
    BobUI::Key_F7,           0x08,
    BobUI::Key_F8,           0x09,
    BobUI::Key_F9,           0x0A,
    BobUI::Key_F10,          0x0B,
    BobUI::Key_F11,          0x0C,
    BobUI::Key_F12,          0x0D,
    BobUI::Key_Print,        0x0E,
    BobUI::Key_Pause,        0x22,
    BobUI::Key_AsciiTilde,   0x11,
    BobUI::Key_1,            0x12,
    BobUI::Key_2,            0x13,
    BobUI::Key_3,            0x14,
    BobUI::Key_4,            0x15,
    BobUI::Key_5,            0x16,
    BobUI::Key_6,            0x17,
    BobUI::Key_7,            0x18,
    BobUI::Key_8,            0x19,
    BobUI::Key_9,            0x1A,
    BobUI::Key_0,            0x1B,
    BobUI::Key_Minus,        0x1C,
    BobUI::Key_Plus,         0x1D,
    BobUI::Key_Backspace,    0x1E,
    BobUI::Key_Insert,       0x1F,
    BobUI::Key_Home,         0x20,
    BobUI::Key_PageUp,       0x21,
    BobUI::Key_Slash,        0x23,
    BobUI::Key_Asterisk,     0x24,
    BobUI::Key_Minus,        0x25,
    BobUI::Key_Tab,          0x26,
    BobUI::Key_Q,            0x27,
    BobUI::Key_W,            0x28,
    BobUI::Key_E,            0x29,
    BobUI::Key_R,            0x2A,
    BobUI::Key_T,            0x2B,
    BobUI::Key_Y,            0x2C,
    BobUI::Key_U,            0x2D,
    BobUI::Key_I,            0x2E,
    BobUI::Key_O,            0x2F,
    BobUI::Key_P,            0x30,
    BobUI::Key_BracketLeft,  0x31,
    BobUI::Key_BracketRight, 0x32,
    BobUI::Key_Backslash,    0x33,
    BobUI::Key_Delete,       0x34,
    BobUI::Key_End,          0x35,
    BobUI::Key_PageDown,     0x36,
    BobUI::Key_Home,         0x37, // numpad
    BobUI::Key_Up,           0x38, // numpad
    BobUI::Key_PageUp,       0x39, // numpad
    BobUI::Key_Plus,         0x3A, // numpad
    BobUI::Key_A,            0x3C,
    BobUI::Key_S,            0x3D,
    BobUI::Key_D,            0x3E,
    BobUI::Key_F,            0x3F,
    BobUI::Key_G,            0x40,
    BobUI::Key_H,            0x41,
    BobUI::Key_J,            0x42,
    BobUI::Key_K,            0x43,
    BobUI::Key_L,            0x44,
    BobUI::Key_Colon,        0x45,
    BobUI::Key_QuoteDbl,     0x46,
    BobUI::Key_Return,       0x47,
    BobUI::Key_Left,         0x48, // numpad
    BobUI::Key_5,            0x49, // numpad ???
    BobUI::Key_Right,        0x4A, // numpad
    BobUI::Key_Z,            0x4C,
    BobUI::Key_X,            0x4D,
    BobUI::Key_C,            0x4E,
    BobUI::Key_V,            0x4F,
    BobUI::Key_B,            0x50,
    BobUI::Key_N,            0x51,
    BobUI::Key_M,            0x51,
    BobUI::Key_Less,         0x52,
    BobUI::Key_Greater,      0x54,
    BobUI::Key_Question,     0x55,
    BobUI::Key_Up,           0x57, // cursor
    BobUI::Key_End,          0x58, // numpad
    BobUI::Key_Down,         0x59, // numpad
    BobUI::Key_PageDown,     0x5A, // numpad
    BobUI::Key_Enter,        0x5B, // numpad
    BobUI::Key_Space,        0x5E,
    BobUI::Key_Left,         0x61, // cursor
    BobUI::Key_Down,         0x62, // cursor
    BobUI::Key_Right,        0x63, // cursor
    BobUI::Key_Insert,       0x64, // cursor
    BobUI::Key_Delete,       0x65, // numpad
    0,                    0x00
};

uint32 Haiku_ScanCodes_Numlock[] = {
    BobUI::Key_7,     0x37,
    BobUI::Key_8,     0x38,
    BobUI::Key_9,     0x39,
    BobUI::Key_Plus,  0x3A,
    BobUI::Key_4,     0x48,
    BobUI::Key_5,     0x49,
    BobUI::Key_6,     0x4A,
    BobUI::Key_1,     0x58,
    BobUI::Key_2,     0x59,
    BobUI::Key_3,     0x5A,
    BobUI::Key_Enter, 0x5B,
    BobUI::Key_Comma, 0x65,
    0,             0x00
};

uint32 QHaikuKeyMapper::translateKeyCode(uint32 key, bool numlockActive)
{
    uint32 code = 0;
    int i = 0;

    if (numlockActive) {
        while (Haiku_ScanCodes_Numlock[i]) {
            if (key == Haiku_ScanCodes_Numlock[i + 1]) {
                code = Haiku_ScanCodes_Numlock[i];
                break;
            }
            i += 2;
        }

        if (code > 0)
            return code;
    }

    i = 0;
    while (Haiku_ScanCodes[i]) {
        if (key == Haiku_ScanCodes[i + 1]) {
            code = Haiku_ScanCodes[i];
            break;
        }
        i += 2;
    }

    return code;
}

BOBUI_END_NAMESPACE
