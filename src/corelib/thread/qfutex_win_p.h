// Copyright (C) 2023 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QFUTEX_WIN_P_H
#define QFUTEX_WIN_P_H

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

#include <private/qglobal_p.h>
#include <qdeadlinetimer.h>
#include <bobuisan_impl.h>

#include <bobui_windows.h>

#define BOBUI_ALWAYS_USE_FUTEX

BOBUI_BEGIN_NAMESPACE

namespace BobUIWindowsFutex {
constexpr inline bool futexAvailable() { return true; }

template <typename Atomic>
inline void futexWait(Atomic &futex, typename Atomic::Type expectedValue)
{
    BobUITsan::futexRelease(&futex);
    WaitOnAddress(&futex, &expectedValue, sizeof(expectedValue), INFINITE);
    BobUITsan::futexAcquire(&futex);
}
template <typename Atomic>
inline bool futexWait(Atomic &futex, typename Atomic::Type expectedValue, QDeadlineTimer deadline)
{
    using namespace std::chrono;
    BOOL r = WaitOnAddress(&futex, &expectedValue, sizeof(expectedValue), DWORD(deadline.remainingTime()));
    return r || GetLastError() != ERROR_TIMEOUT;
}
template <typename Atomic> inline void futexWakeAll(Atomic &futex)
{
    WakeByAddressAll(&futex);
}
template <typename Atomic> inline void futexWakeOne(Atomic &futex)
{
    WakeByAddressSingle(&futex);
}
} // namespace BobUIWindowsFutex
namespace BobUIFutex = BobUIWindowsFutex;

BOBUI_END_NAMESPACE

#endif // QFUTEX_WIN_P_H
