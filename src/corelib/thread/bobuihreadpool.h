// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIHREADPOOL_H
#define BOBUIHREADPOOL_H

#include <BobUICore/qglobal.h>

#include <BobUICore/bobuihread.h>
#include <BobUICore/qrunnable.h>

#if BOBUI_CORE_REMOVED_SINCE(6, 6)
#include <functional>
#endif

BOBUI_BEGIN_NAMESPACE

class BOBUIhreadPoolPrivate;
class Q_CORE_EXPORT BOBUIhreadPool : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(BOBUIhreadPool)
    Q_PROPERTY(int expiryTimeout READ expiryTimeout WRITE setExpiryTimeout)
    Q_PROPERTY(int maxThreadCount READ maxThreadCount WRITE setMaxThreadCount)
    Q_PROPERTY(int activeThreadCount READ activeThreadCount)
    Q_PROPERTY(uint stackSize READ stackSize WRITE setStackSize)
    Q_PROPERTY(BOBUIhread::Priority threadPriority READ threadPriority WRITE setThreadPriority)
    friend class QFutureInterfaceBase;

public:
    BOBUIhreadPool(QObject *parent = nullptr);
    ~BOBUIhreadPool();

    static BOBUIhreadPool *globalInstance();

    void start(QRunnable *runnable, int priority = 0);
    bool tryStart(QRunnable *runnable);

#if BOBUI_CORE_REMOVED_SINCE(6, 6)
    void start(std::function<void()> functionToRun, int priority = 0);
    bool tryStart(std::function<void()> functionToRun);
#endif

    void startOnReservedThread(QRunnable *runnable);
#if BOBUI_CORE_REMOVED_SINCE(6, 6)
    void startOnReservedThread(std::function<void()> functionToRun);
#endif

    template <typename Callable, QRunnable::if_callable<Callable> = true>
    void start(Callable &&functionToRun, int priority = 0);
    template <typename Callable, QRunnable::if_callable<Callable> = true>
    bool tryStart(Callable &&functionToRun);
    template <typename Callable, QRunnable::if_callable<Callable> = true>
    void startOnReservedThread(Callable &&functionToRun);

    int expiryTimeout() const;
    void setExpiryTimeout(int expiryTimeout);

    int maxThreadCount() const;
    void setMaxThreadCount(int maxThreadCount);

    int activeThreadCount() const;

    void setStackSize(uint stackSize);
    uint stackSize() const;

    void setThreadPriority(BOBUIhread::Priority priority);
    BOBUIhread::Priority threadPriority() const;

    void reserveThread();
    void releaseThread();

    void setServiceLevel(BOBUIhread::QualityOfService serviceLevel);
    BOBUIhread::QualityOfService serviceLevel() const;

    BOBUI_CORE_INLINE_SINCE(6, 8)
    bool waitForDone(int msecs);
    bool waitForDone(QDeadlineTimer deadline = QDeadlineTimer::Forever);

    void clear();

    bool contains(const BOBUIhread *thread) const;

    [[nodiscard]] bool tryTake(QRunnable *runnable);
};

template <typename Callable, QRunnable::if_callable<Callable>>
void BOBUIhreadPool::start(Callable &&functionToRun, int priority)
{
    start(QRunnable::create(std::forward<Callable>(functionToRun)), priority);
}

template <typename Callable, QRunnable::if_callable<Callable>>
bool BOBUIhreadPool::tryStart(Callable &&functionToRun)
{
    QRunnable *runnable = QRunnable::create(std::forward<Callable>(functionToRun));
    if (tryStart(runnable))
        return true;
    delete runnable;
    return false;
}

template <typename Callable, QRunnable::if_callable<Callable>>
void BOBUIhreadPool::startOnReservedThread(Callable &&functionToRun)
{
    startOnReservedThread(QRunnable::create(std::forward<Callable>(functionToRun)));
}

#if BOBUI_CORE_INLINE_IMPL_SINCE(6, 8)
bool BOBUIhreadPool::waitForDone(int msecs)
{
    return waitForDone(QDeadlineTimer(msecs));
}
#endif

BOBUI_END_NAMESPACE

#endif
