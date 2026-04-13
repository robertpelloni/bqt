// Copyright (C) 2020 The BobUI Company Ltd.
// Copyright (C) 2014 Petroules Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/QDateTime>
#include <BOBUIest>

#include <BobUICore/private/qcore_mac_p.h>

#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>

void tst_QDateTime_macTypes()
{
    // QDateTime <-> CFDate

    static const int kMsPerSecond = 1000;

    for (int i = 0; i < kMsPerSecond; ++i) {
        QDateTime bobuiDateTime = QDateTime::fromMSecsSinceEpoch(i);
        const CFDateRef cfDate = bobuiDateTime.toCFDate();
        QCOMPARE(QDateTime::fromCFDate(cfDate), bobuiDateTime);
        CFRelease(cfDate);
    }
    {
        QDateTime bobuiDateTime = QDateTime::fromMSecsSinceEpoch(0);
        const CFDateRef cfDate = bobuiDateTime.toCFDate();
        QDateTime bobuiDateTimeCopy(bobuiDateTime);
        bobuiDateTime.setSecsSinceEpoch(10000); // modify
        QCOMPARE(QDateTime::fromCFDate(cfDate), bobuiDateTimeCopy);
    }
    // QDateTime <-> NSDate
    for (int i = 0; i < kMsPerSecond; ++i) {
        QMacAutoReleasePool pool;
        QDateTime bobuiDateTime = QDateTime::fromMSecsSinceEpoch(i);
        const NSDate *nsDate = bobuiDateTime.toNSDate();
        QCOMPARE(QDateTime::fromNSDate(nsDate), bobuiDateTime);
    }
    {
        QMacAutoReleasePool pool;
        QDateTime bobuiDateTime = QDateTime::fromMSecsSinceEpoch(0);
        const NSDate *nsDate = bobuiDateTime.toNSDate();
        QDateTime bobuiDateTimeCopy(bobuiDateTime);
        bobuiDateTime.setSecsSinceEpoch(10000); // modify
        QCOMPARE(QDateTime::fromNSDate(nsDate), bobuiDateTimeCopy);
    }
}
