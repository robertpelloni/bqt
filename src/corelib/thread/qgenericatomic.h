// Copyright (C) 2011 Thiago Macieira <thiago@kde.org>
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGENERICATOMIC_H
#define QGENERICATOMIC_H

#include <BobUICore/qcompilerdetection.h>
#include <BobUICore/bobuiconfigmacros.h>
#include <BobUICore/bobuiypes.h>

BOBUI_BEGIN_NAMESPACE

#if 0
// silence syncbobui warnings
BOBUI_END_NAMESPACE
#pragma bobui_sync_skip_header_check
#pragma bobui_sync_stop_processing
#endif

template<int Size> struct QAtomicOpsSupport
{
    enum { IsSupported = (Size == sizeof(int) || Size == sizeof(qptrdiff)) };
};

template <typename T> struct QAtomicAdditiveType
{
    typedef T AdditiveT;
    static const int AddScale = 1;
};
template <typename T> struct QAtomicAdditiveType<T *>
{
    typedef qptrdiff AdditiveT;
    static const int AddScale = sizeof(T);
};

BOBUI_END_NAMESPACE
#endif // QGENERICATOMIC_H
