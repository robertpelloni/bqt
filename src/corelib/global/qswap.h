// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUICORE_QSWAP_H
#define BOBUICORE_QSWAP_H

#include <BobUICore/bobuiconfigmacros.h>

#include <type_traits>
#include <utility>

#if 0
#pragma bobui_class(BobUISwap)
#pragma bobui_sync_stop_processing
#endif

BOBUI_BEGIN_NAMESPACE

template <typename T>
constexpr void qSwap(T &value1, T &value2)
    noexcept(std::is_nothrow_swappable_v<T>)
{
    using std::swap;
    swap(value1, value2);
}

// pure compile-time micro-optimization for our own headers, so not documented:
template <typename T>
constexpr inline void bobui_ptr_swap(T* &lhs, T* &rhs) noexcept
{
    T *tmp = lhs;
    lhs = rhs;
    rhs = tmp;
}

BOBUI_END_NAMESPACE

#endif // BOBUICORE_QSWAP_H
