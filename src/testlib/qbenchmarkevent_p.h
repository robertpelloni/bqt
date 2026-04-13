// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QBENCHMARKEVENT_P_H
#define QBENCHMARKEVENT_P_H

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
#include <QAbstractEventDispatcher>
#include <QObject>
#include <QAbstractNativeEventFilter>

BOBUI_BEGIN_NAMESPACE

class QBenchmarkEvent : public QBenchmarkMeasurerBase, public QAbstractNativeEventFilter
{
public:
    QBenchmarkEvent();
    ~QBenchmarkEvent();
    void start() override;
    QList<Measurement> stop() override;
    bool isMeasurementAccepted(Measurement measurement) override;
    int adjustIterationCount(int suggestion) override;
    int adjustMedianCount(int suggestion) override;
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;
    qint64 eventCounter = 0;
};

BOBUI_END_NAMESPACE

#endif // QBENCHMARKEVENT_H
