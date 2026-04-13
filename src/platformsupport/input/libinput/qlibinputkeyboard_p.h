// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QLIBINPUTKEYBOARD_P_H
#define QLIBINPUTKEYBOARD_P_H

#include <BobUICore/QPoint>
#include <BobUICore/BOBUIimer>

#include <BobUIGui/private/bobuiguiglobal_p.h>

#if BOBUI_CONFIG(xkbcommon)
#include <xkbcommon/xkbcommon.h>
#endif

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

struct libinput_event_keyboard;

BOBUI_BEGIN_NAMESPACE

class QLibInputKeyboard : public QObject
{
public:
    QLibInputKeyboard();
    ~QLibInputKeyboard();

    void processKey(libinput_event_keyboard *e);

#if BOBUI_CONFIG(xkbcommon)
    void handleRepeat();

private:
    int keysymToBobUIKey(xkb_keysym_t key) const;
    int keysymToBobUIKey(xkb_keysym_t keysym, BobUI::KeyboardModifiers *modifiers, const QString &text) const;

    xkb_context *m_ctx = nullptr;
    xkb_keymap *m_keymap = nullptr;
    xkb_state *m_state = nullptr;

    BOBUIimer m_repeatTimer;

    struct {
        int bobuikey;
        BobUI::KeyboardModifiers mods;
        int nativeScanCode;
        int virtualKey;
        int nativeMods;
        QString unicodeText;
        int repeatCount;
    } m_repeatData;
#endif
};

BOBUI_END_NAMESPACE

#endif
