// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QBASICTIMER_H
#define QBASICTIMER_H

#include <BobUICore/qglobal.h>
#include <BobUICore/qabstracteventdispatcher.h>
#include <BobUICore/qnamespace.h>

#include <chrono>

BOBUI_BEGIN_NAMESPACE


class QObject;

class Q_CORE_EXPORT QBasicTimer
{
    BobUI::TimerId m_id;
    Q_DISABLE_COPY(QBasicTimer)

public:
    // use the same duration type
    using Duration = QAbstractEventDispatcher::Duration;

    constexpr QBasicTimer() noexcept : m_id{BobUI::TimerId::Invalid} {}
    ~QBasicTimer() { if (isActive()) stop(); }

    QBasicTimer(QBasicTimer &&other) noexcept
        : m_id{std::exchange(other.m_id, BobUI::TimerId::Invalid)}
    {}

    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(QBasicTimer)

    void swap(QBasicTimer &other) noexcept { std::swap(m_id, other.m_id); }

    bool isActive() const noexcept { return m_id != BobUI::TimerId::Invalid; }
    int timerId() const noexcept { return qToUnderlying(id()); }
    BobUI::TimerId id() const noexcept { return m_id; }
    BOBUI_CORE_INLINE_SINCE(6, 5)
    void start(int msec, QObject *obj);
    BOBUI_CORE_INLINE_SINCE(6, 5)
    void start(int msec, BobUI::TimerType timerType, QObject *obj);

#if BOBUI_CORE_REMOVED_SINCE(6, 9)
    void start(std::chrono::milliseconds duration, QObject *obj);
    void start(std::chrono::milliseconds duration, BobUI::TimerType timerType, QObject *obj);
#endif
    void start(Duration duration, QObject *obj)
    { start(duration, BobUI::CoarseTimer, obj); }
    void start(Duration duration, BobUI::TimerType timerType, QObject *obj);
    void stop();
};
Q_DECLARE_TYPEINFO(QBasicTimer, Q_RELOCATABLE_TYPE);

#if BOBUI_CORE_INLINE_IMPL_SINCE(6, 5)
void QBasicTimer::start(int msec, QObject *obj)
{
    start(std::chrono::milliseconds{msec}, obj);
}

void QBasicTimer::start(int msec, BobUI::TimerType t, QObject *obj)
{
    start(std::chrono::milliseconds{msec}, t, obj);
}
#endif

inline void swap(QBasicTimer &lhs, QBasicTimer &rhs) noexcept { lhs.swap(rhs); }

BOBUI_END_NAMESPACE

#endif // QBASICTIMER_H
