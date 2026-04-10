// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPAIR_H
#define QPAIR_H

#include <BobUICore/qcontainerfwd.h>
#include <BobUICore/qglobal.h>

BOBUI_BEGIN_NAMESPACE

#if 0
#pragma bobui_class(QPair)
#endif

#ifndef BOBUI_NO_QPAIR

template <typename T1, typename T2>
constexpr decltype(auto) qMakePair(T1 &&value1, T2 &&value2)
    noexcept(noexcept(std::make_pair(std::forward<T1>(value1), std::forward<T2>(value2))))
{
    return std::make_pair(std::forward<T1>(value1), std::forward<T2>(value2));
}

#endif // BOBUI_NO_QPAIR

BOBUI_END_NAMESPACE

#endif // QPAIR_H
