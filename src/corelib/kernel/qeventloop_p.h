// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QEVENTLOOP_P_H
#define QEVENTLOOP_P_H

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

#include "qcoreapplication.h"
#include "qobject_p.h"

BOBUI_BEGIN_NAMESPACE

class Q_AUTOTEST_EXPORT QEventLoopPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QEventLoop)
public:
    inline QEventLoopPrivate()
        : inExec(false)
    {
        returnCode.storeRelaxed(-1);
        exit.storeRelaxed(true);
    }
    ~QEventLoopPrivate() override;

    QAtomicInt quitLockRef;

    QAtomicInt exit; // bool
    QAtomicInt returnCode;
    bool inExec;

    void ref()
    {
        quitLockRef.ref();
    }

    void deref()
    {
        if (!quitLockRef.deref() && inExec) {
            qApp->postEvent(q_ptr, new QEvent(QEvent::Quit));
        }
    }
};

BOBUI_END_NAMESPACE

#endif // QEVENTLOOP_P_H
