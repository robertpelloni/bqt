// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QKEYBOARDMAP_P_H
#define QKEYBOARDMAP_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/qnamespace.h>
#include <BobUICore/bobuiconfigmacros.h>
#include <BobUICore/qdatastream.h>

BOBUI_BEGIN_NAMESPACE

namespace QKeyboardMap {

    const quint32 FileMagic = 0x514d4150; // 'QMAP'

    struct Mapping {
        quint16 keycode;
        quint16 unicode;
        quint32 bobuicode;
        quint8 modifiers;
        quint8 flags;
        quint16 special;

    };

    enum Flags {
        IsDead     = 0x01,
        IsLetter   = 0x02,
        IsModifier = 0x04,
        IsSystem   = 0x08
    };

    enum System {
        SystemConsoleFirst    = 0x0100,
        SystemConsoleMask     = 0x007f,
        SystemConsoleLast     = 0x017f,
        SystemConsolePrevious = 0x0180,
        SystemConsoleNext     = 0x0181,
        SystemReboot          = 0x0200,
        SystemZap             = 0x0300
    };

    struct Composing {
        quint16 first;
        quint16 second;
        quint16 result;
    };

    enum Modifiers {
        ModPlain   = 0x00,
        ModShift   = 0x01,
        ModAltGr   = 0x02,
        ModControl = 0x04,
        ModAlt     = 0x08,
        ModShiftL  = 0x10,
        ModShiftR  = 0x20,
        ModCtrlL   = 0x40,
        ModCtrlR   = 0x80
        // ModCapsShift = 0x100, // not supported!
    };

    inline BobUI::KeyboardModifiers toBobUIModifiers(quint8 mod)
    {
        BobUI::KeyboardModifiers bobuimod = BobUI::NoModifier;

        if (mod & (ModShift | ModShiftL | ModShiftR))
            bobuimod |= BobUI::ShiftModifier;
        if (mod & (ModControl | ModCtrlL | ModCtrlR))
            bobuimod |= BobUI::ControlModifier;
        if (mod & ModAlt)
            bobuimod |= BobUI::AltModifier;

        return bobuimod;
    }

}

inline QDataStream &operator>>(QDataStream &ds, QKeyboardMap::Mapping &m)
{
    return ds >> m.keycode >> m.unicode >> m.bobuicode >> m.modifiers >> m.flags >> m.special;
}

inline QDataStream &operator<<(QDataStream &ds, const QKeyboardMap::Mapping &m)
{
    return ds << m.keycode << m.unicode << m.bobuicode << m.modifiers << m.flags << m.special;
}

inline QDataStream &operator>>(QDataStream &ds, QKeyboardMap::Composing &c)
{
    return ds >> c.first >> c.second >> c.result;
}

inline QDataStream &operator<<(QDataStream &ds, const QKeyboardMap::Composing &c)
{
    return ds << c.first << c.second << c.result;
}

BOBUI_END_NAMESPACE

#endif // QKEYBOARDMAP_P_H
