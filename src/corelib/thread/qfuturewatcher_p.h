// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QFUTUREWATCHER_P_H
#define QFUTUREWATCHER_P_H

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

#include <BobUICore/qfuturewatcher.h>

#include "qfutureinterface_p.h"
#include <qlist.h>

#include <private/qobject_p.h>

BOBUI_REQUIRE_CONFIG(future);

BOBUI_BEGIN_NAMESPACE

class QFutureWatcherBasePrivate : public QObjectPrivate,
                                  public QFutureCallOutInterface
{
    Q_DECLARE_PUBLIC(QFutureWatcherBase)

public:
    QFutureWatcherBasePrivate();

    void postCallOutEvent(const QFutureCallOutEvent &callOutEvent) override;
    void callOutInterfaceDisconnected() override;

    void sendCallOutEvent(QFutureCallOutEvent *event);

    QAtomicInt pendingResultsReady;
    int maximumPendingResultsReady;

    QAtomicInt resultAtConnected;
};

BOBUI_END_NAMESPACE

#endif
