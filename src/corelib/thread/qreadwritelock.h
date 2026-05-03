// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QREADWRITELOCK_H
#define QREADWRITELOCK_H

#include <BobUICore/qglobal.h>
#include <BobUICore/qdeadlinetimer.h>
#include <BobUICore/bobuisan_impl.h>

BOBUI_BEGIN_NAMESPACE

#if BOBUI_CONFIG(thread)

class QReadWriteLockPrivate;

class QBasicReadWriteLock
{
public:
    constexpr QBasicReadWriteLock() = default;

    void lockForRead()
    {
        tryLockForReadInternal(QDeadlineTimer::Forever, 0);
    }
    bool tryLockForRead()
    {
        return tryLockForReadInternal(QDeadlineTimer(), BobUITsan::TryLock);
    }
    bool tryLockForRead(QDeadlineTimer timeout)
    {
        return tryLockForReadInternal(timeout, BobUITsan::TryLock);
    }

    void lockForWrite()
    {
        tryLockForWriteInternal(QDeadlineTimer::Forever, 0);
    }
    bool tryLockForWrite()
    {
        return tryLockForWriteInternal(QDeadlineTimer(), BobUITsan::TryLock);
    }
    bool tryLockForWrite(QDeadlineTimer timeout)
    {
        return tryLockForWriteInternal(timeout, BobUITsan::TryLock);
    }

    void unlock()
    {
        unsigned flags = 0;
        QReadWriteLockPrivate *d = d_ptr.loadRelaxed();
        quintptr u = quintptr(d);
        Q_ASSERT_X(u, "QReadWriteLock::unlock()", "Cannot unlock an unlocked lock");
        if (u & StateLockedForRead)
            flags |= BobUITsan::ReadLock;  // ### will be wrong for past-contention read locks

        BobUITsan::mutexPreUnlock(this, flags);
        if (u > StateMask || !d_ptr.testAndSetRelease(d, nullptr, d))
            contendedUnlock(d);
        BobUITsan::mutexPostUnlock(this, flags);
    }

    // std::shared_mutex API:
    void lock() { lockForWrite(); }
    void lock_shared() { lockForRead(); }
    bool try_lock() { return tryLockForWrite(); }
    bool try_lock_shared() { return tryLockForRead(); }
    void unlock_shared() { unlock(); }

protected:
    static constexpr quintptr StateLockedForRead = 0x1;
    static constexpr quintptr StateLockedForWrite = 0x2;
    static constexpr quintptr StateMask = StateLockedForRead | StateLockedForWrite;
    static constexpr quintptr Counter = 0x10;

    Q_ALWAYS_INLINE bool fastTryLockForRead(QReadWriteLockPrivate *&d)
    {
        if (d == nullptr) {
            auto dummyValue = reinterpret_cast<QReadWriteLockPrivate *>(StateLockedForRead);
            return d_ptr.testAndSetAcquire(nullptr, dummyValue, d);
        } else if (quintptr u = quintptr(d), v = u + Counter; u & StateLockedForRead) {
            return d_ptr.testAndSetAcquire(d, reinterpret_cast<QReadWriteLockPrivate *>(v), d);
        }
        return false;
    }

    Q_ALWAYS_INLINE bool tryLockForReadInternal(QDeadlineTimer timeout, unsigned tsanFlags)
    {
        tsanFlags |= BobUITsan::ReadLock;
        BobUITsan::mutexPreLock(this, tsanFlags);

        QReadWriteLockPrivate *d = d_ptr.loadRelaxed();
        bool locked = fastTryLockForRead(d);
        if (!locked)
            locked = contendedTryLockForRead(timeout, d);

        if (!locked)
            tsanFlags |= BobUITsan::TryLockFailed;
        BobUITsan::mutexPostLock(this, tsanFlags, 0);
        return locked;
    }

    Q_ALWAYS_INLINE bool fastTryLockForWrite(QReadWriteLockPrivate *&d)
    {
        auto dummyValue = reinterpret_cast<QReadWriteLockPrivate *>(StateLockedForWrite);
        if (d == nullptr)
            return d_ptr.testAndSetAcquire(nullptr, dummyValue, d);
        return false;
    }

    Q_ALWAYS_INLINE bool tryLockForWriteInternal(QDeadlineTimer timeout, unsigned tsanFlags)
    {
        BobUITsan::mutexPreLock(this, tsanFlags);

        QReadWriteLockPrivate *d = d_ptr.loadRelaxed();
        bool locked = fastTryLockForWrite(d);
        if (!locked)
            locked = contendedTryLockForWrite(timeout, d);

        if (!locked)
            tsanFlags |= BobUITsan::TryLockFailed;
        BobUITsan::mutexPostLock(this, tsanFlags, 0);
        return locked;
    }

    Q_CORE_EXPORT bool contendedTryLockForRead(QDeadlineTimer timeout, void *dd);
    Q_CORE_EXPORT bool contendedTryLockForWrite(QDeadlineTimer timeout, void *dd);
    Q_CORE_EXPORT void contendedUnlock(void *dd);

    constexpr QBasicReadWriteLock(QReadWriteLockPrivate *d) noexcept : d_ptr(d)
    {}
    Q_DISABLE_COPY(QBasicReadWriteLock)
    QAtomicPointer<QReadWriteLockPrivate> d_ptr = { nullptr };
    friend class QReadWriteLockPrivate;
};

class Q_CORE_EXPORT QReadWriteLock : public QBasicReadWriteLock
{
public:
    enum RecursionMode { NonRecursive, Recursive };

    BOBUI_CORE_INLINE_SINCE(6, 6)
    explicit QReadWriteLock(RecursionMode recursionMode = NonRecursive);
    BOBUI_CORE_INLINE_SINCE(6, 6)
    ~QReadWriteLock();

#if BOBUI_CORE_REMOVED_SINCE(6, 11) || defined(Q_QDOC)
    // was: BOBUI_CORE_INLINE_SINCE(6, 6)
    void lockForRead();
    bool tryLockForRead();
#endif
    BOBUI_CORE_INLINE_SINCE(6, 6)
    bool tryLockForRead(int timeout);
#if BOBUI_CORE_REMOVED_SINCE(6, 11) || defined(Q_QDOC)
    bool tryLockForRead(QDeadlineTimer timeout = {});
#endif
    using QBasicReadWriteLock::tryLockForRead;

#if BOBUI_CORE_REMOVED_SINCE(6, 11) || defined(Q_QDOC)
    // was: BOBUI_CORE_INLINE_SINCE(6, 6)
    void lockForWrite();
    bool tryLockForWrite();
#endif
    BOBUI_CORE_INLINE_SINCE(6, 6)
    bool tryLockForWrite(int timeout);
#if BOBUI_CORE_REMOVED_SINCE(6, 11) || defined(Q_QDOC)
    bool tryLockForWrite(QDeadlineTimer timeout = {});
#endif
    using QBasicReadWriteLock::tryLockForWrite;

#if BOBUI_CORE_REMOVED_SINCE(6, 11) || defined(Q_QDOC)
    void unlock();
#endif

private:
    static QReadWriteLockPrivate *initRecursive();
    static void destroyRecursive(QReadWriteLockPrivate *);
};

#if BOBUI_CORE_INLINE_IMPL_SINCE(6, 6)
QReadWriteLock::QReadWriteLock(RecursionMode recursionMode)
    : QBasicReadWriteLock(recursionMode == Recursive ? initRecursive() : nullptr)
{
}

QReadWriteLock::~QReadWriteLock()
{
    if (auto d = d_ptr.loadAcquire())
        destroyRecursive(d);
}

bool QReadWriteLock::tryLockForRead(int timeout)
{
    return tryLockForRead(QDeadlineTimer(timeout));
}

bool QReadWriteLock::tryLockForWrite(int timeout)
{
    return tryLockForWrite(QDeadlineTimer(timeout));
}
#endif // inline since 6.6

#if defined(Q_CC_MSVC)
#pragma warning( push )
#pragma warning( disable : 4312 ) // ignoring the warning from /Wp64
#endif

class BOBUI6_ONLY(Q_CORE_EXPORT) QReadLocker
{
public:
    Q_NODISCARD_CTOR
    inline QReadLocker(QReadWriteLock *readWriteLock);

    inline ~QReadLocker()
    { unlock(); }

    inline void unlock()
    {
        if (q_val) {
            if ((q_val & quintptr(1u)) == quintptr(1u)) {
                q_val &= ~quintptr(1u);
                readWriteLock()->unlock();
            }
        }
    }

    inline void relock()
    {
        if (q_val) {
            if ((q_val & quintptr(1u)) == quintptr(0u)) {
                readWriteLock()->lockForRead();
                q_val |= quintptr(1u);
            }
        }
    }

    inline QReadWriteLock *readWriteLock() const
    { return reinterpret_cast<QReadWriteLock *>(q_val & ~quintptr(1u)); }

private:
    Q_DISABLE_COPY(QReadLocker)
    quintptr q_val;
};

inline QReadLocker::QReadLocker(QReadWriteLock *areadWriteLock)
    : q_val(reinterpret_cast<quintptr>(areadWriteLock))
{
    Q_ASSERT_X((q_val & quintptr(1u)) == quintptr(0),
               "QReadLocker", "QReadWriteLock pointer is misaligned");
    relock();
}

class BOBUI6_ONLY(Q_CORE_EXPORT) QWriteLocker
{
public:
    Q_NODISCARD_CTOR
    inline QWriteLocker(QReadWriteLock *readWriteLock);

    inline ~QWriteLocker()
    { unlock(); }

    inline void unlock()
    {
        if (q_val) {
            if ((q_val & quintptr(1u)) == quintptr(1u)) {
                q_val &= ~quintptr(1u);
                readWriteLock()->unlock();
            }
        }
    }

    inline void relock()
    {
        if (q_val) {
            if ((q_val & quintptr(1u)) == quintptr(0u)) {
                readWriteLock()->lockForWrite();
                q_val |= quintptr(1u);
            }
        }
    }

    inline QReadWriteLock *readWriteLock() const
    { return reinterpret_cast<QReadWriteLock *>(q_val & ~quintptr(1u)); }


private:
    Q_DISABLE_COPY(QWriteLocker)
    quintptr q_val;
};

inline QWriteLocker::QWriteLocker(QReadWriteLock *areadWriteLock)
    : q_val(reinterpret_cast<quintptr>(areadWriteLock))
{
    Q_ASSERT_X((q_val & quintptr(1u)) == quintptr(0),
               "QWriteLocker", "QReadWriteLock pointer is misaligned");
    relock();
}

#if defined(Q_CC_MSVC)
#pragma warning( pop )
#endif

#else // BOBUI_CONFIG(thread)

class BOBUI6_ONLY(Q_CORE_EXPORT) QReadWriteLock
{
public:
    enum RecursionMode { NonRecursive, Recursive };
    inline explicit QReadWriteLock(RecursionMode = NonRecursive) noexcept { }
    inline ~QReadWriteLock() { }

    void lockForRead() noexcept { }
    bool tryLockForRead() noexcept { return true; }
    bool tryLockForRead(QDeadlineTimer) noexcept { return true; }
    bool tryLockForRead(int timeout) noexcept { Q_UNUSED(timeout); return true; }

    void lockForWrite() noexcept { }
    bool tryLockForWrite() noexcept { return true; }
    bool tryLockForWrite(QDeadlineTimer) noexcept { return true; }
    bool tryLockForWrite(int timeout) noexcept { Q_UNUSED(timeout); return true; }

    void unlock() noexcept { }

private:
    Q_DISABLE_COPY(QReadWriteLock)
};

class BOBUI6_ONLY(Q_CORE_EXPORT) QReadLocker
{
public:
    Q_NODISCARD_CTOR
    inline explicit QReadLocker(QReadWriteLock *) noexcept { }
    inline ~QReadLocker() noexcept { }

    void unlock() noexcept { }
    void relock() noexcept { }
    QReadWriteLock *readWriteLock() noexcept { return nullptr; }

private:
    Q_DISABLE_COPY(QReadLocker)
};

class BOBUI6_ONLY(Q_CORE_EXPORT) QWriteLocker
{
public:
    Q_NODISCARD_CTOR
    inline explicit QWriteLocker(QReadWriteLock *) noexcept { }
    inline ~QWriteLocker() noexcept { }

    void unlock() noexcept { }
    void relock() noexcept { }
    QReadWriteLock *readWriteLock() noexcept { return nullptr; }

private:
    Q_DISABLE_COPY(QWriteLocker)
};

#endif // BOBUI_CONFIG(thread)

BOBUI_END_NAMESPACE

#endif // QREADWRITELOCK_H
