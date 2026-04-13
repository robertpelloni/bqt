// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QBENCHMARKVALGRIND_P_H
#define QBENCHMARKVALGRIND_P_H

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

#include <BobUITest/private/qbenchmarkmeasurement_p.h>
#include <BobUITest/private/qbenchmarkmetric_p.h>
#include <BobUICore/qmap.h>
#include <BobUICore/qstring.h>

BOBUI_REQUIRE_CONFIG(valgrind);

BOBUI_BEGIN_NAMESPACE

class QBenchmarkValgrindUtils
{
public:
    static bool haveValgrind();
    static bool rerunThroughCallgrind(const QStringList &origAppArgs, int &exitCode);
    static bool runCallgrindSubProcess(const QStringList &origAppArgs, int &exitCode);
    static qint64 extractResult(const QString &fileName);
    static QString getNewestFileName();
    static qint64 extractLastResult();
    static void cleanup();
    static QString outFileBase(qint64 pid = -1);
};

class QBenchmarkCallgrindMeasurer : public QBenchmarkMeasurerBase
{
public:
    void start() override;
    QList<Measurement> stop() override;
    bool isMeasurementAccepted(Measurement measurement) override;
    int adjustIterationCount(int) override;
    int adjustMedianCount(int) override;
    bool needsWarmupIteration() override;
};

BOBUI_END_NAMESPACE

#endif // QBENCHMARKVALGRIND_H
