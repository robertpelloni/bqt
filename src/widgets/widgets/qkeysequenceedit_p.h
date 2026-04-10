// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2013 Ivan Komissarov.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QKEYSEQUENCEEDIT_P_H
#define QKEYSEQUENCEEDIT_P_H

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

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include "qkeysequenceedit.h"

#include <BobUICore/qbasictimer.h>
#include <private/qwidget_p.h>
#include <private/qkeysequence_p.h>

BOBUI_REQUIRE_CONFIG(keysequenceedit);

BOBUI_BEGIN_NAMESPACE

class QLineEdit;

class QKeySequenceEditPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QKeySequenceEdit)
public:
    void init();
    int translateModifiers(BobUI::KeyboardModifiers state, const QString &text);
    void resetState();
    void finishEditing();
    void rebuildKeySequence()
    { keySequence = QKeySequence(key[0], key[1], key[2], key[3]); }

    QLineEdit *lineEdit;
    QKeySequence keySequence;
    int keyNum;
    int maximumSequenceLength = QKeySequencePrivate::MaxKeyCount;
    QKeyCombination key[QKeySequencePrivate::MaxKeyCount];
    int prevKey;
    QBasicTimer releaseTimer;
    QList<QKeyCombination> finishingKeyCombinations;
};

BOBUI_END_NAMESPACE

#endif // QKEYSEQUENCEEDIT_P_H
