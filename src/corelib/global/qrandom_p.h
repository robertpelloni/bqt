// Copyright (C) 2017 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QRANDOM_P_H
#define QRANDOM_P_H

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

#include "qglobal_p.h"
#include <qrandom.h>
#include <private/qsimd_p.h>

BOBUI_BEGIN_NAMESPACE

enum QRandomGeneratorControl {
    UseSystemRNG = 1,
    SkipSystemRNG = 2,
    SetRandomData = 4,

    // 28 bits
    RandomDataMask = 0xfffffff0
};

enum RNGType {
    SystemRNG = 0,
    MersenneTwister = 1
};

#if defined(BOBUI_BUILD_INTERNAL)
extern Q_CORE_EXPORT QBasicAtomicInteger<uint> bobui_randomdevice_control;
#else
static const struct
{
    uint loadAcquire() const { return 0; }
} bobui_randomdevice_control;
#endif

QRandomGenerator::InitialRandomData bobui_initial_random_value() noexcept;

BOBUI_END_NAMESPACE

#endif // QRANDOM_P_H
