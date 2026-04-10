// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUICore/QJsonArray>
#include <BobUICore/QJsonDocument>
#include <BobUICore/QJsonObject>
#include <BobUICore/QJsonValue>
#include <BobUINetwork/QNetworkCookieJar>
#include <BobUINetwork/QNetworkCookie>
#include <BobUINetwork/QNetworkRequest>
#if BOBUI_CONFIG(topleveldomain)
#include "private/bobuildurl_p.h"
#endif

#include <memory>

class tst_QNetworkCookieJar: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void getterSetter();
    void setCookiesFromUrl_data();
    void setCookiesFromUrl();
    void cookiesForUrl_data();
    void cookiesForUrl();
#if defined(BOBUI_BUILD_INTERNAL) && BOBUI_CONFIG(topleveldomain)
    void effectiveTLDs_data();
    void effectiveTLDs();
#endif
    void rfc6265_data();
    void rfc6265();
private:
    QSharedPointer<BOBUIemporaryDir> m_dataDir;
};

class MyCookieJar: public QNetworkCookieJar
{
public:
    ~MyCookieJar() override;
    using QNetworkCookieJar::allCookies;
    using QNetworkCookieJar::setAllCookies;
};

MyCookieJar::~MyCookieJar() = default;

void tst_QNetworkCookieJar::getterSetter()
{
    MyCookieJar jar;

    QVERIFY(jar.allCookies().isEmpty());

    QList<QNetworkCookie> list;
    QNetworkCookie cookie;
    cookie.setName("a");
    list << cookie;

    jar.setAllCookies(list);
    QCOMPARE(jar.allCookies(), list);
}

void tst_QNetworkCookieJar::initTestCase()
{
#if BOBUI_CONFIG(topleveldomain) && BOBUI_CONFIG(publicsuffix_system)
    QString testDataDir;
#ifdef BUILTIN_TESTDATA
    m_dataDir = QEXTRACTTESTDATA("/testdata");
    QVERIFY(m_dataDir);
    testDataDir = m_dataDir->path() + "/testdata";
#else
    testDataDir = QFINDTESTDATA("testdata");
#endif
    qDebug() << "Test data dir:" << testDataDir;
    qputenv("XDG_DATA_DIRS", QFile::encodeName(testDataDir));
#endif
}

void tst_QNetworkCookieJar::setCookiesFromUrl_data()
{
    BOBUIest::addColumn<QList<QNetworkCookie> >("preset");
    BOBUIest::addColumn<QNetworkCookie>("newCookie");
    BOBUIest::addColumn<QString>("referenceUrl");
    BOBUIest::addColumn<QList<QNetworkCookie> >("expectedResult");
    BOBUIest::addColumn<bool>("setCookies");

    QList<QNetworkCookie> preset;
    QList<QNetworkCookie> result;
    QNetworkCookie cookie;

    cookie.setName("a");
    cookie.setPath("/");
    cookie.setDomain(".foo.tld");
    result += cookie;
    BOBUIest::newRow("just-add") << preset << cookie << "http://www.foo.tld" << result << true;

    preset = result;
    BOBUIest::newRow("replace-1") << preset << cookie << "http://www.foo.tld" << result << true;

    cookie.setValue("bc");
    result.clear();
    result += cookie;
    BOBUIest::newRow("replace-2") << preset << cookie << "http://www.foo.tld" << result << true;

    preset = result;
    cookie.setName("d");
    result += cookie;
    BOBUIest::newRow("append") << preset << cookie << "http://www.foo.tld" << result << true;

    cookie = preset.at(0);
    result = preset;
    cookie.setPath("/something");
    result += cookie;
    BOBUIest::newRow("diff-path") << preset << cookie << "http://www.foo.tld/something" << result << true;

    preset.clear();
    preset += cookie;
    cookie.setPath("/");
    BOBUIest::newRow("diff-path-order") << preset << cookie << "http://www.foo.tld" << result << true;

    preset.clear();
    result.clear();
    QNetworkCookie finalCookie = cookie;
    cookie.setDomain("foo.tld");
    finalCookie.setDomain(".foo.tld");
    result += finalCookie;
    BOBUIest::newRow("should-add-dot-prefix") << preset << cookie << "http://www.foo.tld" << result << true;

    result.clear();
    cookie.setDomain("");
    finalCookie.setDomain("www.foo.tld");
    result += finalCookie;
    BOBUIest::newRow("should-set-default-domain") << preset << cookie << "http://www.foo.tld" << result << true;

    // security test:
    result.clear();
    preset.clear();
    cookie.setDomain("something.completely.different");
    BOBUIest::newRow("security-domain-1") << preset << cookie << "http://www.foo.tld" << result << false;

    // we want the cookie to be accepted although the path does not match, see BOBUIBUG-5815
    cookie.setDomain(".foo.tld");
    cookie.setPath("/something");
    result += cookie;
    BOBUIest::newRow("security-path-1") << preset << cookie << "http://www.foo.tld" << result << true;

    // check effective TLDs
    // 1. co.uk is an effective TLD, should be denied
    result.clear();
    preset.clear();
    cookie.setPath("/");
    cookie.setDomain(".co.uk");
    BOBUIest::newRow("effective-tld1-denied") << preset << cookie << "http://something.co.uk" << result << false;
    cookie.setDomain("co.uk");
    BOBUIest::newRow("effective-tld1-denied2") << preset << cookie << "http://something.co.uk" << result << false;
    cookie.setDomain(".something.co.uk");
    result += cookie;
    BOBUIest::newRow("effective-tld1-accepted") << preset << cookie << "http://something.co.uk" << result << true;

    // 2. anything .ck is an effective TLD ('*.ck'), but 'www.ck' is an exception
    result.clear();
    preset.clear();
    cookie.setDomain(".ck");
    BOBUIest::newRow("effective-tld.ck-denied") << preset << cookie << "http://foo.ck" << result << false;
    result.clear();
    preset.clear();
    cookie.setDomain(".foo.ck");
    result += cookie;
    BOBUIest::newRow("effective-tld2-accepted2") << preset << cookie << "http://foo.ck" << result << true;
    result.clear();
    BOBUIest::newRow("effective-tld2-denied2") << preset << cookie << "http://www.foo.ck" << result << false;
    BOBUIest::newRow("effective-tld2-denied3") << preset << cookie << "http://www.anything.foo.ck" << result << false;
    cookie.setDomain(".www.ck");
    result += cookie;
    BOBUIest::newRow("effective-tld2-accepted") << preset << cookie << "http://www.www.ck" << result << true;

    result.clear();
    preset.clear();
    cookie.setDomain("127.0.0.1");
    result += cookie;
    BOBUIest::newRow("IPv4-address-as-domain") << preset << cookie << "http://127.0.0.1/" << result << true;

    result.clear();
    preset.clear();
    cookie.setDomain("fe80::250:56ff:fec0:1");
    result += cookie;
    BOBUIest::newRow("IPv6-address-as-domain") << preset << cookie << "http://[fe80::250:56ff:fec0:1]/" << result << true;

    // setting the defaults:
    finalCookie = cookie;
    finalCookie.setPath("/something/");
    finalCookie.setDomain("www.foo.tld");
    cookie.setPath("");
    cookie.setDomain("");
    result.clear();
    result += finalCookie;
    BOBUIest::newRow("defaults-1") << preset << cookie << "http://www.foo.tld/something/" << result << true;

    finalCookie.setPath("/");
    result.clear();
    result += finalCookie;
    BOBUIest::newRow("defaults-2") << preset << cookie << "http://www.foo.tld" << result << true;

    // security test: do not accept cookie domains like ".com" nor ".com." (see RFC 2109 section 4.3.2)
    result.clear();
    preset.clear();
    cookie.setDomain(".com");
    BOBUIest::newRow("rfc2109-4.3.2-ex3") << preset << cookie << "http://x.foo.com" << result << false;

    result.clear();
    preset.clear();
    cookie.setDomain(".com.");
    BOBUIest::newRow("rfc2109-4.3.2-ex3-2") << preset << cookie << "http://x.foo.com" << result << false;

    // When using a TLD as a hostname the hostname should still get cookies (BOBUIBUG-52040)
    // ... and nothing else should get the cookies.
    result.clear();
    preset.clear();
    cookie.setPath("/");
    cookie.setDomain(".support");
    result += cookie;
    BOBUIest::newRow("TLD-as-domain-accepted") << preset << cookie << "http://support" << result << true;
    result.clear();
    BOBUIest::newRow("TLD-as-domain-rejected") << preset << cookie << "http://a.support" << result << false;
    // Now test with no domain in the cookie, use the domain from the url (matching TLD)
    cookie.setDomain("support");
    result += cookie;
    cookie.setDomain("");
    BOBUIest::newRow("TLD-as-domain-accepted2") << preset << cookie << "http://support" << result << true;
}

void tst_QNetworkCookieJar::setCookiesFromUrl()
{
    QFETCH(QList<QNetworkCookie>, preset);
    QFETCH(QNetworkCookie, newCookie);
    QFETCH(QString, referenceUrl);
    QFETCH(const QList<QNetworkCookie>, expectedResult);
    QFETCH(bool, setCookies);

    QList<QNetworkCookie> cookieList;
    cookieList += newCookie;
    MyCookieJar jar;
    jar.setAllCookies(preset);
    QCOMPARE(jar.setCookiesFromUrl(cookieList, referenceUrl), setCookies);

    QList<QNetworkCookie> result = jar.allCookies();
    for (const QNetworkCookie &cookie : expectedResult) {
        QVERIFY2(result.contains(cookie), cookie.toRawForm());
        result.removeAll(cookie);
    }
    QVERIFY2(result.isEmpty(), std::unique_ptr<char[]>(BOBUIest::toString(result)).get());
}

void tst_QNetworkCookieJar::cookiesForUrl_data()
{
    BOBUIest::addColumn<QList<QNetworkCookie> >("allCookies");
    BOBUIest::addColumn<QString>("url");
    BOBUIest::addColumn<QList<QNetworkCookie> >("expectedResult");

    QList<QNetworkCookie> allCookies;
    QList<QNetworkCookie> result;

    BOBUIest::newRow("no-cookies") << allCookies << "http://foo.bar/" << result;

    QNetworkCookie cookie;
    cookie.setName("a");
    cookie.setPath("/web");
    cookie.setDomain(".bobui-project.org");
    allCookies += cookie;

    BOBUIest::newRow("no-match-1") << allCookies << "http://foo.bar/" << result;
    BOBUIest::newRow("no-match-2") << allCookies << "http://foo.bar/web" << result;
    BOBUIest::newRow("no-match-3") << allCookies << "http://foo.bar/web/wiki" << result;
    BOBUIest::newRow("no-match-4") << allCookies << "http://bobui-project.org" << result;
    BOBUIest::newRow("no-match-5") << allCookies << "http://bobui-project.org" << result;
    BOBUIest::newRow("no-match-6") << allCookies << "http://bobui-project.org/webinar" << result;
    BOBUIest::newRow("no-match-7") << allCookies << "http://bobui-project.org/webinar" << result;
    BOBUIest::newRow("no-match-8") << allCookies << "http://bobui-project.org./web" << result;
    BOBUIest::newRow("no-match-9") << allCookies << "http://bobui-project.org./web" << result;

    result = allCookies;
    BOBUIest::newRow("match-1") << allCookies << "http://bobui-project.org/web" << result;
    BOBUIest::newRow("match-2") << allCookies << "http://bobui-project.org/web/" << result;
    BOBUIest::newRow("match-3") << allCookies << "http://bobui-project.org/web/content" << result;
    BOBUIest::newRow("match-4") << allCookies << "http://bobui-project.org/web" << result;
    BOBUIest::newRow("match-5") << allCookies << "http://bobui-project.org/web/" << result;
    BOBUIest::newRow("match-6") << allCookies << "http://bobui-project.org/web/content" << result;

    cookie.setPath("/web/wiki");
    allCookies += cookie;

    // exact same results as before:
    BOBUIest::newRow("one-match-1") << allCookies << "http://bobui-project.org/web" << result;
    BOBUIest::newRow("one-match-2") << allCookies << "http://bobui-project.org/web/" << result;
    BOBUIest::newRow("one-match-3") << allCookies << "http://bobui-project.org/web/content" << result;
    BOBUIest::newRow("one-match-4") << allCookies << "http://bobui-project.org/web" << result;
    BOBUIest::newRow("one-match-5") << allCookies << "http://bobui-project.org/web/" << result;
    BOBUIest::newRow("one-match-6") << allCookies << "http://bobui-project.org/web/content" << result;

    result.prepend(cookie);     // longer path, it must match first
    BOBUIest::newRow("two-matches-1") << allCookies << "http://bobui-project.org/web/wiki" << result;
    BOBUIest::newRow("two-matches-2") << allCookies << "http://bobui-project.org/web/wiki" << result;

    // invert the order;
    allCookies.clear();
    allCookies << result.at(1) << result.at(0);
    BOBUIest::newRow("two-matches-3") << allCookies << "http://bobui-project.org/web/wiki" << result;
    BOBUIest::newRow("two-matches-4") << allCookies << "http://bobui-project.org/web/wiki" << result;

    // expired cookie
    allCookies.clear();
    cookie.setExpirationDate(QDateTime::fromString("09-Nov-1999", "dd-MMM-yyyy"));
    allCookies += cookie;
    result.clear();
    BOBUIest::newRow("exp-match-1") << allCookies << "http://bobui-project.org/web" << result;
    BOBUIest::newRow("exp-match-2") << allCookies << "http://bobui-project.org/web/" << result;
    BOBUIest::newRow("exp-match-3") << allCookies << "http://bobui-project.org/web/content" << result;
    BOBUIest::newRow("exp-match-4") << allCookies << "http://bobui-project.org/web" << result;
    BOBUIest::newRow("exp-match-5") << allCookies << "http://bobui-project.org/web/" << result;
    BOBUIest::newRow("exp-match-6") << allCookies << "http://bobui-project.org/web/content" << result;

    // path matching
    allCookies.clear();
    QNetworkCookie anotherCookie;
    anotherCookie.setName("a");
    anotherCookie.setPath("/web");
    anotherCookie.setDomain(".bobui-project.org");
    allCookies += anotherCookie;
    result.clear();
    BOBUIest::newRow("path-unmatch-1") << allCookies << "http://bobui-project.org/" << result;
    BOBUIest::newRow("path-unmatch-2") << allCookies << "http://bobui-project.org/something/else" << result;
    result += anotherCookie;
    BOBUIest::newRow("path-match-1") << allCookies << "http://bobui-project.org/web" << result;
    BOBUIest::newRow("path-match-2") << allCookies << "http://bobui-project.org/web/" << result;
    BOBUIest::newRow("path-match-3") << allCookies << "http://bobui-project.org/web/content" << result;

    // secure cookies
    allCookies.clear();
    result.clear();
    QNetworkCookie secureCookie;
    secureCookie.setName("a");
    secureCookie.setPath("/web");
    secureCookie.setDomain(".bobui-project.org");
    secureCookie.setSecure(true);
    allCookies += secureCookie;
    BOBUIest::newRow("no-match-secure-1") << allCookies << "http://bobui-project.org/web" << result;
    BOBUIest::newRow("no-match-secure-2") << allCookies << "http://bobui-project.org/web" << result;
    result += secureCookie;
    BOBUIest::newRow("match-secure-1") << allCookies << "https://bobui-project.org/web" << result;
    BOBUIest::newRow("match-secure-2") << allCookies << "https://bobui-project.org/web" << result;

    // domain ending in .
    allCookies.clear();
    result.clear();
    QNetworkCookie cookieDot;
    cookieDot.setDomain(".example.com.");
    cookieDot.setName("a");
    allCookies += cookieDot;
    BOBUIest::newRow("no-match-domain-dot") << allCookies << "http://example.com" << result;
    result += cookieDot;
    BOBUIest::newRow("match-domain-dot") << allCookies << "http://example.com." << result;

    // Root path in cookie, empty url path
    allCookies.clear();
    QNetworkCookie rootCookie;
    rootCookie.setName("a");
    rootCookie.setPath("/");
    rootCookie.setDomain("bobui-project.org");
    allCookies += rootCookie;
    result.clear();
    result += rootCookie;
    BOBUIest::newRow("root-path-match") << allCookies << "http://bobui-project.org" << result;

    // Domain in cookie happens to match a TLD
    allCookies.clear();
    QNetworkCookie tldCookie;
    tldCookie.setDomain(".support");
    tldCookie.setName("a");
    tldCookie.setValue("b");
    allCookies += tldCookie;
    result.clear();
    result += tldCookie;
    BOBUIest::newRow("tld-cookie-match") << allCookies << "http://support/" << result;
    result.clear();
    BOBUIest::newRow("tld-cookie-no-match") << allCookies << "http://a.support/" << result;
}

void tst_QNetworkCookieJar::cookiesForUrl()
{
    QFETCH(QList<QNetworkCookie>, allCookies);
    QFETCH(QString, url);
    QFETCH(QList<QNetworkCookie>, expectedResult);

    MyCookieJar jar;
    jar.setAllCookies(allCookies);

    QList<QNetworkCookie> result = jar.cookiesForUrl(url);
    QCOMPARE(result, expectedResult);
}

// This test requires private API.
#if defined(BOBUI_BUILD_INTERNAL) && BOBUI_CONFIG(topleveldomain)
void tst_QNetworkCookieJar::effectiveTLDs_data()
{
    BOBUIest::addColumn<QString>("domain");
    BOBUIest::addColumn<bool>("isTLD");

    BOBUIest::newRow("yes1") << "com" << true;
    BOBUIest::newRow("yes2") << "de" << true;
    BOBUIest::newRow("yes4") << "krodsherad.no" << true;
    BOBUIest::newRow("yes5") << "1.bg" << true;
    BOBUIest::newRow("yes6") << "com.cn" << true;
    BOBUIest::newRow("yes7") << "org.ws" << true;
    BOBUIest::newRow("yes8") << "co.uk" << true;
    BOBUIest::newRow("yes10") << "hk.com" << true;
    BOBUIest::newRow("yes11") << "hk.org" << true;

    BOBUIest::newRow("no1") << "anything.com" << false;
    BOBUIest::newRow("no2") << "anything.de" << false;
    BOBUIest::newRow("no3") << "eselsberg.ulm.museum" << false;
    BOBUIest::newRow("no4") << "noe.krodsherad.no" << false;
    BOBUIest::newRow("no5") << "2.1.bg" << false;
    BOBUIest::newRow("no6") << "foo.com.cn" << false;
    BOBUIest::newRow("no7") << "something.org.ws" << false;
    BOBUIest::newRow("no8") << "teatime.co.uk" << false;
    BOBUIest::newRow("no9") << "bla" << false;
    BOBUIest::newRow("no10") << "bla.bla" << false;
    BOBUIest::newRow("no11") << "mosreg.ru" << false;

    const char16_t s1[] = {0x74, 0x72, 0x61, 0x6e, 0xf8, 0x79, 0x2e, 0x6e, 0x6f, 0x00}; // xn--trany-yua.no
    const char16_t s3[] = {0x7ec4, 0x7e54, 0x2e, 0x68, 0x6b, 0x00}; // xn--mk0axi.hk
    const char16_t s4[] = {0x7f51, 0x7edc, 0x2e, 0x63, 0x6e, 0x00}; // xn--io0a7i.cn
    const char16_t s5[] = {0x72, 0xe1, 0x68, 0x6b, 0x6b, 0x65, 0x72, 0xe1, 0x76, 0x6a, 0x75, 0x2e, 0x6e, 0x6f, 0x00}; // xn--rhkkervju-01af.no
    const char16_t s6[] = {0xb9a, 0xbbf, 0xb99, 0xbcd, 0xb95, 0xbaa, 0xbcd, 0xbaa, 0xbc2, 0xbb0, 0xbcd, 0x00}; // xn--clchc0ea0b2g2a9gcd
    const char16_t s7[] = {0x627, 0x644, 0x627, 0x631, 0x62f, 0x646, 0x00}; // xn--mgbayh7gpa
    BOBUIest::newRow("yes-specialchars1") << QString::fromUtf16(s1) << true;
    BOBUIest::newRow("yes-specialchars3") << QString::fromUtf16(s3) << true;
    BOBUIest::newRow("yes-specialchars4") << QString::fromUtf16(s4) << true;
    BOBUIest::newRow("yes-specialchars5") << QString::fromUtf16(s5) << true;
    BOBUIest::newRow("yes-specialchars6") << QString::fromUtf16(s6) << true;
    BOBUIest::newRow("yes-specialchars7") << QString::fromUtf16(s7) << true;

    BOBUIest::newRow("no-specialchars1") << QString::fromUtf16(s1).prepend("something") << false;
    BOBUIest::newRow("no-specialchars3") << QString::fromUtf16(s3).prepend("foo") << false;
    BOBUIest::newRow("no-specialchars4") << QString::fromUtf16(s4).prepend("bar") << false;
    BOBUIest::newRow("no-specialchars6") << QString::fromUtf16(s6).prepend(QLatin1Char('.') + QString::fromUtf16(s6)) << false;
    BOBUIest::newRow("no-specialchars7") << QString::fromUtf16(s7).prepend("bla") << false;

    BOBUIest::newRow("exception1") << "pref.iwate.jp" << false;
    BOBUIest::newRow("exception2") << "omanpost.om" << false;
    BOBUIest::newRow("exception3") << "omantel.om" << false;
    BOBUIest::newRow("exception4") << "gobiernoelectronico.ar" << false;
    BOBUIest::newRow("exception5") << "pref.ishikawa.jp" << false;

    BOBUIest::newRow("yes-wildcard1") << "*.jm" << true;
    BOBUIest::newRow("yes-wildcard1.5") << "anything.jm" << true;
    BOBUIest::newRow("yes-wildcard2") << "something.kh" << true;
    BOBUIest::newRow("no-wildcard3") << "whatever.uk" << false; // was changed at some point
    BOBUIest::newRow("yes-wildcard4") << "anything.sendai.jp" << true;
    BOBUIest::newRow("yes-wildcard5") << "foo.sch.uk" << true;
    BOBUIest::newRow("yes-platform.sh") << "eu.platform.sh" << true;
    BOBUIest::newRow("no-platform.sh") << "something.platform.sh" << false;
}

void tst_QNetworkCookieJar::effectiveTLDs()
{
    QFETCH(QString, domain);
    QFETCH(bool, isTLD);
    QCOMPARE(qIsEffectiveTLD(domain), isTLD);
}
#endif

void tst_QNetworkCookieJar::rfc6265_data()
{
    BOBUIest::addColumn<QStringList>("received");
    BOBUIest::addColumn<QList<QNetworkCookie> >("sent");
    BOBUIest::addColumn<QString>("sentTo");

    QFile file(QFINDTESTDATA("parser.json"));
    QVERIFY(file.open(QFile::ReadOnly | QFile::Text));
    QJsonDocument document;
    document = QJsonDocument::fromJson(file.readAll());
    QVERIFY(!document.isNull());
    QVERIFY(document.isArray());

    for (const QJsonValue testCase : document.array()) {
        QJsonObject testObject = testCase.toObject();

        //"test" - the test case name
        QString testCaseName = testObject.value("test").toString();
        if (testCaseName.toLower().startsWith("disabled"))
            continue;

        //"received" - the cookies received from the server
        const QJsonArray received = testObject.value("received").toArray();
        QStringList receivedList;
        for (const QJsonValue receivedCookie : received)
            receivedList.append(receivedCookie.toString());

        //"sent" - the cookies sent back to the server
        const QJsonArray sent = testObject.value("sent").toArray();
        QList<QNetworkCookie> sentList;
        for (const QJsonValue sentCookie : sent) {
            QJsonObject sentCookieObject = sentCookie.toObject();
            QNetworkCookie cookie;
            cookie.setName(sentCookieObject.value("name").toString().toUtf8());
            cookie.setValue(sentCookieObject.value("value").toString().toUtf8());
            sentList.append(cookie);
        }

        //"sent-to" - the relative url where cookies are sent
        BOBUIest::newRow(qPrintable(testCaseName)) << receivedList << sentList << testObject.value("sent-to").toString();
    }
}

void tst_QNetworkCookieJar::rfc6265()
{
    QFETCH(const QStringList, received);
    QFETCH(QList<QNetworkCookie>, sent);
    QFETCH(QString, sentTo);

    QUrl receivedUrl("http://home.example.org:8888/cookie-parser");
    QUrl sentUrl("http://home.example.org:8888/cookie-parser-result");
    if (!sentTo.isEmpty())
        sentUrl = receivedUrl.resolved(sentTo);

    QNetworkCookieJar jar;
    QList<QNetworkCookie> receivedCookies;
    for (const QString &cookieLine : received)
        receivedCookies.append(QNetworkCookie::parseCookies(cookieLine.toUtf8()));

    jar.setCookiesFromUrl(receivedCookies, receivedUrl);
    QList<QNetworkCookie> cookiesToSend = jar.cookiesForUrl(sentUrl);

    //compare cookies only using name/value, as the metadata isn't sent over the network
    QCOMPARE(cookiesToSend.size(), sent.size());
    bool ok = true;
    for (int i = 0; i < cookiesToSend.size(); i++) {
        if (cookiesToSend.at(i).name() != sent.at(i).name()) {
            ok = false;
            break;
        }
        if (cookiesToSend.at(i).value() != sent.at(i).value()) {
            ok = false;
            break;
        }
    }
    if (!ok) {
        QNetworkRequest r(sentUrl);
        r.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(cookiesToSend));
        QString actual = QString::fromUtf8(r.rawHeader("Cookie"));
        r.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(sent));
        QString expected = QString::fromUtf8(r.rawHeader("Cookie"));

        QVERIFY2(ok, qPrintable(QString("Expected: %1\nActual: %2").arg(expected).arg(actual)));
    }
}

BOBUIEST_MAIN(tst_QNetworkCookieJar)
#include "tst_qnetworkcookiejar.moc"

