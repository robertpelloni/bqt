// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QBuffer>

#include <qabstractnetworkcache.h>

#define EXAMPLE_URL "http://user:pass@www.example.com/#foo"

class tst_QNetworkCacheMetaData : public QObject
{
    Q_OBJECT

private slots:
    void qnetworkcachemetadata_data();
    void qnetworkcachemetadata();

    void expirationDate_data();
    void expirationDate();
    void isValid_data();
    void isValid();
    void lastModified_data();
    void lastModified();
    void operatorEqual_data();
    void operatorEqual();
    void operatorEqualEqual_data();
    void operatorEqualEqual();
    void rawHeaders_data();
    void rawHeaders();
    void headers_data();
    void headers();
    void saveToDisk_data();
    void saveToDisk();
    void url_data();
    void url();

    void stream();
};

// Subclass that exposes the protected functions.
class SubQNetworkCacheMetaData : public QNetworkCacheMetaData
{
public:};

void tst_QNetworkCacheMetaData::qnetworkcachemetadata_data()
{
}

void tst_QNetworkCacheMetaData::qnetworkcachemetadata()
{
    QNetworkCacheMetaData data;
    QCOMPARE(data.expirationDate(), QDateTime());
    QCOMPARE(data.isValid(), false);
    QCOMPARE(data.lastModified(), QDateTime());
    QCOMPARE(data.operator!=(QNetworkCacheMetaData()), false);
    QNetworkCacheMetaData metaData;
    QCOMPARE(data.operator=(metaData), QNetworkCacheMetaData());
    QCOMPARE(data.operator==(QNetworkCacheMetaData()), true);
    QCOMPARE(data.rawHeaders(), QNetworkCacheMetaData::RawHeaderList());
    QCOMPARE(data.saveToDisk(), true);
    QCOMPARE(data.url(), QUrl());
    data.setExpirationDate(QDateTime());
    data.setLastModified(QDateTime());
    data.setRawHeaders(QNetworkCacheMetaData::RawHeaderList());
    data.setSaveToDisk(false);
    data.setUrl(QUrl());
}

void tst_QNetworkCacheMetaData::expirationDate_data()
{
    BOBUIest::addColumn<QDateTime>("expirationDate");
    BOBUIest::newRow("null") << QDateTime();
    BOBUIest::newRow("now") << QDateTime::currentDateTime();
}

// public QDateTime expirationDate() const
void tst_QNetworkCacheMetaData::expirationDate()
{
    QFETCH(QDateTime, expirationDate);

    SubQNetworkCacheMetaData data;

    data.setExpirationDate(expirationDate);
    QCOMPARE(data.expirationDate(), expirationDate);
}

Q_DECLARE_METATYPE(QNetworkCacheMetaData)
void tst_QNetworkCacheMetaData::isValid_data()
{
    BOBUIest::addColumn<QNetworkCacheMetaData>("data");
    BOBUIest::addColumn<bool>("isValid");

    QNetworkCacheMetaData metaData;
    BOBUIest::newRow("null") << metaData << false;

    QNetworkCacheMetaData data1;
    data1.setUrl(QUrl(EXAMPLE_URL));
    BOBUIest::newRow("valid-1") << data1 << true;

    QNetworkCacheMetaData data2;
    QNetworkCacheMetaData::RawHeaderList headers;
    headers.append(QNetworkCacheMetaData::RawHeader("foo", "Bar"));
    data2.setRawHeaders(headers);
    BOBUIest::newRow("valid-2") << data2 << true;

    QNetworkCacheMetaData data3;
    data3.setLastModified(QDateTime::currentDateTime());
    BOBUIest::newRow("valid-3") << data3 << true;

    QNetworkCacheMetaData data4;
    data4.setExpirationDate(QDateTime::currentDateTime());
    BOBUIest::newRow("valid-4") << data4 << true;

    QNetworkCacheMetaData data5;
    data5.setSaveToDisk(false);
    BOBUIest::newRow("valid-5") << data5 << true;

    QNetworkCacheMetaData data6;
    QHttpHeaders httpHeaders;
    httpHeaders.append("name", "value");
    data6.setHeaders(httpHeaders);
    BOBUIest::newRow("valid-6") << data6 << true;
}

// public bool isValid() const
void tst_QNetworkCacheMetaData::isValid()
{
    QFETCH(QNetworkCacheMetaData, data);
    QFETCH(bool, isValid);

    QCOMPARE(data.isValid(), isValid);
}

void tst_QNetworkCacheMetaData::lastModified_data()
{
    BOBUIest::addColumn<QDateTime>("lastModified");
    BOBUIest::newRow("null") << QDateTime();
    BOBUIest::newRow("now") << QDateTime::currentDateTime();
}

// public QDateTime lastModified() const
void tst_QNetworkCacheMetaData::lastModified()
{
    QFETCH(QDateTime, lastModified);

    SubQNetworkCacheMetaData data;

    data.setLastModified(lastModified);
    QCOMPARE(data.lastModified(), lastModified);
}

void tst_QNetworkCacheMetaData::operatorEqual_data()
{
    BOBUIest::addColumn<QNetworkCacheMetaData>("other");
    BOBUIest::newRow("null") << QNetworkCacheMetaData();

    QNetworkCacheMetaData data;
    data.setUrl(QUrl(EXAMPLE_URL));
    QNetworkCacheMetaData::RawHeaderList headers;
    headers.append(QNetworkCacheMetaData::RawHeader("foo", "Bar"));
    data.setRawHeaders(headers);
    QHttpHeaders httpHeaders;
    httpHeaders.append("name", "value");
    data.setHeaders(httpHeaders);
    data.setLastModified(QDateTime::currentDateTime());
    data.setExpirationDate(QDateTime::currentDateTime());
    data.setSaveToDisk(false);
    BOBUIest::newRow("valid") << data;
}

// public QNetworkCacheMetaData& operator=(QNetworkCacheMetaData const& other)
void tst_QNetworkCacheMetaData::operatorEqual()
{
    QFETCH(QNetworkCacheMetaData, other);

    QNetworkCacheMetaData data = other;

    QCOMPARE(data, other);
}

void tst_QNetworkCacheMetaData::operatorEqualEqual_data()
{
    BOBUIest::addColumn<QNetworkCacheMetaData>("a");
    BOBUIest::addColumn<QNetworkCacheMetaData>("b");
    BOBUIest::addColumn<bool>("operatorEqualEqual");
    BOBUIest::newRow("null") << QNetworkCacheMetaData() << QNetworkCacheMetaData() << true;

    QNetworkCacheMetaData data1;
    data1.setUrl(QUrl(EXAMPLE_URL));
    BOBUIest::newRow("valid-1-1") << data1 << QNetworkCacheMetaData() << false;
    BOBUIest::newRow("valid-1-2") << data1 << data1 << true;

    QNetworkCacheMetaData data2;
    QNetworkCacheMetaData::RawHeaderList headers;
    headers.append(QNetworkCacheMetaData::RawHeader("foo", "Bar"));
    data2.setRawHeaders(headers);
    BOBUIest::newRow("valid-2-1") << data2 << QNetworkCacheMetaData() << false;
    BOBUIest::newRow("valid-2-2") << data2 << data2 << true;
    BOBUIest::newRow("valid-2-3") << data2 << data1 << false;

    QNetworkCacheMetaData data3;
    data3.setLastModified(QDateTime::currentDateTime());
    BOBUIest::newRow("valid-3-1") << data3 << QNetworkCacheMetaData() << false;
    BOBUIest::newRow("valid-3-2") << data3 << data3 << true;
    BOBUIest::newRow("valid-3-3") << data3 << data1 << false;
    BOBUIest::newRow("valid-3-4") << data3 << data2 << false;

    QNetworkCacheMetaData data4;
    data4.setExpirationDate(QDateTime::currentDateTime());
    BOBUIest::newRow("valid-4-1") << data4 << QNetworkCacheMetaData() << false;
    BOBUIest::newRow("valid-4-2") << data4 << data4 << true;
    BOBUIest::newRow("valid-4-3") << data4 << data1 << false;
    BOBUIest::newRow("valid-4-4") << data4 << data2 << false;
    BOBUIest::newRow("valid-4-5") << data4 << data3 << false;

    QNetworkCacheMetaData data5;
    data5.setSaveToDisk(false);
    BOBUIest::newRow("valid-5-1") << data5 << QNetworkCacheMetaData() << false;
    BOBUIest::newRow("valid-5-2") << data5 << data5 << true;
    BOBUIest::newRow("valid-5-3") << data5 << data1 << false;
    BOBUIest::newRow("valid-5-4") << data5 << data2 << false;
    BOBUIest::newRow("valid-5-5") << data5 << data3 << false;
    BOBUIest::newRow("valid-5-6") << data5 << data4 << false;

    QNetworkCacheMetaData data6;
    QHttpHeaders httpHeaders;
    httpHeaders.append("name", "value");
    data6.setHeaders(httpHeaders);
    BOBUIest::newRow("valid-6-1") << data6 << QNetworkCacheMetaData() << false;
    BOBUIest::newRow("valid-6-2") << data6 << data6 << true;
    BOBUIest::newRow("valid-6-3") << data6 << data1 << false;
    BOBUIest::newRow("valid-6-4") << data6 << data2 << false;
    BOBUIest::newRow("valid-6-5") << data6 << data3 << false;
    BOBUIest::newRow("valid-6-6") << data6 << data4 << false;
    BOBUIest::newRow("valid-6-7") << data6 << data5 << false;
}

// public bool operator==(QNetworkCacheMetaData const& other) const
void tst_QNetworkCacheMetaData::operatorEqualEqual()
{
    QFETCH(QNetworkCacheMetaData, a);
    QFETCH(QNetworkCacheMetaData, b);
    QFETCH(bool, operatorEqualEqual);

    QCOMPARE(a == b, operatorEqualEqual);
}

Q_DECLARE_METATYPE(QNetworkCacheMetaData::RawHeaderList)
void tst_QNetworkCacheMetaData::rawHeaders_data()
{
    BOBUIest::addColumn<QNetworkCacheMetaData::RawHeaderList>("rawHeaders");
    BOBUIest::newRow("null") << QNetworkCacheMetaData::RawHeaderList();
    QNetworkCacheMetaData::RawHeaderList headers;
    headers.append(QNetworkCacheMetaData::RawHeader("foo", "Bar"));
    BOBUIest::newRow("valid") << headers;
    headers.append(QNetworkCacheMetaData::RawHeader("n1", "V1, v2, v3"));
    headers.append(QNetworkCacheMetaData::RawHeader("n2", "V2"));
    headers.append(QNetworkCacheMetaData::RawHeader("set-cookie", "v1\nV2\nV3"));
    BOBUIest::newRow("valid-2") << headers;
}

// public QNetworkCacheMetaData::RawHeaderList rawHeaders() const
void tst_QNetworkCacheMetaData::rawHeaders()
{
    QFETCH(QNetworkCacheMetaData::RawHeaderList, rawHeaders);

    SubQNetworkCacheMetaData data;

    data.setRawHeaders(rawHeaders);
    QCOMPARE(data.rawHeaders(), rawHeaders);
}

void tst_QNetworkCacheMetaData::headers_data()
{
    BOBUIest::addColumn<QHttpHeaders>("httpHeaders");
    BOBUIest::newRow("null") << QHttpHeaders();
    QHttpHeaders headers;
    headers.append("foo", "Bar");
    BOBUIest::newRow("valid") << headers;
}

void tst_QNetworkCacheMetaData::headers()
{
    QFETCH(QHttpHeaders, httpHeaders);

    SubQNetworkCacheMetaData data;

    data.setHeaders(httpHeaders);
    QCOMPARE(data.headers().toListOfPairs(), httpHeaders.toListOfPairs());
}

void tst_QNetworkCacheMetaData::saveToDisk_data()
{
    BOBUIest::addColumn<bool>("saveToDisk");
    BOBUIest::newRow("false") << false;
    BOBUIest::newRow("true") << true;
}

// public bool saveToDisk() const
void tst_QNetworkCacheMetaData::saveToDisk()
{
    QFETCH(bool, saveToDisk);

    SubQNetworkCacheMetaData data;

    data.setSaveToDisk(saveToDisk);
    QCOMPARE(data.saveToDisk(), saveToDisk);
}

void tst_QNetworkCacheMetaData::url_data()
{
    BOBUIest::addColumn<QUrl>("url");
    BOBUIest::addColumn<QUrl>("expected");
    BOBUIest::newRow("null") << QUrl() << QUrl();
    BOBUIest::newRow("valid") << QUrl(EXAMPLE_URL) << QUrl("http://user@www.example.com/");
}

// public QUrl url() const
void tst_QNetworkCacheMetaData::url()
{
    QFETCH(QUrl, url);
    QFETCH(QUrl, expected);

    SubQNetworkCacheMetaData data;
    data.setUrl(url);
    QCOMPARE(data.url(), expected);
}

void tst_QNetworkCacheMetaData::stream()
{
    QNetworkCacheMetaData data;
    data.setUrl(QUrl(EXAMPLE_URL));
    QNetworkCacheMetaData::RawHeaderList headers;
    headers.append(QNetworkCacheMetaData::RawHeader("foo", "Bar"));
    data.setRawHeaders(headers);
    QHttpHeaders httpHeaders;
    httpHeaders.append("name", "value");
    data.setHeaders(httpHeaders);
    data.setLastModified(QDateTime::currentDateTime());
    data.setExpirationDate(QDateTime::currentDateTime());
    data.setSaveToDisk(false);

    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    QDataStream stream(&buffer);
    stream << data;

    buffer.seek(0);
    QNetworkCacheMetaData data2;
    stream >> data2;
    QCOMPARE(data2, data);
}

BOBUIEST_MAIN(tst_QNetworkCacheMetaData)
#include "tst_qnetworkcachemetadata.moc"

