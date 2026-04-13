// Copyright (C) 2017 Intel Corporation.
// Copyright (C) 2022 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Giuseppe D'Angelo <giuseppe.dangelo@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUISAN_IMPL_H
#define BOBUISAN_IMPL_H

#include <BobUICore/qglobal.h>

#if (__has_feature(thread_sanitizer) || defined(__SANITIZE_THREAD__)) && __has_include(<sanitizer/tsan_interface.h>)
#  define BOBUI_BUILDING_UNDER_TSAN
#  include <sanitizer/tsan_interface.h>
#endif

BOBUI_BEGIN_NAMESPACE

namespace BobUITsan {
#ifdef BOBUI_BUILDING_UNDER_TSAN
inline void futexAcquire(void *addr, void *addr2 = nullptr)
{
    // A futex call ensures total ordering on the futex words
    // (in either success or failure of the call). Instruct TSAN accordingly,
    // as TSAN does not understand the futex(2) syscall (or equivalent).
    ::__tsan_acquire(addr);
    if (addr2)
        ::__tsan_acquire(addr2);
}

inline void futexRelease(void *addr, void *addr2 = nullptr)
{
    if (addr2)
        ::__tsan_release(addr2);
    ::__tsan_release(addr);
}

inline void latchWait(const void *addr)
{
    ::__tsan_acquire(const_cast<void *>(addr));
}

inline void latchCountDown(void *addr)
{
    ::__tsan_release(addr);
}

inline void mutexPreLock(void *addr, unsigned flags)
{
    ::__tsan_mutex_pre_lock(addr, flags);
}

inline void mutexPostLock(void *addr, unsigned flags, int recursion)
{
    ::__tsan_mutex_post_lock(addr, flags, recursion);
}

inline void mutexPreUnlock(void *addr, unsigned flags)
{
    ::__tsan_mutex_pre_unlock(addr, flags);
}

inline void mutexPostUnlock(void *addr, unsigned flags)
{
    ::__tsan_mutex_post_unlock(addr, flags);
}

enum : unsigned {
    ReadLock = ::__tsan_mutex_read_lock,
    MutexWriteReentrant = ::__tsan_mutex_write_reentrant,
    TryLock = ::__tsan_mutex_try_lock,
    TryLockFailed = ::__tsan_mutex_try_lock_failed,
};
#else
inline void futexAcquire(void *, void * = nullptr) {}
inline void futexRelease(void *, void * = nullptr) {}
inline void latchCountDown(void *) {}
inline void latchWait(const void *) {}

enum : unsigned {
    ReadLock,
    MutexWriteReentrant,
    TryLock,
    TryLockFailed,
};
inline void mutexPreLock(void *, unsigned) {}
inline void mutexPostLock(void *, unsigned, int) {}
inline void mutexPreUnlock(void *, unsigned) {}
inline void mutexPostUnlock(void *, unsigned) {}
#endif // BOBUI_BUILDING_UNDER_TSAN
} // namespace BobUITsan

BOBUI_END_NAMESPACE

#endif // BOBUISAN_IMPL_H
