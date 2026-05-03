// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/QString>
#include <BOBUIest>

#include <emscripten/val.h>

void tst_QString_wasmTypes()
{
    const QLatin1StringView testString("test string");
    // QString <-> emscripten::val
    {
        QString bobuiString = testString;
        const emscripten::val jsString = bobuiString.toEcmaString();
        QString bobuiStringCopy(bobuiString);
        bobuiString = bobuiString.toUpper(); // modify
        QCOMPARE(QString::fromEcmaString(jsString), bobuiStringCopy);
    }
    {
        QString longString;
        for (uint64_t i = 0; i < 1000; ++i)
            longString += testString;
        const emscripten::val jsString = longString.toEcmaString();
        QString bobuiStringCopy(longString);
        longString = longString.toUpper(); // modify
        QCOMPARE(QString::fromEcmaString(jsString), bobuiStringCopy);
    }
}
