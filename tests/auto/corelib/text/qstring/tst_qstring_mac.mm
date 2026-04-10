// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/QString>
#include <BOBUIest>

#include <BobUICore/private/qcore_mac_p.h>

#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>

using namespace BobUI::StringLiterals;

void tst_QString_macTypes()
{
    const QLatin1StringView testString("test string");
    // QString <-> CFString
    {
        QString bobuiString = testString;
        const CFStringRef cfString = bobuiString.toCFString();
        QCOMPARE(QString::fromCFString(cfString), bobuiString);
        CFRelease(cfString);
    }
    {
        QString bobuiString = testString;
        const CFStringRef cfString = bobuiString.toCFString();
        QString bobuiStringCopy(bobuiString);
        bobuiString = bobuiString.toUpper(); // modify
        QCOMPARE(QString::fromCFString(cfString), bobuiStringCopy);
    }
    // QString <-> NSString
    {
        QMacAutoReleasePool pool;

        QString bobuiString = testString;
        const NSString *nsString = bobuiString.toNSString();
        QCOMPARE(QString::fromNSString(nsString), bobuiString);
    }
    {
        QMacAutoReleasePool pool;

        QString bobuiString = testString;
        const NSString *nsString = bobuiString.toNSString();
        QString bobuiStringCopy(bobuiString);
        bobuiString = bobuiString.toUpper(); // modify
        QCOMPARE(QString::fromNSString(nsString), bobuiStringCopy);
    }
}
