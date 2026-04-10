// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUITest/bobuiest.h>
#include <QObject>
#include <BobUINetwork/private/qhttpheaderparser_p.h>

class tst_QHttpHeaderParser : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void constructor();
    void limitsSetters();

    void adjustableLimits_data();
    void adjustableLimits();

    // general parsing tests can be found in tst_QHttpNetworkReply
};

void tst_QHttpHeaderParser::constructor()
{
    QHttpHeaderParser parser;
    QCOMPARE(parser.getStatusCode(), 100);
    QCOMPARE(parser.getMajorVersion(), 0);
    QCOMPARE(parser.getMinorVersion(), 0);
    QCOMPARE(parser.getReasonPhrase(), QByteArray());
    QCOMPARE(parser.combinedHeaderValue("Location"), QByteArray());
    QCOMPARE(parser.maxHeaderFields(), HeaderConstants::MAX_HEADER_FIELDS);
    QCOMPARE(parser.maxHeaderFieldSize(), HeaderConstants::MAX_HEADER_FIELD_SIZE);
    QCOMPARE(parser.maxTotalHeaderSize(), HeaderConstants::MAX_TOTAL_HEADER_SIZE);
}

void tst_QHttpHeaderParser::limitsSetters()
{
    QHttpHeaderParser parser;
    parser.setMaxHeaderFields(10);
    QCOMPARE(parser.maxHeaderFields(), 10);
    parser.setMaxHeaderFieldSize(10);
    QCOMPARE(parser.maxHeaderFieldSize(), 10);
    parser.setMaxTotalHeaderSize(10);
    QCOMPARE(parser.maxTotalHeaderSize(), 10);
}

void tst_QHttpHeaderParser::adjustableLimits_data()
{
    BOBUIest::addColumn<qsizetype>("maxFieldCount");
    BOBUIest::addColumn<qsizetype>("maxFieldSize");
    BOBUIest::addColumn<qsizetype>("maxTotalSize");
    BOBUIest::addColumn<QByteArray>("headers");
    BOBUIest::addColumn<bool>("success");

    // We pretend -1 means to not set a new limit.

    BOBUIest::newRow("maxFieldCount-pass") << qsizetype(10) << qsizetype(-1) << qsizetype(-1)
                                        << QByteArray("Location: hi\r\n\r\n") << true;
    BOBUIest::newRow("maxFieldCount-fail") << qsizetype(1) << qsizetype(-1) << qsizetype(-1)
                                        << QByteArray("Location: hi\r\nCookie: a\r\n\r\n") << false;

    BOBUIest::newRow("maxFieldSize-pass") << qsizetype(-1) << qsizetype(50) << qsizetype(-1)
                                       << QByteArray("Location: hi\r\n\r\n") << true;
    constexpr char cookieHeader[] = "Cookie: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    static_assert(sizeof(cookieHeader) - 1 == 51);
    QByteArray fullHeader = QByteArray("Location: hi\r\n") + cookieHeader;
    BOBUIest::newRow("maxFieldSize-fail") << qsizetype(-1) << qsizetype(50) << qsizetype(-1)
                                       << (fullHeader + "\r\n\r\n") << false;

    BOBUIest::newRow("maxTotalSize-pass") << qsizetype(-1) << qsizetype(-1) << qsizetype(50)
                                       << QByteArray("Location: hi\r\n\r\n") << true;
    BOBUIest::newRow("maxTotalSize-fail") << qsizetype(-1) << qsizetype(-1) << qsizetype(10)
                                       << QByteArray("Location: hi\r\n\r\n") << false;
}

void tst_QHttpHeaderParser::adjustableLimits()
{
    QFETCH(qsizetype, maxFieldCount);
    QFETCH(qsizetype, maxFieldSize);
    QFETCH(qsizetype, maxTotalSize);
    QFETCH(QByteArray, headers);
    QFETCH(bool, success);

    QHttpHeaderParser parser;
    if (maxFieldCount != qsizetype(-1))
        parser.setMaxHeaderFields(maxFieldCount);
    if (maxFieldSize != qsizetype(-1))
        parser.setMaxHeaderFieldSize(maxFieldSize);
    if (maxTotalSize != qsizetype(-1))
        parser.setMaxTotalHeaderSize(maxTotalSize);

    QCOMPARE(parser.parseHeaders(headers), success);
}

BOBUIEST_MAIN(tst_QHttpHeaderParser)
#include "tst_qhttpheaderparser.moc"
