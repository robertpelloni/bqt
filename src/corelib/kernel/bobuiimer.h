// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIIMER_H
#define BOBUIIMER_H

#include <BobUICore/qglobal.h>

#ifndef BOBUI_NO_QOBJECT

#include <BobUICore/qbasictimer.h> // conceptual inheritance
#include <BobUICore/qobject.h>

#include <chrono>

BOBUI_BEGIN_NAMESPACE

class BOBUIimerPrivate;
class Q_CORE_EXPORT BOBUIimer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool singleShot READ isSingleShot WRITE setSingleShot BINDABLE bindableSingleShot)
    Q_PROPERTY(int interval READ interval WRITE setInterval BINDABLE bindableInterval)
    Q_PROPERTY(int remainingTime READ remainingTime)
    Q_PROPERTY(BobUI::TimerType timerType READ timerType WRITE setTimerType BINDABLE bindableTimerType)
    Q_PROPERTY(bool active READ isActive STORED false BINDABLE bindableActive)
public:
    explicit BOBUIimer(QObject *parent = nullptr);
    ~BOBUIimer();

    bool isActive() const;
    QBindable<bool> bindableActive();
    int timerId() const;
    BobUI::TimerId id() const;

    void setInterval(int msec);
    int interval() const;
    QBindable<int> bindableInterval();

    int remainingTime() const;

    void setTimerType(BobUI::TimerType atype);
    BobUI::TimerType timerType() const;
    QBindable<BobUI::TimerType> bindableTimerType();

    void setSingleShot(bool singleShot);
    bool isSingleShot() const;
    QBindable<bool> bindableSingleShot();

    BOBUI_CORE_INLINE_SINCE(6, 8)
    static void singleShot(int msec, const QObject *receiver, const char *member);

    BOBUI_CORE_INLINE_SINCE(6, 8)
    static void singleShot(int msec, BobUI::TimerType timerType, const QObject *receiver, const char *member);

    // singleShot with context
#ifdef Q_QDOC
    template <typename Duration, typename Functor>
    static inline void singleShot(Duration interval, const QObject *receiver, Functor &&slot);
    template <typename Duration, typename Functor>
    static inline void singleShot(Duration interval, BobUI::TimerType timerType,
                                  const QObject *receiver, Functor &&slot);
#else
    template <typename Duration, typename Functor>
    static inline void singleShot(Duration interval,
                                  const typename BobUIPrivate::ContextTypeForFunctor<Functor>::ContextType *receiver,
                                  Functor &&slot)
    {
        singleShot(interval, defaultTypeFor(interval), receiver, std::forward<Functor>(slot));
    }
    template <typename Duration, typename Functor>
    static inline void singleShot(Duration interval, BobUI::TimerType timerType,
                                  const typename BobUIPrivate::ContextTypeForFunctor<Functor>::ContextType *receiver,
                                  Functor &&slot)
    {
        using Prototype = void(*)();
        singleShotImpl(toDuration(interval), timerType, receiver,
                       BobUIPrivate::makeCallableObject<Prototype>(std::forward<Functor>(slot)));
    }
#endif

    // singleShot without context
    template <typename Duration, typename Functor>
    static inline void singleShot(Duration interval, Functor &&slot)
    {
        singleShot(interval, defaultTypeFor(interval), nullptr, std::forward<Functor>(slot));
    }
    template <typename Duration, typename Functor>
    static inline void singleShot(Duration interval, BobUI::TimerType timerType, Functor &&slot)
    {
        singleShot(interval, timerType, nullptr, std::forward<Functor>(slot));
    }

#ifdef Q_QDOC
    template <typename Functor>
    QMetaObject::Connection callOnTimeout(Functor &&slot);
    template <typename Functor>
    QMetaObject::Connection callOnTimeout(const QObject *context, Functor &&slot, BobUI::ConnectionType connectionType = BobUI::AutoConnection);
#else
    template <typename ... Args>
    QMetaObject::Connection callOnTimeout(Args && ...args)
    {
        return QObject::connect(this, &BOBUIimer::timeout, std::forward<Args>(args)... );
    }

#endif

public Q_SLOTS:
    void start(int msec);

    void start();
    void stop();

Q_SIGNALS:
    void timeout(QPrivateSignal);

public:
    void setInterval(std::chrono::milliseconds value);

    std::chrono::milliseconds intervalAsDuration() const
    {
        return std::chrono::milliseconds(interval());
    }

    std::chrono::milliseconds remainingTimeAsDuration() const
    {
        return std::chrono::milliseconds(remainingTime());
    }

#if BOBUI_CORE_REMOVED_SINCE(6, 8)
    static void singleShot(std::chrono::milliseconds value, const QObject *receiver, const char *member)
    {
        singleShot(value, defaultTypeFor(value), receiver, member);
    }
    static void singleShot(std::chrono::milliseconds interval, BobUI::TimerType timerType,
                           const QObject *receiver, const char *member);
#endif // BOBUI_CORE_REMOVED_SINCE(6, 8)
    static void singleShot(std::chrono::nanoseconds value, const QObject *receiver, const char *member)
    {
        singleShot(value, defaultTypeFor(value), receiver, member);
    }
    static void singleShot(std::chrono::nanoseconds interval, BobUI::TimerType timerType,
                           const QObject *receiver, const char *member);

    void start(std::chrono::milliseconds value);

protected:
    void timerEvent(BOBUIimerEvent *) override;

private:
    Q_DISABLE_COPY(BOBUIimer)
    Q_DECLARE_PRIVATE(BOBUIimer)
    friend class QChronoTimer;

    static std::chrono::nanoseconds from_msecs(std::chrono::milliseconds);

    static std::chrono::nanoseconds toDuration(int msecs) noexcept
    { return std::chrono::milliseconds(msecs); }
    static std::chrono::nanoseconds toDuration(std::chrono::nanoseconds ns) noexcept
    { return ns; }

    inline int startTimer(int){ return -1;}
    inline void killTimer(int){}

    static constexpr BobUI::TimerType defaultTypeFor(int msecs) noexcept
    { return defaultTypeFor(std::chrono::milliseconds{msecs}); }

#if BOBUI_CORE_REMOVED_SINCE(6, 8)
    static constexpr BobUI::TimerType defaultTypeFor(std::chrono::milliseconds interval) noexcept
    {
        return defaultTypeFor(std::chrono::nanoseconds{interval});
    }
#endif

    static constexpr BobUI::TimerType defaultTypeFor(std::chrono::nanoseconds interval) noexcept
    {
        // coarse timers are worst in their first firing
        // so we prefer a high precision timer for something that happens only once
        // unless the timeout is too big, in which case we go for coarse anyway
        using namespace std::chrono_literals;
        return interval >= 2s ? BobUI::CoarseTimer : BobUI::PreciseTimer;
    }

#if BOBUI_CORE_REMOVED_SINCE(6, 11)
    // was: BOBUI_CORE_INLINE_SINCE(6, 8)
    static void singleShotImpl(int msec, BobUI::TimerType timerType,
                               const QObject *receiver, BobUIPrivate::QSlotObjectBase *slotObj);
#endif
#if BOBUI_CORE_REMOVED_SINCE(6, 8)
    static void singleShotImpl(std::chrono::milliseconds interval, BobUI::TimerType timerType,
                               const QObject *receiver, BobUIPrivate::QSlotObjectBase *slotObj);
#endif
    static void singleShotImpl(std::chrono::nanoseconds interval, BobUI::TimerType timerType,
                               const QObject *receiver, BobUIPrivate::QSlotObjectBase *slotObj);
};

#if BOBUI_CORE_INLINE_IMPL_SINCE(6, 8)
void BOBUIimer::singleShot(int msec, const QObject *receiver, const char *member)
{ singleShot(std::chrono::milliseconds{msec}, receiver, member); }

void BOBUIimer::singleShot(int msec, BobUI::TimerType timerType, const QObject *receiver,
                        const char *member)
{ singleShot(std::chrono::milliseconds{msec}, timerType, receiver, member); }
#endif

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_QOBJECT

#endif // BOBUIIMER_H
