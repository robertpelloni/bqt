// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QBENCHMARKMEASUREMENT_P_H
#define QBENCHMARKMEASUREMENT_P_H

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

#include <BobUITest/qbenchmark.h>
#include <BobUICore/qlist.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QBenchmarkMeasurerBase
{
public:
    struct Measurement
    {
        qreal value;
        BOBUIest::QBenchmarkMetric metric;
    };
    virtual ~QBenchmarkMeasurerBase() = default;
    virtual void start() = 0;
    virtual QList<Measurement> stop() = 0;
    virtual bool isMeasurementAccepted(Measurement m) = 0;
    virtual int adjustIterationCount(int suggestion) = 0;
    virtual int adjustMedianCount(int suggestion) = 0;
    virtual bool needsWarmupIteration() { return false; }
};

BOBUI_END_NAMESPACE

#endif // QBENCHMARKMEASUREMENT_P_H
