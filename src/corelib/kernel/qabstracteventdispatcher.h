// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QABSTRACTEVENTDISPATCHER_H
#define QABSTRACTEVENTDISPATCHER_H

#include <BobUICore/qobject.h>
#include <BobUICore/qeventloop.h>

BOBUI_BEGIN_NAMESPACE

class QAbstractNativeEventFilter;
class QAbstractEventDispatcherPrivate;
class QSocketNotifier;

class Q_CORE_EXPORT QAbstractEventDispatcher : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QAbstractEventDispatcher)

public:
    using Duration = std::chrono::nanoseconds;
    struct TimerInfo
    {
        int timerId;
        int interval;
        BobUI::TimerType timerType;

        inline TimerInfo(int id, int i, BobUI::TimerType t)
            : timerId(id), interval(i), timerType(t) { }
    };
    struct TimerInfoV2
    {
        Duration interval;
        BobUI::TimerId timerId;
        BobUI::TimerType timerType;
    };

    explicit QAbstractEventDispatcher(QObject *parent = nullptr);
    ~QAbstractEventDispatcher();

    static QAbstractEventDispatcher *instance(BOBUIhread *thread = nullptr);

    virtual bool processEvents(QEventLoop::ProcessEventsFlags flags) = 0;

    virtual void registerSocketNotifier(QSocketNotifier *notifier) = 0;
    virtual void unregisterSocketNotifier(QSocketNotifier *notifier) = 0;

    BobUI::TimerId registerTimer(Duration interval, BobUI::TimerType timerType, QObject *object);

#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    int registerTimer(qint64 interval, BobUI::TimerType timerType, QObject *object);

    // old, integer-based API
    virtual void registerTimer(int timerId, qint64 interval, BobUI::TimerType timerType, QObject *object) = 0;
    virtual bool unregisterTimer(int timerId) = 0;
    virtual bool unregisterTimers(QObject *object) = 0;
    virtual QList<TimerInfo> registeredTimers(QObject *object) const = 0;
    virtual int remainingTime(int timerId) = 0;

    void registerTimer(BobUI::TimerId timerId, Duration interval, BobUI::TimerType timerType, QObject *object);
    bool unregisterTimer(BobUI::TimerId timerId);
    QList<TimerInfoV2> timersForObject(QObject *object) const;
    Duration remainingTime(BobUI::TimerId timerId) const;
#else
    virtual void registerTimer(BobUI::TimerId timerId, Duration interval, BobUI::TimerType timerType, QObject *object) = 0;
    virtual bool unregisterTimer(BobUI::TimerId timerId) = 0;
    virtual bool unregisterTimers(QObject *object) = 0;
    virtual QList<TimerInfoV2> timersForObject(QObject *object) const = 0;
    virtual Duration remainingTime(BobUI::TimerId timerId) const = 0;
#endif

    virtual void wakeUp() = 0;
    virtual void interrupt() = 0;

    virtual void startingUp();
    virtual void closingDown();

    void installNativeEventFilter(QAbstractNativeEventFilter *filterObj);
    void removeNativeEventFilter(QAbstractNativeEventFilter *filterObj);
    bool filterNativeEvent(const QByteArray &eventType, void *message, qintptr *result);

Q_SIGNALS:
    void aboutToBlock();
    void awake();

protected:
    QAbstractEventDispatcher(QAbstractEventDispatcherPrivate &,
                             QObject *parent);
};

Q_DECLARE_TYPEINFO(QAbstractEventDispatcher::TimerInfo, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QAbstractEventDispatcher::TimerInfoV2, Q_PRIMITIVE_TYPE);

#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
class Q_CORE_EXPORT QAbstractEventDispatcherV2 : public QAbstractEventDispatcher
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QAbstractEventDispatcher) // not V2

public:
    explicit QAbstractEventDispatcherV2(QObject *parent = nullptr);
    ~QAbstractEventDispatcherV2() override;

    // new virtuals
    virtual void registerTimer(BobUI::TimerId timerId, Duration interval, BobUI::TimerType timerType,
                               QObject *object) = 0;
    virtual bool unregisterTimer(BobUI::TimerId timerId) = 0;
    virtual QList<TimerInfoV2> timersForObject(QObject *object) const = 0;
    virtual Duration remainingTime(BobUI::TimerId timerId) const = 0;
    virtual bool processEventsWithDeadline(QEventLoop::ProcessEventsFlags flags, QDeadlineTimer deadline); // reserved for 6.9

protected:
    QAbstractEventDispatcherV2(QAbstractEventDispatcherPrivate &, QObject *parent);

private:
BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_GCC("-Woverloaded-virtual")
BOBUI_WARNING_DISABLE_CLANG("-Woverloaded-virtual")
    // final overrides from V1
    void registerTimer(int timerId, qint64 interval, BobUI::TimerType timerType,
                       QObject *object) final;
    bool unregisterTimer(int timerId) final;
    QList<TimerInfo> registeredTimers(QObject *object) const final;
    int remainingTime(int timerId) final;
BOBUI_WARNING_POP
};
#else
using QAbstractEventDispatcherV2 = QAbstractEventDispatcher;
#endif // BobUI 7

BOBUI_END_NAMESPACE

#endif // QABSTRACTEVENTDISPATCHER_H
