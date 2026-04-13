// Copyright (C) 2022 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSINGLESHOTTIMER_P_H
#define QSINGLESHOTTIMER_P_H

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

#include <BobUICore/qobject.h>
#include <BobUICore/qabstracteventdispatcher.h>
#include <BobUICore/qbasictimer.h>
#include <BobUICore/qcoreevent.h>
#include <BobUICore/qdeadlinetimer.h>
#include <BobUICore/qnamespace.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIPrivate {
class QSlotObjectBase;
}

class Q_AUTOTEST_EXPORT QSingleShotTimer : public QObject
{
    Q_OBJECT

    QBasicTimer timer;

public:
    // use the same duration type
    using Duration = QAbstractEventDispatcher::Duration;

    explicit QSingleShotTimer(Duration interval, BobUI::TimerType timerType,
                              const QObject *r, const char *member);
    explicit QSingleShotTimer(Duration interval, BobUI::TimerType timerType,
                              const QObject *r, BobUIPrivate::QSlotObjectBase *slotObj);
    ~QSingleShotTimer() override;

    void startTimerForReceiver(Duration interval, BobUI::TimerType timerType,
                               const QObject *receiver);
Q_SIGNALS:
    void timeout();

private:
    class StartTimerEvent : public BOBUIimerEvent
    {
    public:
        StartTimerEvent(QSingleShotTimer *timer, const QDeadlineTimer &deadline)
            : BOBUIimerEvent(BobUI::TimerId::Invalid), timer(timer), deadline(deadline)
        {}

        std::unique_ptr<QSingleShotTimer> timer;
        QDeadlineTimer deadline;
    };

    void timerFinished();
    void timerEvent(BOBUIimerEvent *) override;
};

BOBUI_END_NAMESPACE

#endif // QSINGLESHOTTIMER_P_H
