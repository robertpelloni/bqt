// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

#if BOBUI_CONFIG(http)
#include <BobUINetwork/QHttp1Configuration>
#include <BobUINetwork/QHttp2Configuration>
#endif
#include <BobUINetwork/QNetworkRequest>
#include <BobUINetwork/QNetworkCookie>

#include <BobUICore/QDateTime>
#include <BobUICore/BOBUIimeZone>
#include <BobUICore/QUrl>

Q_DECLARE_METATYPE(QNetworkRequest::KnownHeaders)

class tst_QNetworkRequest: public QObject
{
    Q_OBJECT

private slots:
    void ctor_data();
    void ctor();
    void implicitDefaultCtor();
    void setUrl_data();
    void setUrl();
    void setRawHeader_data();
    void setRawHeader();
    void rawHeaderList_data();
    void rawHeaderList();
    void setHeader_data();
    void setHeader();
    void rawHeaderParsing_data();
    void rawHeaderParsing();
    void originatingObject();
    void setHeaders_data();
    void setHeaders();
    void operatorEqual_data();
    void operatorEqual();

    void removeHeader();
};

void tst_QNetworkRequest::ctor_data()
{
    BOBUIest::addColumn<QUrl>("url");

    BOBUIest::newRow("nothing") << QUrl();
    BOBUIest::newRow("empty") << QUrl();
    BOBUIest::newRow("http") << QUrl("http://bobui-project.org");
}

void tst_QNetworkRequest::ctor()
{
    QFETCH(QUrl, url);

    if (qstrcmp(BOBUIest::currentDataTag(), "nothing") == 0) {
        QNetworkRequest request;
        QCOMPARE(request.url(), url);
    } else {
        QNetworkRequest request(url);
        QCOMPARE(request.url(), url);
    }
}

void tst_QNetworkRequest::implicitDefaultCtor()
{
    QNetworkRequest r = {};
    Q_UNUSED(r);
}

void tst_QNetworkRequest::setUrl_data()
{
    ctor_data();
}

void tst_QNetworkRequest::setUrl()
{
    QFETCH(QUrl, url);
    QNetworkRequest request;

    if (qstrcmp(BOBUIest::currentDataTag(), "nothing") != 0)
        request.setUrl(url);

    QCOMPARE(request.url(), url);
}

void tst_QNetworkRequest::setRawHeader_data()
{
    BOBUIest::addColumn<QByteArray>("header");
    BOBUIest::addColumn<QByteArray>("value");
    BOBUIest::addColumn<QByteArray>("headerToGet");
    BOBUIest::addColumn<QByteArray>("expectedValue");
    BOBUIest::addColumn<bool>("hasHeader");

    BOBUIest::newRow("null-header") << QByteArray() << QByteArray("abc")
                                 << QByteArray() << QByteArray() << false;
    BOBUIest::newRow("empty-header") << QByteArray("") << QByteArray("abc")
                                  << QByteArray("") << QByteArray() << false;
    BOBUIest::newRow("null-value") << QByteArray("foo") << QByteArray()
                                << QByteArray("foo") << QByteArray() << false;
    BOBUIest::newRow("empty-value") << QByteArray("foo") << QByteArray("")
                                 << QByteArray("foo") << QByteArray("") << true;
    BOBUIest::newRow("empty-value-vs-null") << QByteArray("foo") << QByteArray("")
                                         << QByteArray("foo") << QByteArray() << true;

    BOBUIest::newRow("UPPER-UPPER") << QByteArray("FOO") << QByteArray("abc")
                                 << QByteArray("FOO") << QByteArray("abc") << true;
    BOBUIest::newRow("UPPER-Mixed") << QByteArray("FOO") << QByteArray("abc")
                                 << QByteArray("Foo") << QByteArray("abc") << true;
    BOBUIest::newRow("UPPER-lower") << QByteArray("FOO") << QByteArray("abc")
                                 << QByteArray("foo") << QByteArray("abc") << true;
    BOBUIest::newRow("Mixed-UPPER") << QByteArray("Foo") << QByteArray("abc")
                                 << QByteArray("FOO") << QByteArray("abc") << true;
    BOBUIest::newRow("Mixed-Mixed") << QByteArray("Foo") << QByteArray("abc")
                                 << QByteArray("Foo") << QByteArray("abc") << true;
    BOBUIest::newRow("Mixed-lower") << QByteArray("Foo") << QByteArray("abc")
                                 << QByteArray("foo") << QByteArray("abc") << true;
    BOBUIest::newRow("lower-UPPER") << QByteArray("foo") << QByteArray("abc")
                                 << QByteArray("FOO") << QByteArray("abc") << true;
    BOBUIest::newRow("lower-Mixed") << QByteArray("foo") << QByteArray("abc")
                                 << QByteArray("Foo") << QByteArray("abc") << true;
    BOBUIest::newRow("lower-lower") << QByteArray("foo") << QByteArray("abc")
                                 << QByteArray("foo") << QByteArray("abc") << true;
}

void tst_QNetworkRequest::setRawHeader()
{
    QFETCH(QByteArray, header);
    QFETCH(QByteArray, value);
    QFETCH(QByteArray, headerToGet);
    QFETCH(QByteArray, expectedValue);
    QFETCH(bool, hasHeader);

    QNetworkRequest request;
    request.setRawHeader(header, value);

    QCOMPARE(request.hasRawHeader(headerToGet), hasHeader);
    QCOMPARE(request.rawHeader(headerToGet), expectedValue);
}

void tst_QNetworkRequest::rawHeaderList_data()
{
    BOBUIest::addColumn<QList<QByteArray> >("set");
    BOBUIest::addColumn<QList<QByteArray> >("expected");

    BOBUIest::newRow("empty") << QList<QByteArray>() << QList<QByteArray>();

    QList<QByteArray> set;
    QList<QByteArray> expected;

    set << "foo";
    expected = set;
    BOBUIest::newRow("one") << set << expected;

    set << "bar";
    expected = set;
    BOBUIest::newRow("two") << set << expected;

    set.clear();
    expected.clear();
    set << "foo" << "foo";
    expected << "foo";
    BOBUIest::newRow("repeated") << set << expected;

    set.clear();
    expected.clear();
    set << "foo" << "bar" << "foo";
    expected << "bar" << "foo";
    BOBUIest::newRow("repeated-interleaved") << set << expected;
}

void tst_QNetworkRequest::rawHeaderList()
{
    QFETCH(const QList<QByteArray>, set);
    QFETCH(QList<QByteArray>, expected);

    QNetworkRequest request;
    for (const QByteArray &header : set)
        request.setRawHeader(header, "a value");

    QList<QByteArray> got = request.rawHeaderList();
    QCOMPARE(got.size(), expected.size());
    for (int i = 0; i < got.size(); ++i)
        QCOMPARE(got.at(i), expected.at(i));
}

void tst_QNetworkRequest::setHeader_data()
{
    BOBUIest::addColumn<QNetworkRequest::KnownHeaders>("cookedHeader");
    BOBUIest::addColumn<QVariant>("cookedValue");
    BOBUIest::addColumn<bool>("success");
    BOBUIest::addColumn<QString>("rawHeader");
    BOBUIest::addColumn<QString>("rawValue");

    BOBUIest::newRow("Content-Type-Null") << QNetworkRequest::ContentTypeHeader << QVariant()
                                       << false << "Content-Type" << "";
    BOBUIest::newRow("Content-Type-String") << QNetworkRequest::ContentTypeHeader << QVariant("text/html")
                                         << true
                                         << "Content-Type" << "text/html";
    BOBUIest::newRow("Content-Type-ByteArray") << QNetworkRequest::ContentTypeHeader
                                            << QVariant("text/html") << true
                                            << "Content-Type" << "text/html";

    BOBUIest::newRow("Content-Length-Int") << QNetworkRequest::ContentLengthHeader << QVariant(1)
                                        << true << "Content-Length" << "1";
    BOBUIest::newRow("Content-Length-Int64") << QNetworkRequest::ContentLengthHeader << QVariant(qint64(1))
                                          << true << "Content-Length" << "1";

    BOBUIest::newRow("Location-String") << QNetworkRequest::LocationHeader << QVariant("http://foo/with space")
                                     << true << "Location" << "http://foo/with space";
    BOBUIest::newRow("Location-ByteArray") << QNetworkRequest::LocationHeader
                                        << QVariant("http://foo/with space")
                                        << true << "Location" << "http://foo/with space";
    BOBUIest::newRow("Location-Url") << QNetworkRequest::LocationHeader
                                  << QVariant(QUrl("http://foo/with space"))
                                  << true << "Location" << "http://foo/with%20space";

    BOBUIest::newRow("Last-Modified-Date") << QNetworkRequest::LastModifiedHeader
                                        << QVariant(QDate(2007, 11, 01))
                                        << true << "Last-Modified"
                                        << "Thu, 01 Nov 2007 00:00:00 GMT";
    BOBUIest::newRow("Last-Modified-DateTime-UTC")
        << QNetworkRequest::LastModifiedHeader
        << QVariant(QDateTime(QDate(2007, 11, 1), BOBUIime(18, 8, 30), BOBUIimeZone::UTC))
        << true << "Last-Modified" << "Thu, 01 Nov 2007 18:08:30 GMT";
    // BOBUIBUG-80666: format dates correctly (as GMT) even if the date passed in isn't in UTC:
    BOBUIest::newRow("Last-Modified-DateTime-Local")
        << QNetworkRequest::LastModifiedHeader
        << QVariant(QDateTime(QDate(2007, 11, 1), BOBUIime(18, 8, 30), BOBUIimeZone::UTC).toLocalTime())
        << true << "Last-Modified" << "Thu, 01 Nov 2007 18:08:30 GMT";
    BOBUIest::newRow("Last-Modified-DateTime-Offset")
        << QNetworkRequest::LastModifiedHeader
        << QVariant(QDateTime(QDate(2007, 11, 1), BOBUIime(18, 8, 30),
                              BOBUIimeZone::UTC).toOffsetFromUtc(3600))
        << true << "Last-Modified" << "Thu, 01 Nov 2007 18:08:30 GMT";
#if BOBUI_CONFIG(timezone)
    BOBUIimeZone cet("Europe/Oslo");
    if (cet.isValid()) {
        BOBUIest::newRow("Last-Modified-DateTime-CET")
            << QNetworkRequest::LastModifiedHeader
            << QVariant(QDateTime(QDate(2007, 11, 1), BOBUIime(18, 8, 30),
                                  BOBUIimeZone::UTC).toTimeZone(cet))
            << true << "Last-Modified" << "Thu, 01 Nov 2007 18:08:30 GMT";
    }
#endif

    BOBUIest::newRow("If-Modified-Since-Date") << QNetworkRequest::IfModifiedSinceHeader
                                        << QVariant(QDate(2017, 7, 01))
                                        << true << "If-Modified-Since"
                                        << "Sat, 01 Jul 2017 00:00:00 GMT";
    BOBUIest::newRow("If-Modified-Since-DateTime") << QNetworkRequest::IfModifiedSinceHeader
                                            << QVariant(QDateTime(QDate(2017, 7, 01),
                                                                  BOBUIime(3, 14, 15),
                                                                  BOBUIimeZone::UTC))
                                            << true << "If-Modified-Since"
                                            << "Sat, 01 Jul 2017 03:14:15 GMT";

    BOBUIest::newRow("Etag-strong") << QNetworkRequest::ETagHeader << QVariant(R"("xyzzy")")
                                            << true << "ETag" << R"("xyzzy")";
    BOBUIest::newRow("Etag-weak") << QNetworkRequest::ETagHeader << QVariant(R"(W/"xyzzy")")
                                            << true << "ETag" << R"(W/"xyzzy")";
    BOBUIest::newRow("Etag-empty") << QNetworkRequest::ETagHeader << QVariant(R"("")")
                                            << true << "ETag" << R"("")";

    BOBUIest::newRow("If-Match-empty") << QNetworkRequest::IfMatchHeader << QVariant(R"("")")
                                            << true << "If-Match" << R"("")";
    BOBUIest::newRow("If-Match-any") << QNetworkRequest::IfMatchHeader << QVariant(R"("*")")
                                            << true << "If-Match" << R"("*")";
    BOBUIest::newRow("If-Match-single") << QNetworkRequest::IfMatchHeader << QVariant(R"("xyzzy")")
                                            << true << "If-Match" << R"("xyzzy")";
    BOBUIest::newRow("If-Match-multiple") << QNetworkRequest::IfMatchHeader
                                            << QVariant(R"("xyzzy", "r2d2xxxx", "c3piozzzz")")
                                            << true << "If-Match"
                                            << R"("xyzzy", "r2d2xxxx", "c3piozzzz")";

    BOBUIest::newRow("If-None-Match-empty") << QNetworkRequest::IfNoneMatchHeader << QVariant(R"("")")
                                            << true << "If-None-Match" << R"("")";
    BOBUIest::newRow("If-None-Match-any") << QNetworkRequest::IfNoneMatchHeader << QVariant(R"("*")")
                                            << true << "If-None-Match" << R"("*")";
    BOBUIest::newRow("If-None-Match-single") << QNetworkRequest::IfNoneMatchHeader << QVariant(R"("xyzzy")")
                                            << true << "If-None-Match" << R"("xyzzy")";
    BOBUIest::newRow("If-None-Match-multiple") << QNetworkRequest::IfNoneMatchHeader
                                            << QVariant(R"("xyzzy", W/"r2d2xxxx", "c3piozzzz")")
                                            << true << "If-None-Match"
                                            << R"("xyzzy", W/"r2d2xxxx", "c3piozzzz")";

    QNetworkCookie cookie;
    cookie.setName("a");
    cookie.setValue("b");
    BOBUIest::newRow("Cookie-1") << QNetworkRequest::CookieHeader
                              << QVariant::fromValue(QList<QNetworkCookie>() << cookie)
                              << true << "Cookie"
                              << "a=b";
    BOBUIest::newRow("SetCookie-1") << QNetworkRequest::SetCookieHeader
                                 << QVariant::fromValue(QList<QNetworkCookie>() << cookie)
                                 << true << "Set-Cookie"
                                 << "a=b";

    cookie.setPath("/");
    BOBUIest::newRow("Cookie-2") << QNetworkRequest::CookieHeader
                              << QVariant::fromValue(QList<QNetworkCookie>() << cookie)
                              << true << "Cookie"
                              << "a=b";
    BOBUIest::newRow("SetCookie-2") << QNetworkRequest::SetCookieHeader
                                 << QVariant::fromValue(QList<QNetworkCookie>() << cookie)
                                 << true << "Set-Cookie"
                                 << "a=b; path=/";

    QNetworkCookie cookie2;
    cookie2.setName("c");
    cookie2.setValue("d");
    BOBUIest::newRow("Cookie-3") << QNetworkRequest::CookieHeader
                              << QVariant::fromValue(QList<QNetworkCookie>() << cookie << cookie2)
                              << true << "Cookie"
                              << "a=b; c=d";
    BOBUIest::newRow("SetCookie-3") << QNetworkRequest::SetCookieHeader
                                 << QVariant::fromValue(QList<QNetworkCookie>() << cookie << cookie2)
                                 << true << "Set-Cookie"
                                 << "a=b; path=/, c=d";
}

void tst_QNetworkRequest::setHeader()
{
    QFETCH(QNetworkRequest::KnownHeaders, cookedHeader);
    QFETCH(QVariant, cookedValue);
    QFETCH(bool, success);
    QFETCH(QString, rawHeader);
    QFETCH(QString, rawValue);

    QNetworkRequest request;
    request.setHeader(cookedHeader, cookedValue);

    QCOMPARE(request.header(cookedHeader).isNull(), !success);
    QCOMPARE(request.hasRawHeader(rawHeader.toLatin1()), success);
    QCOMPARE(request.rawHeader(rawHeader.toLatin1()).isEmpty(), !success);

    if (success) {
        QCOMPARE(request.header(cookedHeader), cookedValue);
        QCOMPARE(QString(request.rawHeader(rawHeader.toLatin1())), rawValue);
    }
}

void tst_QNetworkRequest::rawHeaderParsing_data()
{
    BOBUIest::addColumn<QNetworkRequest::KnownHeaders>("cookedHeader");
    BOBUIest::addColumn<QVariant>("cookedValue");
    BOBUIest::addColumn<bool>("success");
    BOBUIest::addColumn<QString>("rawHeader");
    BOBUIest::addColumn<QString>("rawValue");

    BOBUIest::newRow("Content-Type") << QNetworkRequest::ContentTypeHeader << QVariant("text/html")
                                  << true
                                  << "Content-Type" << "text/html";
    BOBUIest::newRow("Content-Length") << QNetworkRequest::ContentLengthHeader << QVariant(qint64(1))
                                    << true << "Content-Length" << "1";
    BOBUIest::newRow("Location") << QNetworkRequest::LocationHeader
                              << QVariant(QUrl("http://foo/with space"))
                              << true << "Location" << "http://foo/with%20space";
    BOBUIest::newRow("Last-Modified-RFC1123") << QNetworkRequest::LastModifiedHeader
                                           << QVariant(QDateTime(QDate(1994, 11, 06),
                                                                 BOBUIime(8, 49, 37),
                                                                 BOBUIimeZone::UTC))
                                           << true << "Last-Modified"
                                           << "Sun, 06 Nov 1994 08:49:37 GMT";
    BOBUIest::newRow("Last-Modified-RFC850") << QNetworkRequest::LastModifiedHeader
                                           << QVariant(QDateTime(QDate(1994, 11, 06),
                                                                 BOBUIime(8, 49, 37),
                                                                 BOBUIimeZone::UTC))
                                           << true << "Last-Modified"
                                           << "Sunday, 06-Nov-94 08:49:37 GMT";
    BOBUIest::newRow("Last-Modified-asctime") << QNetworkRequest::LastModifiedHeader
                                           << QVariant(QDateTime(QDate(1994, 11, 06),
                                                                 BOBUIime(8, 49, 37),
                                                                 BOBUIimeZone::UTC))
                                           << true << "Last-Modified"
                                           << "Sun Nov  6 08:49:37 1994";

    BOBUIest::newRow("If-Modified-Since-RFC1123") << QNetworkRequest::IfModifiedSinceHeader
                                           << QVariant(QDateTime(QDate(1994, 8, 06),
                                                                 BOBUIime(8, 49, 37),
                                                                 BOBUIimeZone::UTC))
                                           << true << "If-Modified-Since"
                                           << "Sun, 06 Aug 1994 08:49:37 GMT";
    BOBUIest::newRow("If-Modified-Since-RFC850") << QNetworkRequest::IfModifiedSinceHeader
                                           << QVariant(QDateTime(QDate(1994, 8, 06),
                                                                 BOBUIime(8, 49, 37),
                                                                 BOBUIimeZone::UTC))
                                           << true << "If-Modified-Since"
                                           << "Sunday, 06-Aug-94 08:49:37 GMT";
    BOBUIest::newRow("If-Modified-Since-asctime") << QNetworkRequest::IfModifiedSinceHeader
                                           << QVariant(QDateTime(QDate(1994, 8, 06),
                                                                 BOBUIime(8, 49, 37),
                                                                 BOBUIimeZone::UTC))
                                           << true << "If-Modified-Since"
                                           << "Sun Aug  6 08:49:37 1994";

    BOBUIest::newRow("Etag-strong") << QNetworkRequest::ETagHeader << QVariant(R"("xyzzy")")
                                            << true << "ETag" << R"("xyzzy")";
    BOBUIest::newRow("Etag-weak") << QNetworkRequest::ETagHeader << QVariant(R"(W/"xyzzy")")
                                            << true << "ETag" << R"(W/"xyzzy")";
    BOBUIest::newRow("Etag-empty") << QNetworkRequest::ETagHeader << QVariant(R"("")")
                                            << true << "ETag" << R"("")";

    BOBUIest::newRow("If-Match-empty") << QNetworkRequest::IfMatchHeader << QVariant(QStringList(R"("")"))
                                            << true << "If-Match" << R"("")";
    BOBUIest::newRow("If-Match-any") << QNetworkRequest::IfMatchHeader << QVariant(QStringList(R"("*")"))
                                            << true << "If-Match" << R"("*")";
    BOBUIest::newRow("If-Match-single") << QNetworkRequest::IfMatchHeader
                                            << QVariant(QStringList(R"("xyzzy")"))
                                            << true << "If-Match" << R"("xyzzy")";
    BOBUIest::newRow("If-Match-multiple") << QNetworkRequest::IfMatchHeader
                                            << QVariant(QStringList({R"("xyzzy")",
                                                                     R"("r2d2xxxx")",
                                                                     R"("c3piozzzz")"}))
                                           << true << "If-Match"
                                           << R"("xyzzy", "r2d2xxxx", "c3piozzzz")";

    BOBUIest::newRow("If-None-Match-empty") << QNetworkRequest::IfNoneMatchHeader
                                            << QVariant(QStringList(R"("")"))
                                            << true << "If-None-Match" << R"("")";
    BOBUIest::newRow("If-None-Match-any") << QNetworkRequest::IfNoneMatchHeader
                                            << QVariant(QStringList(R"("*")"))
                                            << true << "If-None-Match" << R"("*")";
    BOBUIest::newRow("If-None-Match-single") << QNetworkRequest::IfNoneMatchHeader
                                            << QVariant(QStringList(R"("xyzzy")"))
                                            << true << "If-None-Match" << R"("xyzzy")";
    BOBUIest::newRow("If-None-Match-multiple") << QNetworkRequest::IfNoneMatchHeader
                                            << QVariant(QStringList({R"("xyzzy")",
                                                                     R"(W/"r2d2xxxx")",
                                                                     R"("c3piozzzz")"}))
                                            << true << "If-None-Match"
                                            << R"("xyzzy", W/"r2d2xxxx", "c3piozzzz")";

    BOBUIest::newRow("Content-Length-invalid1") << QNetworkRequest::ContentLengthHeader << QVariant()
                                             << false << "Content-Length" << "1a";
    BOBUIest::newRow("Content-Length-invalid2") << QNetworkRequest::ContentLengthHeader << QVariant()
                                             << false << "Content-Length" << "a";


    BOBUIest::newRow("Location-invalid1") << QNetworkRequest::LocationHeader << QVariant() << false
                                       << "Location" << "abc";
    BOBUIest::newRow("Location-invalid2") << QNetworkRequest::LocationHeader << QVariant() << false
                                       << "Location" << "1http://foo";
    BOBUIest::newRow("Location-invalid3") << QNetworkRequest::LocationHeader << QVariant() << false
                                       << "Location" << "http://foo/%gg";

    // don't test for invalid dates because we may want to support broken servers in the future

    QNetworkCookie cookie;
    cookie.setName("a");
    cookie.setValue("b");
    BOBUIest::newRow("Cookie-1") << QNetworkRequest::CookieHeader
                              << QVariant::fromValue(QList<QNetworkCookie>() << cookie)
                              << true << "Cookie"
                              << "a=b";
    BOBUIest::newRow("SetCookie-1") << QNetworkRequest::SetCookieHeader
                                 << QVariant::fromValue(QList<QNetworkCookie>() << cookie)
                                 << true << "Set-Cookie"
                                 << "a=b";

    cookie.setPath("/");
    BOBUIest::newRow("SetCookie-2") << QNetworkRequest::SetCookieHeader
                                 << QVariant::fromValue(QList<QNetworkCookie>() << cookie)
                                 << true << "Set-Cookie"
                                 << "a=b; path=/";

    QNetworkCookie cookie2;
    cookie.setPath("");
    cookie2.setName("c");
    cookie2.setValue("d");
    BOBUIest::newRow("Cookie-3") << QNetworkRequest::CookieHeader
                              << QVariant::fromValue(QList<QNetworkCookie>() << cookie << cookie2)
                              << true << "Cookie"
                              << "a=b; c=d";
    cookie.setPath("/");
    BOBUIest::newRow("SetCookie-3") << QNetworkRequest::SetCookieHeader
                                 << QVariant::fromValue(QList<QNetworkCookie>() << cookie << cookie2)
                                 << true << "Set-Cookie"
                                 << "a=b; path=/\nc=d";
    BOBUIest::newRow("Content-Disposition") << QNetworkRequest::ContentDispositionHeader
                                         << QVariant("attachment; filename=\"test.txt\"") << true
                                         << "Content-Disposition" << "attachment; filename=\"test.txt\"";
}

void tst_QNetworkRequest::rawHeaderParsing()
{
    QFETCH(QNetworkRequest::KnownHeaders, cookedHeader);
    QFETCH(QVariant, cookedValue);
    QFETCH(bool, success);
    QFETCH(QString, rawHeader);
    QFETCH(QString, rawValue);

    QNetworkRequest request;
    request.setRawHeader(rawHeader.toLatin1(), rawValue.toLatin1());

    // even if it doesn't parse, it's as a raw header
    QVERIFY(request.hasRawHeader(rawHeader.toLatin1()));
    QVERIFY(request.hasRawHeader(rawHeader.toLower().toLatin1()));
    QCOMPARE(QString(request.rawHeader(rawHeader.toLatin1())), rawValue);

    QCOMPARE(request.header(cookedHeader).isNull(), !success);
    if (cookedValue.metaType().id() < QMetaType::User)
        QCOMPARE(request.header(cookedHeader), cookedValue);
    else if (cookedValue.userType() == qMetaTypeId<QList<QNetworkCookie> >())
        QCOMPARE(qvariant_cast<QList<QNetworkCookie> >(request.header(cookedHeader)),
                 qvariant_cast<QList<QNetworkCookie> >(cookedValue));
}

void tst_QNetworkRequest::removeHeader()
{
    QNetworkRequest request;

    request.setRawHeader("Foo", "1");
    QVERIFY(request.hasRawHeader("Foo"));
    QVERIFY(request.hasRawHeader("foo"));
    request.setRawHeader("Foo", QByteArray());
    QVERIFY(!request.hasRawHeader("Foo"));

    // same, but remove with different capitalisation
    request.setRawHeader("Foo", "1");
    QVERIFY(request.hasRawHeader("Foo"));
    QVERIFY(request.hasRawHeader("foo"));
    request.setRawHeader("foo", QByteArray());
    QVERIFY(!request.hasRawHeader("Foo"));

    // same, but not the first
    request.setRawHeader("Bar", "2");
    request.setRawHeader("Foo", "1");
    QVERIFY(request.hasRawHeader("Foo"));
    QVERIFY(request.hasRawHeader("foo"));
    request.setRawHeader("foo", QByteArray());
    QVERIFY(!request.hasRawHeader("Foo"));
    QVERIFY(request.hasRawHeader("bar"));

    // same, but not the first nor last
    request.setRawHeader("Foo", "1");
    request.setRawHeader("Bar", "3");
    QVERIFY(request.hasRawHeader("Foo"));
    QVERIFY(request.hasRawHeader("foo"));
    request.setRawHeader("foo", QByteArray());
    QVERIFY(!request.hasRawHeader("Foo"));
    QVERIFY(request.hasRawHeader("bar"));
}

void tst_QNetworkRequest::originatingObject()
{
    QNetworkRequest request;

    QVERIFY(!request.originatingObject());

    {
        QObject dummy;
        request.setOriginatingObject(&dummy);
        QCOMPARE(request.originatingObject(), &dummy);
    }

    QVERIFY(!request.originatingObject());
}

void tst_QNetworkRequest::setHeaders_data()
{
    BOBUIest::addColumn<QHttpHeaders>("h");
    BOBUIest::newRow("null") << QHttpHeaders();
    QHttpHeaders headers;
    headers.append("name1", "value1");
    BOBUIest::newRow("valid") << headers;
}

void tst_QNetworkRequest::setHeaders()
{
    QFETCH(QHttpHeaders, h);

    QNetworkRequest r1;

    auto result = r1.headers();
    QVERIFY(result.isEmpty());

    r1.setHeaders(h);
    QCOMPARE(r1.headers().toListOfPairs(), h.toListOfPairs());

    QNetworkRequest r2;
    auto tmp = h;
    r2.setHeaders(std::move(tmp));
    QCOMPARE(r2.headers().toListOfPairs(), h.toListOfPairs());
}

void tst_QNetworkRequest::operatorEqual_data()
{
    BOBUIest::addColumn<QNetworkRequest>("a");
    BOBUIest::addColumn<QNetworkRequest>("b");
    BOBUIest::addColumn<bool>("expectedToMatch");
    BOBUIest::newRow("null") << QNetworkRequest() << QNetworkRequest() << true;

    QNetworkRequest data1;
    data1.setUrl(QUrl("http://bobui-project.org"));
    BOBUIest::newRow("url-1-1") << data1 << QNetworkRequest() << false;
    BOBUIest::newRow("url-1-2") << data1 << data1 << true;

    QNetworkRequest data2;
    QHttpHeaders headers;
    headers.append("name1", "value1");
    data2.setHeaders(headers);
    BOBUIest::newRow("headers-2-1") << data2 << QNetworkRequest() << false;
    BOBUIest::newRow("headers-2-2") << data2 << data2 << true;
    BOBUIest::newRow("headers-2-3") << data2 << data1 << false;

    QNetworkRequest data3;
    data3.setPeerVerifyName("peerName");
    BOBUIest::newRow("peerName-3-1") << data3 << QNetworkRequest() << false;
    BOBUIest::newRow("peerName-3-2") << data3 << data3 << true;
    BOBUIest::newRow("peerName-3-3") << data3 << data1 << false;
    BOBUIest::newRow("peerName-3-4") << data3 << data2 << false;

    QNetworkRequest data4;
    data4.setAttribute(QNetworkRequest::Http2AllowedAttribute, true);
    BOBUIest::newRow("attribute-4-1") << data4 << QNetworkRequest() << false;
    BOBUIest::newRow("attribute-4-2") << data4 << data4 << true;
    BOBUIest::newRow("attribute-4-3") << data4 << data1 << false;
    BOBUIest::newRow("attribute-4-4") << data4 << data2 << false;
    BOBUIest::newRow("attribute-4-5") << data4 << data3 << false;

    QNetworkRequest data5;
    data5.setPriority(QNetworkRequest::Priority::HighPriority);
    BOBUIest::newRow("priority-5-1") << data5 << QNetworkRequest() << false;
    BOBUIest::newRow("priority-5-2") << data5 << data5 << true;
    BOBUIest::newRow("priority-5-3") << data5 << data1 << false;
    BOBUIest::newRow("priority-5-4") << data5 << data2 << false;
    BOBUIest::newRow("priority-5-5") << data5 << data3 << false;
    BOBUIest::newRow("priority-5-6") << data5 << data4 << false;

    QNetworkRequest data6;
    data6.setMaximumRedirectsAllowed(3);
    BOBUIest::newRow("maxRedirects-6-1") << data6 << QNetworkRequest() << false;
    BOBUIest::newRow("maxRedirects-6-2") << data6 << data6 << true;
    BOBUIest::newRow("maxRedirects-6-3") << data6 << data1 << false;
    BOBUIest::newRow("maxRedirects-6-4") << data6 << data2 << false;
    BOBUIest::newRow("maxRedirects-6-5") << data6 << data3 << false;
    BOBUIest::newRow("maxRedirects-6-6") << data6 << data4 << false;
    BOBUIest::newRow("maxRedirects-6-7") << data6 << data5 << false;

#if BOBUI_CONFIG(http)
    QNetworkRequest data7;
    QHttp1Configuration http1Configuration;
    http1Configuration.setNumberOfConnectionsPerHost(5);
    data7.setHttp1Configuration(http1Configuration);
    BOBUIest::newRow("http1Config-7-1") << data7 << QNetworkRequest() << false;
    BOBUIest::newRow("http1Config-7-2") << data7 << data7 << true;
    BOBUIest::newRow("http1Config-7-3") << data7 << data1 << false;
    BOBUIest::newRow("http1Config-7-4") << data7 << data2 << false;
    BOBUIest::newRow("http1Config-7-5") << data7 << data3 << false;
    BOBUIest::newRow("http1Config-7-6") << data7 << data4 << false;
    BOBUIest::newRow("http1Config-7-7") << data7 << data5 << false;
    BOBUIest::newRow("http1Config-7-8") << data7 << data6 << false;

    QNetworkRequest data8;
    QHttp2Configuration http2Configuration;
    http2Configuration.setMaxFrameSize(16386);
    data8.setHttp2Configuration(http2Configuration);
    BOBUIest::newRow("http2Config-8-1") << data8 << QNetworkRequest() << false;
    BOBUIest::newRow("http2Config-8-2") << data8 << data8 << true;
    BOBUIest::newRow("http2Config-8-3") << data8 << data1 << false;
    BOBUIest::newRow("http2Config-8-4") << data8 << data2 << false;
    BOBUIest::newRow("http2Config-8-5") << data8 << data3 << false;
    BOBUIest::newRow("http2Config-8-6") << data8 << data4 << false;
    BOBUIest::newRow("http2Config-8-7") << data8 << data5 << false;
    BOBUIest::newRow("http2Config-8-8") << data8 << data6 << false;
    BOBUIest::newRow("http2Config-8-9") << data8 << data7 << false;

    QNetworkRequest data9;
    data9.setDecompressedSafetyCheckThreshold(-1);
    BOBUIest::newRow("threshold-9-1") << data9 << QNetworkRequest() << false;
    BOBUIest::newRow("threshold-9-2") << data9 << data9 << true;
    BOBUIest::newRow("threshold-9-3") << data9 << data1 << false;
    BOBUIest::newRow("threshold-9-4") << data9 << data2 << false;
    BOBUIest::newRow("threshold-9-5") << data9 << data3 << false;
    BOBUIest::newRow("threshold-9-6") << data9 << data4 << false;
    BOBUIest::newRow("threshold-9-7") << data9 << data5 << false;
    BOBUIest::newRow("threshold-9-8") << data9 << data6 << false;
    BOBUIest::newRow("threshold-9-9") << data9 << data7 << false;
    BOBUIest::newRow("threshold-9-10") << data9 << data8 << false;
#endif

#if BOBUI_CONFIG(http) || defined (Q_OS_WASM)
    QNetworkRequest data10;
    data10.setTransferTimeout(50000);
    BOBUIest::newRow("timeout-10-1") << data10 << QNetworkRequest() << false;
    BOBUIest::newRow("timeout-10-2") << data10 << data10 << true;
    BOBUIest::newRow("timeout-10-3") << data10 << data1 << false;
    BOBUIest::newRow("timeout-10-4") << data10 << data2 << false;
    BOBUIest::newRow("timeout-10-5") << data10 << data3 << false;
    BOBUIest::newRow("timeout-10-6") << data10 << data4 << false;
    BOBUIest::newRow("timeout-10-7") << data10 << data5 << false;
    BOBUIest::newRow("timeout-10-8") << data10 << data6 << false;
    BOBUIest::newRow("timeout-10-9") << data10 << data7 << false;
    BOBUIest::newRow("timeout-10-10") << data10 << data8 << false;
    BOBUIest::newRow("timeout-10-11") << data10 << data9 << false;
#endif
}

// public bool operator==(const QNetworkRequest &other) const
void tst_QNetworkRequest::operatorEqual()
{
    QFETCH(QNetworkRequest, a);
    QFETCH(QNetworkRequest, b);
    QFETCH(bool, expectedToMatch);

    QCOMPARE(a == b, expectedToMatch);
}

BOBUIEST_MAIN(tst_QNetworkRequest)
#include "tst_qnetworkrequest.moc"
