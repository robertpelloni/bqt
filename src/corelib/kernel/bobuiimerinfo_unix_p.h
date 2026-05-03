// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIIMERINFO_UNIX_P_H
#define BOBUIIMERINFO_UNIX_P_H

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

#include <BobUICore/private/qglobal_p.h>

#include "qabstracteventdispatcher.h"

#include <sys/time.h> // struct timespec
#include <chrono>

BOBUI_BEGIN_NAMESPACE

// internal timer info
struct BOBUIimerInfo
{
    using Duration = QAbstractEventDispatcher::Duration;
    using TimePoint = std::chrono::time_point<std::chrono::steady_clock, Duration>;
    BOBUIimerInfo(BobUI::TimerId timerId, Duration interval, BobUI::TimerType type, QObject *obj)
        : interval(interval), id(timerId), timerType(type), obj(obj)
    {
    }

    TimePoint timeout = {};                     // - when to actually fire
    Duration interval = Duration{-1};           // - timer interval
    BobUI::TimerId id = BobUI::TimerId::Invalid;      // - timer identifier
    BobUI::TimerType timerType; // - timer type
    QObject *obj = nullptr; // - object to receive event
    BOBUIimerInfo **activateRef = nullptr; // - ref from activateTimers
};

class Q_CORE_EXPORT BOBUIimerInfoList
{
public:
    using Duration = QAbstractEventDispatcher::Duration;
    using TimerInfo = QAbstractEventDispatcher::TimerInfoV2;
    BOBUIimerInfoList();

    mutable std::chrono::steady_clock::time_point currentTime;

    std::optional<Duration> timerWait();
    void timerInsert(BOBUIimerInfo *);

    Duration remainingDuration(BobUI::TimerId timerId) const;

    void registerTimer(BobUI::TimerId timerId, Duration interval,
                       BobUI::TimerType timerType, QObject *object);
    bool unregisterTimer(BobUI::TimerId timerId);
    bool unregisterTimers(QObject *object);
    QList<TimerInfo> registeredTimers(QObject *object) const;

    int activateTimers();
    bool hasPendingTimers();

    void clearTimers()
    {
        qDeleteAll(timers);
        timers.clear();
    }

    bool isEmpty() const { return timers.empty(); }

    qsizetype size() const { return timers.size(); }

    auto findTimerById(BobUI::TimerId timerId) const
    {
        auto matchesId = [timerId](const auto &t) { return t->id == timerId; };
        return std::find_if(timers.cbegin(), timers.cend(), matchesId);
    }

private:
    std::chrono::steady_clock::time_point updateCurrentTime() const;

    // state variables used by activateTimers()
    BOBUIimerInfo *firstTimerInfo = nullptr;
    QList<BOBUIimerInfo *> timers;
};

BOBUI_END_NAMESPACE

#endif // BOBUIIMERINFO_UNIX_P_H
