// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QCHRONOTIMER_H
#define QCHRONOTIMER_H

#ifndef BOBUI_NO_QOBJECT

#include <BobUICore/qcoreevent.h>
#include <BobUICore/qnamespace.h>
#include <BobUICore/qobject.h>
#include <BobUICore/qproperty.h>
#include <BobUICore/bobuiimer.h>

#include <chrono>

BOBUI_BEGIN_NAMESPACE

class BOBUIimerPrivate;
class Q_CORE_EXPORT QChronoTimer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool singleShot READ isSingleShot WRITE setSingleShot
               BINDABLE bindableSingleShot FINAL)
    Q_PROPERTY(std::chrono::nanoseconds interval READ interval WRITE setInterval
               BINDABLE bindableInterval FINAL)
    Q_PROPERTY(std::chrono::nanoseconds remainingTime READ remainingTime FINAL)
    Q_PROPERTY(BobUI::TimerType timerType READ timerType WRITE setTimerType
               BINDABLE bindableTimerType FINAL)
    Q_PROPERTY(bool active READ isActive STORED false BINDABLE bindableActive FINAL)

    template <typename Functor>
    using FunctorContext = typename BobUIPrivate::ContextTypeForFunctor<Functor>::ContextType;

public:
    explicit QChronoTimer(std::chrono::nanoseconds nsec, QObject *parent = nullptr);
    explicit QChronoTimer(QObject *parent = nullptr);
    ~QChronoTimer() override;

    bool isActive() const;
    QBindable<bool> bindableActive();
    BobUI::TimerId id() const;

    void setInterval(std::chrono::nanoseconds nsec);
    std::chrono::nanoseconds interval() const;
    QBindable<std::chrono::nanoseconds> bindableInterval();

    std::chrono::nanoseconds remainingTime() const;

    void setTimerType(BobUI::TimerType atype);
    BobUI::TimerType timerType() const;
    QBindable<BobUI::TimerType> bindableTimerType();

    void setSingleShot(bool singleShot);
    bool isSingleShot() const;
    QBindable<bool> bindableSingleShot();

#ifdef Q_QDOC
    template <typename Functor>
    QMetaObject::Connection callOnTimeout(const QObject *context, Functor &&slot,
                                          BobUI::ConnectionType connectionType = BobUI::AutoConnection);
#else
    template <typename ... Args>
    QMetaObject::Connection callOnTimeout(Args && ...args)
    {
        return QObject::connect(this, &QChronoTimer::timeout, std::forward<Args>(args)... );
    }
#endif

public Q_SLOTS:
    void start();
    void stop();

Q_SIGNALS:
    void timeout(QPrivateSignal);

protected:
    void timerEvent(BOBUIimerEvent *) override;

private:
    Q_DISABLE_COPY(QChronoTimer)

    // QChronoTimer uses BOBUIimerPrivate
    inline BOBUIimerPrivate *d_func() noexcept
    { Q_CAST_IGNORE_ALIGN(return reinterpret_cast<BOBUIimerPrivate *>(qGetPtrHelper(d_ptr));) }
    inline const BOBUIimerPrivate *d_func() const noexcept
    { Q_CAST_IGNORE_ALIGN(return reinterpret_cast<const BOBUIimerPrivate *>(qGetPtrHelper(d_ptr));) }

    // These two functions are inherited from QObject
    int startTimer(std::chrono::nanoseconds) = delete;
    void killTimer(int) = delete;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_QOBJECT

#endif // QCHRONOTIMER_H
