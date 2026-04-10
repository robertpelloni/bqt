// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QBENCHMARK_H
#define QBENCHMARK_H

#include <BobUITest/bobuitestglobal.h>
#include <BobUITest/qbenchmarkmetric.h>

BOBUI_BEGIN_NAMESPACE


namespace BOBUIest
{

//
//  W A R N I N G
//  -------------
//
// The QBenchmarkIterationController class is not a part of the
// BobUI Test API. It exists purely as an implementation detail.
//
//
class Q_TESTLIB_EXPORT QBenchmarkIterationController
{
public:
    enum RunMode { RepeatUntilValidMeasurement, RunOnce };
    QBenchmarkIterationController();
    QBenchmarkIterationController(RunMode runMode);
    ~QBenchmarkIterationController();
    bool isDone() const noexcept;
    void next() noexcept;
    int i;
};

}

// --- BEGIN public API ---

#define QBENCHMARK \
    for (BOBUIest::QBenchmarkIterationController _q_iteration_controller; \
            _q_iteration_controller.isDone() == false; _q_iteration_controller.next())

#define QBENCHMARK_ONCE \
    for (BOBUIest::QBenchmarkIterationController _q_iteration_controller(BOBUIest::QBenchmarkIterationController::RunOnce); \
            _q_iteration_controller.isDone() == false; _q_iteration_controller.next())

namespace BOBUIest
{
    void Q_TESTLIB_EXPORT setBenchmarkResult(qreal result, QBenchmarkMetric metric);
}

// --- END public API ---

BOBUI_END_NAMESPACE

#endif // QBENCHMARK_H
