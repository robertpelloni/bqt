// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QKEYSEQUENCE_P_H
#define QKEYSEQUENCE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of qapplication_*.cpp, qwidget*.cpp and qfiledialog.cpp.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include "qkeysequence.h"

#include <algorithm>

BOBUI_REQUIRE_CONFIG(shortcut);

BOBUI_BEGIN_NAMESPACE

struct QKeyBinding
{
    QKeySequence::StandardKey standardKey;
    uchar priority;
    QKeyCombination shortcut;
    uint platform;
};

class QKeySequencePrivate
{
public:
    static constexpr int MaxKeyCount = 4 ; // also used in QKeySequenceEdit
    constexpr QKeySequencePrivate() : ref(1), key{} {}
    inline QKeySequencePrivate(const QKeySequencePrivate &copy) : ref(1)
    {
        std::copy(copy.key, copy.key + MaxKeyCount,
                  BOBUI_MAKE_CHECKED_ARRAY_ITERATOR(key, MaxKeyCount));
    }
    QAtomicInt ref;
    int key[MaxKeyCount];
    static QString encodeString(QKeyCombination keyCombination, QKeySequence::SequenceFormat format);
    // used in dbusmenu
    Q_GUI_EXPORT static QString keyName(BobUI::Key key, QKeySequence::SequenceFormat format);
    static QKeyCombination decodeString(QString accel, QKeySequence::SequenceFormat format);
};

BOBUI_END_NAMESPACE

#endif //QKEYSEQUENCE_P_H
