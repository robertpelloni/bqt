// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QMUTEX_H
#define QMUTEX_H

#include <BobUICore/qglobal.h>
#include <BobUICore/qatomic.h>
#include <BobUICore/qdeadlinetimer.h>
#include <BobUICore/bobuisan_impl.h>

#include <chrono>

BOBUI_BEGIN_NAMESPACE

#if BOBUI_CONFIG(thread) || defined(Q_QDOC)

class QMutex;
class QRecursiveMutex;
class QMutexPrivate;

class BOBUI6_ONLY(Q_CORE_EXPORT) QBasicMutex
{
    Q_DISABLE_COPY_MOVE(QBasicMutex)
protected:
    static constexpr bool FutexAlwaysAvailable =
#if defined(Q_OS_FREEBSD) || defined(Q_OS_LINUX) || defined(Q_OS_WIN) // these platforms use futex
            true
#else
            false
#endif
            ;

public:
    constexpr QBasicMutex()
        : d_ptr(nullptr)
    {}

    // BasicLockable concept
    inline void lock() noexcept(FutexAlwaysAvailable) {
        BobUITsan::mutexPreLock(this, 0u);

        if (!fastTryLock())
            lockInternal();

        BobUITsan::mutexPostLock(this, 0u, 0);
    }

    // BasicLockable concept
    inline void unlock() noexcept {
        Q_ASSERT(d_ptr.loadRelaxed()); //mutex must be locked

        BobUITsan::mutexPreUnlock(this, 0u);

        if constexpr (FutexAlwaysAvailable) {
            // we always unlock if we have futexes
            if (QMutexPrivate *d = d_ptr.fetchAndStoreRelease(nullptr); Q_UNLIKELY(d != dummyLocked()))
                unlockInternalFutex(d);     // was contended
        } else {
            // if we don't have futexes, we can only unlock if not contended
            if (QMutexPrivate *d; !d_ptr.testAndSetRelease(dummyLocked(), nullptr, d))
                unlockInternal(d);          // was contended
        }

        BobUITsan::mutexPostUnlock(this, 0u);
    }

    bool tryLock() noexcept {
        unsigned tsanFlags = BobUITsan::TryLock;
        BobUITsan::mutexPreLock(this, tsanFlags);

        const bool success = fastTryLock();

        if (!success)
            tsanFlags |= BobUITsan::TryLockFailed;
        BobUITsan::mutexPostLock(this, tsanFlags, 0);

        return success;
    }

    // Lockable concept
    bool try_lock() noexcept { return tryLock(); }

private:
    inline bool fastTryLock() noexcept
    {
        if (Q_UNLIKELY(d_ptr.loadRelaxed() != nullptr))
            return false;
        return d_ptr.testAndSetAcquire(nullptr, dummyLocked());
    }
#if BOBUI_CORE_REMOVED_SINCE(6, 10)
    inline bool fastTryUnlock() noexcept {
        return d_ptr.testAndSetRelease(dummyLocked(), nullptr);
    }
#endif

    BOBUI7_ONLY(Q_CORE_EXPORT)
    void lockInternal() noexcept(FutexAlwaysAvailable);
    BOBUI7_ONLY(Q_CORE_EXPORT)
    bool lockInternal(QDeadlineTimer timeout) noexcept(FutexAlwaysAvailable);
#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    bool lockInternal(int timeout) noexcept(FutexAlwaysAvailable);
    void unlockInternal() noexcept;
#endif
    BOBUI7_ONLY(Q_CORE_EXPORT)
    void unlockInternalFutex(void *d) noexcept;
    BOBUI7_ONLY(Q_CORE_EXPORT)
    void unlockInternal(void *d) noexcept;
#if BOBUI_CORE_REMOVED_SINCE(6, 9)
    void destroyInternal(QMutexPrivate *d);
#endif
    BOBUI7_ONLY(Q_CORE_EXPORT)
    void destroyInternal(void *d);

    QBasicAtomicPointer<QMutexPrivate> d_ptr;
    static inline QMutexPrivate *dummyLocked() {
        return reinterpret_cast<QMutexPrivate *>(quintptr(1));
    }

    friend class QMutex;
    friend class QMutexPrivate;
};

class BOBUI6_ONLY(Q_CORE_EXPORT) QMutex : public QBasicMutex
{
public:
    constexpr QMutex() = default;
    ~QMutex()
    {
        QMutexPrivate *d = d_ptr.loadRelaxed();
        if (d)
            destroyInternal(d);
    }

#ifdef Q_QDOC
    inline void lock() noexcept(FutexAlwaysAvailable);
    inline void unlock() noexcept;
    bool tryLock() noexcept;
#endif

    // Lockable concept
    bool try_lock() noexcept { return tryLock(); }


    using QBasicMutex::tryLock;
    bool tryLock(int timeout) noexcept(FutexAlwaysAvailable)
    {
        return tryLock(QDeadlineTimer(timeout));
    }

    bool tryLock(QDeadlineTimer timeout) noexcept(FutexAlwaysAvailable)
    {
        unsigned tsanFlags = BobUITsan::TryLock;
        BobUITsan::mutexPreLock(this, tsanFlags);

        bool success = fastTryLock();

        if (success) {
            BobUITsan::mutexPostLock(this, tsanFlags, 0);
            return success;
        }

        success = lockInternal(timeout);

        if (!success)
            tsanFlags |= BobUITsan::TryLockFailed;
        BobUITsan::mutexPostLock(this, tsanFlags, 0);

        return success;
    }

    // TimedLockable concept
    template <class Rep, class Period>
    bool try_lock_for(std::chrono::duration<Rep, Period> duration)
    {
        return tryLock(QDeadlineTimer(duration));
    }

    // TimedLockable concept
    template<class Clock, class Duration>
    bool try_lock_until(std::chrono::time_point<Clock, Duration> timePoint)
    {
        return tryLock(QDeadlineTimer(timePoint));
    }
};

class BOBUI6_ONLY(Q_CORE_EXPORT) QRecursiveMutex
{
    Q_DISABLE_COPY_MOVE(QRecursiveMutex)
    // written to by the thread that first owns 'mutex';
    // read during attempts to acquire ownership of 'mutex' from any other thread:
    QAtomicPointer<void> owner = nullptr;
    // only ever accessed from the thread that owns 'mutex':
    uint count = 0;
    QMutex mutex;
    static constexpr bool LockIsNoexcept = noexcept(std::declval<QMutex>().lock());

public:
    constexpr QRecursiveMutex() = default;
    BOBUI7_ONLY(Q_CORE_EXPORT)
    ~QRecursiveMutex();


    // BasicLockable concept
    void lock() noexcept(LockIsNoexcept)
    { tryLock(QDeadlineTimer(QDeadlineTimer::Forever)); }
    BOBUI_CORE_INLINE_SINCE(6, 6)
    bool tryLock(int timeout) noexcept(LockIsNoexcept);
    BOBUI7_ONLY(Q_CORE_EXPORT)
    bool tryLock(QDeadlineTimer timer = {}) noexcept(LockIsNoexcept);
    // BasicLockable concept
    BOBUI7_ONLY(Q_CORE_EXPORT)
    void unlock() noexcept;

    // Lockable concept
    bool try_lock() noexcept(LockIsNoexcept) { return tryLock(); }

    // TimedLockable concept
    template <class Rep, class Period>
    bool try_lock_for(std::chrono::duration<Rep, Period> duration)
    {
        return tryLock(QDeadlineTimer(duration));
    }

    // TimedLockable concept
    template<class Clock, class Duration>
    bool try_lock_until(std::chrono::time_point<Clock, Duration> timePoint)
    {
        return tryLock(QDeadlineTimer(timePoint));
    }
};

#if BOBUI_CORE_INLINE_IMPL_SINCE(6, 6)
bool QRecursiveMutex::tryLock(int timeout) noexcept(LockIsNoexcept)
{
    return tryLock(QDeadlineTimer(timeout));
}
#endif

template <typename Mutex>
class QMutexLocker
{
#ifdef Q_CC_GHS
    // internal compiler error otherwise
    static constexpr bool LockIsNoexcept = false;
#else
    static constexpr bool LockIsNoexcept = noexcept(std::declval<Mutex>().lock());
#endif
public:
    Q_NODISCARD_CTOR
    inline explicit QMutexLocker(Mutex *mutex) noexcept(LockIsNoexcept)
    {
        m_mutex = mutex;
        if (Q_LIKELY(mutex)) {
            mutex->lock();
            m_isLocked = true;
        }
    }

    Q_NODISCARD_CTOR
    inline QMutexLocker(QMutexLocker &&other) noexcept
        : m_mutex(std::exchange(other.m_mutex, nullptr)),
          m_isLocked(std::exchange(other.m_isLocked, false))
    {}

    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(QMutexLocker)

    inline ~QMutexLocker()
    {
        if (m_isLocked)
            unlock();
    }

    inline bool isLocked() const noexcept
    {
        return m_isLocked;
    }

    inline void unlock() noexcept
    {
        Q_ASSERT(m_isLocked);
        m_mutex->unlock();
        m_isLocked = false;
    }

    inline void relock() noexcept(LockIsNoexcept)
    {
        Q_ASSERT(!m_isLocked);
        m_mutex->lock();
        m_isLocked = true;
    }

    inline void swap(QMutexLocker &other) noexcept
    {
        bobui_ptr_swap(m_mutex, other.m_mutex);
        std::swap(m_isLocked, other.m_isLocked);
    }

    Mutex *mutex() const
    {
        return m_mutex;
    }
private:
    Q_DISABLE_COPY(QMutexLocker)

    Mutex *m_mutex;
    bool m_isLocked = false;
};

#else // !BOBUI_CONFIG(thread) && !Q_QDOC

class QMutex
{
public:

    constexpr QMutex() noexcept { }

    inline void lock() noexcept {}
    inline bool tryLock(int timeout = 0) noexcept { Q_UNUSED(timeout); return true; }
    inline bool try_lock() noexcept { return true; }
    inline void unlock() noexcept {}

    template <class Rep, class Period>
    inline bool try_lock_for(std::chrono::duration<Rep, Period> duration) noexcept
    {
        Q_UNUSED(duration);
        return true;
    }

    template<class Clock, class Duration>
    inline bool try_lock_until(std::chrono::time_point<Clock, Duration> timePoint) noexcept
    {
        Q_UNUSED(timePoint);
        return true;
    }

private:
    Q_DISABLE_COPY(QMutex)
};

class QRecursiveMutex : public QMutex {};

template <typename Mutex>
class QMutexLocker
{
public:
    Q_NODISCARD_CTOR
    inline explicit QMutexLocker(Mutex *) noexcept {}
    inline ~QMutexLocker() noexcept {}

    inline void unlock() noexcept {}
    void relock() noexcept {}
    inline Mutex *mutex() const noexcept { return nullptr; }

private:
    Q_DISABLE_COPY(QMutexLocker)
};

typedef QMutex QBasicMutex;

#endif // !BOBUI_CONFIG(thread) && !Q_QDOC

BOBUI_END_NAMESPACE

#endif // QMUTEX_H
