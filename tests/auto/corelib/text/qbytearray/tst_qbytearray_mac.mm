// Copyright (C) 2014 Samuel Gaist <samuel.gaist@edeltech.ch>
// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/QByteArray>
#include <BOBUIest>

#include <BobUICore/private/qcore_mac_p.h>

#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>

void tst_QByteArray_macTypes()
{
    // QByteArray <-> CFData
    {
        QByteArray bobuiByteArray("test bytearray");
        const CFDataRef cfData = bobuiByteArray.toCFData();
        QCOMPARE(QByteArray::fromCFData(cfData), bobuiByteArray);
        CFRelease(cfData);
    }
    {
        QByteArray bobuiByteArray("test bytearray");
        const CFDataRef cfData = bobuiByteArray.toCFData();
        QByteArray bobuiByteArrayCopy(bobuiByteArray);
        bobuiByteArray = bobuiByteArray.toUpper(); // modify
        QCOMPARE(QByteArray::fromCFData(cfData), bobuiByteArrayCopy);
    }
    // QByteArray <-> CFData Raw
    {
        QByteArray bobuiByteArray("test bytearray");
        const CFDataRef cfData = bobuiByteArray.toRawCFData();
        const UInt8 * cfDataPtr = CFDataGetBytePtr(cfData);
        QCOMPARE(reinterpret_cast<const UInt8*>(bobuiByteArray.constData()), cfDataPtr);
        CFRelease(cfData);
    }
    {
        const UInt8 data[] = "cfdata test";
        const CFDataRef cfData = CFDataCreate(kCFAllocatorDefault, data, sizeof(data));
        const UInt8 * cfDataPtr = CFDataGetBytePtr(cfData);
        QByteArray bobuiByteArray = QByteArray::fromRawCFData(cfData);
        QCOMPARE(reinterpret_cast<const UInt8*>(bobuiByteArray.constData()), cfDataPtr);
        CFRelease(cfData);
    }
    // QByteArray <-> NSData
    {
        QMacAutoReleasePool pool;
        QByteArray bobuiByteArray("test bytearray");
        const NSData *nsData = bobuiByteArray.toNSData();
        QCOMPARE(QByteArray::fromNSData(nsData), bobuiByteArray);
    }
    {
        QMacAutoReleasePool pool;
        QByteArray bobuiByteArray("test bytearray");
        const NSData *nsData = bobuiByteArray.toNSData();
        QByteArray bobuiByteArrayCopy(bobuiByteArray);
        bobuiByteArray = bobuiByteArray.toUpper(); // modify
        QCOMPARE(QByteArray::fromNSData(nsData), bobuiByteArrayCopy);
    }
    // QByteArray <-> NSData Raw
    {
        QMacAutoReleasePool pool;
        QByteArray bobuiByteArray("test bytearray");
        const NSData *nsData = bobuiByteArray.toRawNSData();
        QCOMPARE([nsData bytes], bobuiByteArray.constData());
    }
    {
        QMacAutoReleasePool pool;
        const char data[] = "nsdata test";
        const NSData *nsData = [NSData dataWithBytes:data length:sizeof(data)];
        QByteArray bobuiByteArray = QByteArray::fromRawNSData(nsData);
        QCOMPARE(bobuiByteArray.constData(), [nsData bytes]);
    }
}
