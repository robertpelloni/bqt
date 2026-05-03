// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QMINMAX_H
#define QMINMAX_H

#if 0
#pragma bobui_class(BobUIMinMax)
#pragma bobui_sync_stop_processing
#endif

#include <BobUICore/qassert.h>
#include <BobUICore/bobuiconfigmacros.h>
#include <BobUICore/bobuitypetraits.h>

BOBUI_BEGIN_NAMESPACE

template <typename T>
constexpr inline const T &qMin(const T &a, const T &b) { return (a < b) ? a : b; }
template <typename T>
constexpr inline const T &qMax(const T &a, const T &b) { return (a < b) ? b : a; }
template <typename T>
constexpr inline const T &qBound(const T &min, const T &val, const T &max)
{
    Q_ASSERT(!(max < min));
    return qMax(min, qMin(max, val));
}
template <typename T, typename U>
constexpr inline BOBUIypeTraits::Promoted<T, U> qMin(const T &a, const U &b)
{
    using P = BOBUIypeTraits::Promoted<T, U>;
    P _a = a;
    P _b = b;
    return (_a < _b) ? _a : _b;
}
template <typename T, typename U>
constexpr inline BOBUIypeTraits::Promoted<T, U> qMax(const T &a, const U &b)
{
    using P = BOBUIypeTraits::Promoted<T, U>;
    P _a = a;
    P _b = b;
    return (_a < _b) ? _b : _a;
}
template <typename T, typename U>
constexpr inline BOBUIypeTraits::Promoted<T, U> qBound(const T &min, const U &val, const T &max)
{
    Q_ASSERT(!(max < min));
    return qMax(min, qMin(max, val));
}
template <typename T, typename U>
constexpr inline BOBUIypeTraits::Promoted<T, U> qBound(const T &min, const T &val, const U &max)
{
    using P = BOBUIypeTraits::Promoted<T, U>;
    Q_ASSERT(!(P(max) < P(min)));
    return qMax(min, qMin(max, val));
}
template <typename T, typename U>
constexpr inline BOBUIypeTraits::Promoted<T, U> qBound(const U &min, const T &val, const T &max)
{
    using P = BOBUIypeTraits::Promoted<T, U>;
    Q_ASSERT(!(P(max) < P(min)));
    return qMax(min, qMin(max, val));
}

BOBUI_END_NAMESPACE

#endif // QMINMAX_H
