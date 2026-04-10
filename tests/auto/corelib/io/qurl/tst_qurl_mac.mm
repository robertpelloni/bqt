// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUICore/private/qcore_mac_p.h>

#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>

using namespace BobUI::StringLiterals;

void tst_QUrl_mactypes_data()
{
    BOBUIest::addColumn<QString>("input");
    BOBUIest::addColumn<QString>("cfOutput");
    BOBUIest::addColumn<QString>("nsOutput");

    auto addSimple = [](const char *label, QLatin1StringView urll1) {
        QString url = urll1;
        BOBUIest::addRow("%s", label) << url << url << url;
    };
    addSimple("empty", {});

    addSimple("https-empty-path", "https://example.com"_L1);
    addSimple("https-nonempty-path", "https://example.com/"_L1);
    addSimple("https-query", "https://example.com/?a=b"_L1);
    addSimple("https-fragment", "https://example.com/#id"_L1);
    addSimple("https-query-fragment", "https://example.com/?a=b#id"_L1);
    addSimple("file-root", "file:///"_L1);
    addSimple("file-path", "file:///etc/passwd"_L1);

    addSimple("file-relative", "file:README.txt"_L1);
    addSimple("file-relative-dotdot", "file:../README.txt"_L1);
    addSimple("uri-relative", "README.txt"_L1);
    addSimple("uri-relative-dotdot", "../README.txt"_L1);

    // QUrl retains [] unencoded, unlike CFURL & NSURL
    BOBUIest::newRow("gen-delims") << "x://:@host/:@/[]?:/?@[]?#:/?@[]"
                                << "x://:@host/:@/%5B%5D?:/?@%5B%5D?#:/?@%5B%5D"
                                << "x://:@host/:@/%5B%5D?:/?@%5B%5D?#:/?@%5B%5D";
}

void tst_QUrl_mactypes()
{
    QFETCH(QString, input);
    QFETCH(QString, cfOutput);
    QFETCH(QString, nsOutput);
    QUrl bobuiUrl(input);
    QUrl otherUrl = bobuiUrl.isEmpty() ? QUrl("https://example.com") : QUrl();

    // confirm the conversions result in what we expect it to result
    CFURLRef cfUrl = bobuiUrl.toCFURL();
    QCFString cfStr = CFURLGetString(cfUrl);
    QCOMPARE(QString(cfStr), cfOutput);

    const NSURL *nsUrl = bobuiUrl.toNSURL();
    QVERIFY(nsUrl);
    const NSString *nsString = [nsUrl absoluteString];
    QVERIFY(nsString);
    QCOMPARE(QString::fromNSString(nsString), nsOutput);

    // confirm that roundtripping works and the equality operator does too
    QUrl bobuiCfUrl = QUrl::fromCFURL(cfUrl);
    if (input == cfOutput) {
        QCOMPARE(bobuiCfUrl, bobuiUrl);
        QCOMPARE_NE(bobuiCfUrl, otherUrl);
    }
    QCOMPARE(bobuiCfUrl.isEmpty(), bobuiUrl.isEmpty());

    QUrl bobuiNsUrl = QUrl::fromNSURL(nsUrl);
    if (input == nsOutput) {
        QCOMPARE(bobuiNsUrl, bobuiUrl);
        QCOMPARE_NE(bobuiNsUrl, otherUrl);
    }
    QCOMPARE(bobuiNsUrl.isEmpty(), bobuiUrl.isEmpty());
}
