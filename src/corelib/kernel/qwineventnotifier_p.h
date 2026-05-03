// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINEVENTNOTIFIER_P_H
#define QWINEVENTNOTIFIER_P_H

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

#include "qwineventnotifier.h"

#include <private/qobject_p.h>
#include <BobUICore/qatomic.h>
#include <BobUICore/bobui_windows.h>

BOBUI_BEGIN_NAMESPACE

class QWinEventNotifierPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QWinEventNotifier)
public:
    QWinEventNotifierPrivate() : QWinEventNotifierPrivate(0, false) {}
    QWinEventNotifierPrivate(HANDLE h, bool e);
    virtual ~QWinEventNotifierPrivate();

    static void CALLBACK waitCallback(PTP_CALLBACK_INSTANCE instance, PVOID context,
                                      PTP_WAIT wait, TP_WAIT_RESULT waitResult);

    HANDLE handleToEvent;
    PTP_WAIT waitObject = NULL;

    enum PostingState { NotPosted = 0, Posted, IgnorePosted };
    QAtomicInt winEventActPosted;
    bool enabled;
    bool registered;
};

BOBUI_END_NAMESPACE

#endif // QWINEVENTNOTIFIER_P_H
