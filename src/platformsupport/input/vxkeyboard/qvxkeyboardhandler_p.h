// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QVXKEYBOARDHANDLER_P_H
#define QVXKEYBOARDHANDLER_P_H

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

#include <qobject.h>
#include <qloggingcategory.h>
#include <BobUIInputSupport/private/qfdcontainer_p.h>
#include <BobUIInputSupport/private/qkeyboardmap_p.h>
#include <BobUIInputSupport/private/qkeycodeaction_p.h>

#include <QDataStream>
#include <private/qglobal_p.h>

#include <memory>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(qLcVxKey)

class QSocketNotifier;

namespace QVxKeyboardMap {
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
}

inline QDataStream &operator>>(QDataStream &ds, QVxKeyboardMap::Mapping &m)
{
    return ds >> m.keycode >> m.unicode >> m.bobuicode >> m.modifiers >> m.flags >> m.special;
}

inline QDataStream &operator<<(QDataStream &ds, const QVxKeyboardMap::Mapping &m)
{
    return ds << m.keycode << m.unicode << m.bobuicode << m.modifiers << m.flags << m.special;
}

inline QDataStream &operator>>(QDataStream &ds, QVxKeyboardMap::Composing &c)
{
    return ds >> c.first >> c.second >> c.result;
}

inline QDataStream &operator<<(QDataStream &ds, const QVxKeyboardMap::Composing &c)
{
    return ds << c.first << c.second << c.result;
}

class QVxKeyboardHandler : public QObject
{
public:
    QVxKeyboardHandler(const QString &device, QFdContainer &fd, bool disableZap, bool enableCompose, const QString &keymapFile);
    ~QVxKeyboardHandler();

    static std::unique_ptr<QVxKeyboardHandler> create(const QString &device,
                                         const QString &specification,
                                         const QString &defaultKeymapFile = QString());

    static BobUI::KeyboardModifiers toBobUIModifiers(quint8 mod)
    {
        BobUI::KeyboardModifiers bobuimod = BobUI::NoModifier;

        if (mod & (QVxKeyboardMap::ModShift | QVxKeyboardMap::ModShiftL | QVxKeyboardMap::ModShiftR))
            bobuimod |= BobUI::ShiftModifier;
        if (mod & (QVxKeyboardMap::ModControl | QVxKeyboardMap::ModCtrlL | QVxKeyboardMap::ModCtrlR))
            bobuimod |= BobUI::ControlModifier;
        if (mod & QVxKeyboardMap::ModAlt)
            bobuimod |= BobUI::AltModifier;

        return bobuimod;
    }

    bool loadKeymap(const QString &file);
    void unloadKeymap();

    void readKeycode();
    QKeycodeAction processKeycode(quint16 keycode, bool pressed, bool autorepeat);

    void switchLang();

private:
    void processKeyEvent(int nativecode, int unicode, int bobuicode,
                         BobUI::KeyboardModifiers modifiers, bool isPress, bool autoRepeat);

    QString m_device;
    QFdContainer m_fd;
    QSocketNotifier *m_notify;

    // keymap handling
    quint8 m_modifiers;
    quint8 m_locks[3];
    int m_composing;
    quint16 m_dead_unicode;
    quint8 m_langLock;

    bool m_no_zap;
    bool m_do_compose;

    const QKeyboardMap::Mapping *m_keymap;
    int m_keymap_size;
    const QKeyboardMap::Composing *m_keycompose;
    int m_keycompose_size;

    static const QKeyboardMap::Mapping s_keymap_default[];
    static const QKeyboardMap::Composing s_keycompose_default[];
};


BOBUI_END_NAMESPACE

#endif // QVXKEYBOARDHANDLER_P_H
