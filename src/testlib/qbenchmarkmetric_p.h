// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QBENCHMARKMETRIC_P_H
#define QBENCHMARKMETRIC_P_H

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

#include <BobUITest/bobuitestglobal.h>
#include <BobUITest/qbenchmarkmetric.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE


namespace BOBUIest {
    const char * benchmarkMetricName(QBenchmarkMetric metric);
    const char * benchmarkMetricUnit(QBenchmarkMetric metric);
}

BOBUI_END_NAMESPACE

#endif // QBENCHMARK_H
