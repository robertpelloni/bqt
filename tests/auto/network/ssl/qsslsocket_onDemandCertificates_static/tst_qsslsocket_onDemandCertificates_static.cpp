// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUITest/bobuiest.h>

#include <BobUINetwork/qsslsocket.h>
#include <BobUINetwork/qnetworkproxy.h>
#include <BobUINetwork/qauthenticator.h>

#include "private/qhostinfo_p.h"

#include "../../../network-settings.h"

#ifndef BOBUI_NO_OPENSSL
typedef QSharedPointer<QSslSocket> QSslSocketPtr;
#endif

class tst_QSslSocket_onDemandCertificates_static : public QObject
{
    Q_OBJECT

    int proxyAuthCalled;

public:

#ifndef BOBUI_NO_OPENSSL
    QSslSocketPtr newSocket();
#endif

public slots:
    void initTestCase_data();
    void initTestCase();
    void init();
    void cleanup();
    void proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *auth);

#ifndef BOBUI_NO_OPENSSL
private slots:
    void onDemandRootCertLoadingStaticMethods();

private:
    QSslSocket *socket;
#endif // BOBUI_NO_OPENSSL
};

enum ProxyTests {
    NoProxy = 0x00,
    Socks5Proxy = 0x01,
    HttpProxy = 0x02,
    TypeMask = 0x0f,

    NoAuth = 0x00,
    AuthBasic = 0x10,
    AuthNtlm = 0x20,
    AuthMask = 0xf0
};

void tst_QSslSocket_onDemandCertificates_static::initTestCase_data()
{
    BOBUIest::addColumn<bool>("setProxy");
    BOBUIest::addColumn<int>("proxyType");

    BOBUIest::newRow("WithoutProxy") << false << 0;
    BOBUIest::newRow("WithSocks5Proxy") << true << int(Socks5Proxy);
    BOBUIest::newRow("WithSocks5ProxyAuth") << true << int(Socks5Proxy | AuthBasic);

    BOBUIest::newRow("WithHttpProxy") << true << int(HttpProxy);
    BOBUIest::newRow("WithHttpProxyBasicAuth") << true << int(HttpProxy | AuthBasic);
    // uncomment the line below when NTLM works
//    BOBUIest::newRow("WithHttpProxyNtlmAuth") << true << int(HttpProxy | AuthNtlm);
}

void tst_QSslSocket_onDemandCertificates_static::initTestCase()
{
#ifdef BOBUI_TEST_SERVER
    QVERIFY(BobUINetworkSettings::verifyConnection(BobUINetworkSettings::socksProxyServerName(), 1080));
    QVERIFY(BobUINetworkSettings::verifyConnection(BobUINetworkSettings::socksProxyServerName(), 1081));
    QVERIFY(BobUINetworkSettings::verifyConnection(BobUINetworkSettings::httpProxyServerName(), 3128));
    QVERIFY(BobUINetworkSettings::verifyConnection(BobUINetworkSettings::httpProxyServerName(), 3129));
    QVERIFY(BobUINetworkSettings::verifyConnection(BobUINetworkSettings::httpProxyServerName(), 3130));
#else
    if (!BobUINetworkSettings::verifyTestNetworkSettings())
        QSKIP("No network test server available");
#endif // BOBUI_TEST_SERVER
}

void tst_QSslSocket_onDemandCertificates_static::init()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy) {
        QFETCH_GLOBAL(int, proxyType);
        const auto socksAddr = BobUINetworkSettings::socksProxyServerIp().toString();
        const auto squidAddr = BobUINetworkSettings::httpProxyServerIp().toString();

        QNetworkProxy proxy;

        switch (proxyType) {
        case Socks5Proxy:
            proxy = QNetworkProxy(QNetworkProxy::Socks5Proxy, socksAddr, 1080);
            break;

        case Socks5Proxy | AuthBasic:
            proxy = QNetworkProxy(QNetworkProxy::Socks5Proxy, socksAddr, 1081);
            break;

        case HttpProxy | NoAuth:
            proxy = QNetworkProxy(QNetworkProxy::HttpProxy, squidAddr, 3128);
            break;

        case HttpProxy | AuthBasic:
            proxy = QNetworkProxy(QNetworkProxy::HttpProxy, squidAddr, 3129);
            break;

        case HttpProxy | AuthNtlm:
            proxy = QNetworkProxy(QNetworkProxy::HttpProxy, squidAddr, 3130);
            break;
        }
        QNetworkProxy::setApplicationProxy(proxy);
    }

    bobui_qhostinfo_clear_cache();
}

void tst_QSslSocket_onDemandCertificates_static::cleanup()
{
    QNetworkProxy::setApplicationProxy(QNetworkProxy::DefaultProxy);
}

#ifndef BOBUI_NO_OPENSSL
QSslSocketPtr tst_QSslSocket_onDemandCertificates_static::newSocket()
{
    QSslSocket *socket = new QSslSocket;

    proxyAuthCalled = 0;
    connect(socket, SIGNAL(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)),
            SLOT(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)),
            BobUI::DirectConnection);

    return QSslSocketPtr(socket);
}
#endif

void tst_QSslSocket_onDemandCertificates_static::proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *auth)
{
    ++proxyAuthCalled;
    auth->setUser("qsockstest");
    auth->setPassword("password");
}

#ifndef BOBUI_NO_OPENSSL

void tst_QSslSocket_onDemandCertificates_static::onDemandRootCertLoadingStaticMethods()
{
    QString host("www.bobui.io");

    // setting empty default configuration -> should not work
    QSslConfiguration conf;
    QSslConfiguration originalDefaultConf = QSslConfiguration::defaultConfiguration();
    QSslConfiguration::setDefaultConfiguration(conf);
    QSslSocketPtr socket4 = newSocket();
    this->socket = socket4.data();
    socket4->connectToHostEncrypted(host, 443);
    QVERIFY(!socket4->waitForEncrypted(4000));
    QSslConfiguration::setDefaultConfiguration(originalDefaultConf); // restore old behaviour for run with proxies etc.
}

#endif // BOBUI_NO_OPENSSL

BOBUIEST_MAIN(tst_QSslSocket_onDemandCertificates_static)
#include "tst_qsslsocket_onDemandCertificates_static.moc"
