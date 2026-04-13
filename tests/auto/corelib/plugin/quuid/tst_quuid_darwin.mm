// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/QUuid>
#include <BOBUIest>

#include <BobUICore/private/qcore_mac_p.h>

#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>

void tst_QUuid_darwinTypes()
{
    // QUuid <-> CFUUID
    {
        const auto bobuiUuid = QUuid::fromString(QLatin1String("0f7169cc-5711-4af9-99d9-fecb2329fdef"));
        const CFUUIDRef cfuuid = bobuiUuid.toCFUUID();
        QCOMPARE(QUuid::fromCFUUID(cfuuid), bobuiUuid);
        CFStringRef cfstring = CFUUIDCreateString(0, cfuuid);
        QCOMPARE(QString::fromCFString(cfstring), bobuiUuid.toString().mid(1, 36).toUpper());
        CFRelease(cfstring);
        CFRelease(cfuuid);
    }
    {
        auto bobuiUuid = QUuid::fromString(QLatin1String("0f7169cc-5711-4af9-99d9-fecb2329fdef"));
        const CFUUIDRef cfuuid = bobuiUuid.toCFUUID();
        QUuid bobuiUuidCopy(bobuiUuid);
        bobuiUuid = QUuid::fromString(QLatin1String("93eec131-13c5-4d13-aaea-e456b4c57efa")); // modify
        QCOMPARE(QUuid::fromCFUUID(cfuuid), bobuiUuidCopy);
        CFStringRef cfstring = CFUUIDCreateString(0, cfuuid);
        QCOMPARE(QString::fromCFString(cfstring), bobuiUuidCopy.toString().mid(1, 36).toUpper());
        CFRelease(cfstring);
        CFRelease(cfuuid);
    }
    // QUuid <-> NSUUID
    {
        QMacAutoReleasePool pool;

        const auto bobuiUuid = QUuid::fromString(QLatin1String("0f7169cc-5711-4af9-99d9-fecb2329fdef"));
        const NSUUID *nsuuid = bobuiUuid.toNSUUID();
        QCOMPARE(QUuid::fromNSUUID(nsuuid), bobuiUuid);
        QCOMPARE(QString::fromNSString([nsuuid UUIDString]), bobuiUuid.toString().mid(1, 36).toUpper());
    }
    {
        QMacAutoReleasePool pool;

        auto bobuiUuid = QUuid::fromString(QLatin1String("0f7169cc-5711-4af9-99d9-fecb2329fdef"));
        const NSUUID *nsuuid = bobuiUuid.toNSUUID();
        QUuid bobuiUuidCopy(bobuiUuid);
        bobuiUuid = QUuid::fromString(QLatin1String("93eec131-13c5-4d13-aaea-e456b4c57efa")); // modify
        QCOMPARE(QUuid::fromNSUUID(nsuuid), bobuiUuidCopy);
        QCOMPARE(QString::fromNSString([nsuuid UUIDString]), bobuiUuidCopy.toString().mid(1, 36).toUpper());
    }
}
