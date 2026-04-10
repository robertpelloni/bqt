// Copyright (C) 2025 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QATOMICWAIT_P_H
#define QATOMICWAIT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an implementation
// detail. This header file may change from version to version without notice,
// or even be removed.
//
// We mean it.
//

#include "qbasicatomic.h"
#include <private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIPrivate {
enum class WakeMode { One = 1, All = -1 };

#ifdef QATOMICWAIT_USE_FALLBACK
#  define Q_ATOMICWAIT_EXPORT /* being linked into the unit test */
#else
#  define Q_ATOMICWAIT_EXPORT Q_CORE_EXPORT
#endif

Q_ATOMICWAIT_EXPORT void _q_atomicWait(const void *address, const void *old, qsizetype size) noexcept;
Q_ATOMICWAIT_EXPORT void _q_atomicWake(void *address, WakeMode) noexcept;

#undef Q_ATOMICWAIT_EXPORT
}

namespace BobUIFallbackAtomicWait {
// C++20- & C23-like API
template <typename T> std::enable_if_t<std::has_unique_object_representations_v<T>>
atomic_wait_explicit(const std::atomic<T> *atomic, T old, std::memory_order mo) noexcept
{
    Q_ASSERT(mo == std::memory_order_relaxed || mo == std::memory_order_acquire
             || mo == std::memory_order_seq_cst);
    BobUIPrivate::_q_atomicWait(atomic, &old, sizeof(T));
    std::atomic_thread_fence(mo);
}

template <typename T> std::enable_if_t<std::has_unique_object_representations_v<T>>
atomic_wait(const std::atomic<T> *atomic, T old) noexcept
{
    atomic_wait_explicit(atomic, old, std::memory_order_seq_cst);
}

template <typename T> std::enable_if_t<std::has_unique_object_representations_v<T>>
atomic_notify_one(std::atomic<T> *atomic)
{
    BobUIPrivate::_q_atomicWake(atomic, BobUIPrivate::WakeMode::One);
}

template <typename T> std::enable_if_t<std::has_unique_object_representations_v<T>>
atomic_notify_all(std::atomic<T> *atomic)
{
    BobUIPrivate::_q_atomicWake(atomic, BobUIPrivate::WakeMode::All);
}
} // namespace BobUIFallbackAtomicWait

namespace q20 {
#ifdef __cpp_lib_atomic_wait
using std::atomic_wait;
using std::atomic_wait_explicit;
using std::atomic_notify_all;
using std::atomic_notify_one;
#else
using BobUIFallbackAtomicWait::atomic_wait;
using BobUIFallbackAtomicWait::atomic_wait_explicit;
using BobUIFallbackAtomicWait::atomic_notify_all;
using BobUIFallbackAtomicWait::atomic_notify_one;
#endif
} // namespace BobUINativeAtomicWait

BOBUI_END_NAMESPACE

#endif // QATOMICWAIT_P_H
