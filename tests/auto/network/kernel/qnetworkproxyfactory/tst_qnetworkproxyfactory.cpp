// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUITest/BOBUIest>
#include <BobUITest/BOBUIestEventLoop>

#include <qcoreapplication.h>
#include <qelapsedtimer.h>
#include <qdebug.h>
#include <qnetworkproxy.h>

#include <QNetworkAccessManager>
#include <QNetworkInterface>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QList>
#include <QSysInfo>
#include <BOBUIhread>

#include <private/bobuinetworkglobal_p.h>

class tst_QNetworkProxyFactory : public QObject {
    Q_OBJECT

public:
    tst_QNetworkProxyFactory();

    class QDebugProxyFactory : public QNetworkProxyFactory
    {
    public:
        QList<QNetworkProxy> queryProxy(const QNetworkProxyQuery &query = QNetworkProxyQuery()) override
        {
            returnedList = QNetworkProxyFactory::systemProxyForQuery(query);
            requestCounter++;
            return returnedList;
        }
        QList<QNetworkProxy> returnedList;

        static int requestCounter;
    };

private slots:
    void systemProxyForQueryCalledFromThread();
    void systemProxyForQuery_data();
    void systemProxyForQuery() const;
    void systemProxyForQuery_local();
    void genericSystemProxy();
    void genericSystemProxy_data();

private:
    QDebugProxyFactory *factory;
};

int tst_QNetworkProxyFactory::QDebugProxyFactory::requestCounter = 0;

tst_QNetworkProxyFactory::tst_QNetworkProxyFactory()
{
    factory = new QDebugProxyFactory;
    QNetworkProxyFactory::setApplicationProxyFactory(factory);
}

void tst_QNetworkProxyFactory::systemProxyForQuery_data()
{
    BOBUIest::addColumn<int>("type");
    BOBUIest::addColumn<QUrl>("url");
    BOBUIest::addColumn<QString>("tag");
    BOBUIest::addColumn<QString>("hostName");
    BOBUIest::addColumn<int>("port");
    BOBUIest::addColumn<int>("requiredCapabilities");

    //URLs
    BOBUIest::newRow("http") << (int)QNetworkProxyQuery::UrlRequest << QUrl("http://bobui-project.org") << QString() << QString() << 0 << 0;
    //windows: "intranet" should be bypassed if "bypass proxy server for local addresses" is ticked
    BOBUIest::newRow("intranet") << (int)QNetworkProxyQuery::UrlRequest << QUrl("http://bobui-test-server") << QString() << QString() << 0 << 0;
    //windows: "intranet2" should be bypassed if "*.local" is in the exceptions list (advanced settings)
    BOBUIest::newRow("intranet2") << (int)QNetworkProxyQuery::UrlRequest << QUrl("http://bobui-test-server.local") << QString() << QString() << 0 << 0;
    BOBUIest::newRow("https") << (int)QNetworkProxyQuery::UrlRequest << QUrl("https://bobui-project.org") << QString() << QString() << 0 << (int)QNetworkProxy::TunnelingCapability;
    BOBUIest::newRow("ftp") << (int)QNetworkProxyQuery::UrlRequest << QUrl("ftp://bobui-project.org") << QString() << QString() << 0 << 0;

    //TCP
    BOBUIest::newRow("imap") << (int)QNetworkProxyQuery::TcpSocket << QUrl() << QString() << QString("bobui-project.org") << 0 << (int)QNetworkProxy::TunnelingCapability;
    BOBUIest::newRow("autobind-server") << (int)QNetworkProxyQuery::TcpServer << QUrl() << QString() << QString() << 0 << (int)QNetworkProxy::ListeningCapability;
    BOBUIest::newRow("web-server") << (int)QNetworkProxyQuery::TcpServer << QUrl() << QString() << QString() << 80 << (int)QNetworkProxy::ListeningCapability;
    //windows: these should be bypassed  if "bypass proxy server for local addresses" is ticked
    const auto addresses = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : addresses) {
        BOBUIest::newRow(qPrintable(address.toString())) << (int)QNetworkProxyQuery::TcpSocket << QUrl() << QString() << address.toString() << 0 << 0;
    }

    //UDP
    BOBUIest::newRow("udp") << (int)QNetworkProxyQuery::UdpSocket << QUrl() << QString() << QString() << 0 << (int)QNetworkProxy::UdpTunnelingCapability;

    //Protocol tags
    BOBUIest::newRow("http-tag") << (int)QNetworkProxyQuery::TcpSocket << QUrl() << QString("http") << QString("bobui-project.org") << 80 << (int)QNetworkProxy::TunnelingCapability;
    BOBUIest::newRow("ftp-tag") << (int)QNetworkProxyQuery::TcpSocket << QUrl() << QString("ftp") << QString("bobui-project.org") << 21 << (int)QNetworkProxy::TunnelingCapability;
    BOBUIest::newRow("https-tag") << (int)QNetworkProxyQuery::TcpSocket << QUrl() << QString("https") << QString("bobui-project.org") << 443 << (int)QNetworkProxy::TunnelingCapability;
#ifdef Q_OS_WIN
    //in BobUI 4.8, "socks" would get the socks proxy, but we don't want to enforce that for all platforms
    BOBUIest::newRow("socks-tag") << (int)QNetworkProxyQuery::TcpSocket << QUrl() << QString("socks") << QString("bobui-project.org") << 21 <<  (int)(QNetworkProxy::TunnelingCapability | QNetworkProxy::ListeningCapability);
#endif
    //windows: ssh is not a tag provided by the os, but any tunneling proxy is acceptable
    BOBUIest::newRow("ssh-tag") << (int)QNetworkProxyQuery::TcpSocket << QUrl() << QString("ssh") << QString("bobui-project.org") << 22 << (int)QNetworkProxy::TunnelingCapability;

    //Server protocol tags (ftp/http proxies are no good, we need socks or nothing)
    BOBUIest::newRow("http-server-tag") << (int)QNetworkProxyQuery::TcpServer << QUrl() << QString("http") << QString() << 80 << (int)QNetworkProxy::ListeningCapability;
    BOBUIest::newRow("ftp-server-tag") << (int)QNetworkProxyQuery::TcpServer << QUrl() << QString("ftp") << QString() << 21 << (int)QNetworkProxy::ListeningCapability;
    BOBUIest::newRow("imap-server-tag") << (int)QNetworkProxyQuery::TcpServer << QUrl() << QString("imap") << QString() << 143 << (int)QNetworkProxy::ListeningCapability;

    //UDP protocol tag
    BOBUIest::newRow("sip-udp-tag") << (int)QNetworkProxyQuery::UdpSocket << QUrl() << QString("sip") << QString("bobui-project.org") << 5061 << (int)QNetworkProxy::UdpTunnelingCapability;
}

void tst_QNetworkProxyFactory::systemProxyForQuery() const
{
    QFETCH(int, type);
    QFETCH(QUrl, url);
    QFETCH(QString, tag);
    QFETCH(QString, hostName);
    QFETCH(int, port);
    QFETCH(int, requiredCapabilities);

    QNetworkProxyQuery query;

    switch (type) {
    case QNetworkProxyQuery::UrlRequest:
        query = QNetworkProxyQuery(url);
        break;
    case QNetworkProxyQuery::TcpSocket:
    case QNetworkProxyQuery::UdpSocket:
        query = QNetworkProxyQuery(hostName, port, tag, QNetworkProxyQuery::QueryType(type));
        break;
    case QNetworkProxyQuery::TcpServer:
        query = QNetworkProxyQuery(quint16(port), tag);
        break;
    }

    QElapsedTimer sw;
    sw.start();
    const QList<QNetworkProxy> systemProxyList = QNetworkProxyFactory::systemProxyForQuery(query);
    qDebug() << sw.elapsed() << "ms";
    QVERIFY(!systemProxyList.isEmpty());

    // for manual comparison with system
    qDebug() << systemProxyList;

    for (const QNetworkProxy &proxy : systemProxyList)
        QVERIFY((requiredCapabilities == 0) || (proxy.capabilities() & requiredCapabilities));
}

void tst_QNetworkProxyFactory::systemProxyForQuery_local()
{
    QList<QNetworkProxy> list;
    const QString proxyHost("myproxy.test.com");

    // set an arbitrary proxy
    QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::HttpProxy, proxyHost, 80));
    factory = 0;

    // localhost
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QUrl("http://localhost/")));
    QVERIFY(list.isEmpty() || (list[0].type() == QNetworkProxy::NoProxy));
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QString("localhost"), 80));
    QVERIFY(list.isEmpty() || (list[0].type() == QNetworkProxy::NoProxy));

    // 127.0.0.1
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QUrl("http://127.0.0.1/")));
    QVERIFY(list.isEmpty() || (list[0].type() == QNetworkProxy::NoProxy));
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QString("127.0.0.1"), 80));
    QVERIFY(list.isEmpty() || (list[0].type() == QNetworkProxy::NoProxy));

    // [::1]
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QUrl("http://[::1]/")));
    QVERIFY(list.isEmpty() || (list[0].type() == QNetworkProxy::NoProxy));
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QString("[::1]"), 80));
    QVERIFY(list.isEmpty() || (list[0].type() == QNetworkProxy::NoProxy));

    // an arbitrary host
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QUrl("http://another.host.com/")));
    QVERIFY((!list.isEmpty()) && (list[0].hostName() == proxyHost));
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QString("another.host.com"), 80));
    QVERIFY((!list.isEmpty()) && (list[0].hostName() == proxyHost));

    // disable proxy
    QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::NoProxy));
    factory = 0;

    // localhost
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QUrl("http://localhost/")));
    QVERIFY(list.isEmpty() || (list[0].type() == QNetworkProxy::NoProxy));
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QString("localhost"), 80));
    QVERIFY(list.isEmpty() || (list[0].type() == QNetworkProxy::NoProxy));

    // 127.0.0.1
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QUrl("http://127.0.0.1/")));
    QVERIFY(list.isEmpty() || (list[0].type() == QNetworkProxy::NoProxy));
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QString("127.0.0.1"), 80));
    QVERIFY(list.isEmpty() || (list[0].type() == QNetworkProxy::NoProxy));

    // [::1]
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QUrl("http://[::1]/")));
    QVERIFY(list.isEmpty() || (list[0].type() == QNetworkProxy::NoProxy));
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QString("[::1]"), 80));
    QVERIFY(list.isEmpty() || (list[0].type() == QNetworkProxy::NoProxy));

    // an arbitrary host
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QUrl("http://another.host.com/")));
    QVERIFY(list.isEmpty() || (list[0].type() == QNetworkProxy::NoProxy));
    list = QNetworkProxyFactory::proxyForQuery(QNetworkProxyQuery(QString("another.host.com"), 80));
    QVERIFY(list.isEmpty() || (list[0].type() == QNetworkProxy::NoProxy));
}

Q_DECLARE_METATYPE(QNetworkProxy::ProxyType)

void tst_QNetworkProxyFactory::genericSystemProxy()
{
    QFETCH(QByteArray, envVar);
    QFETCH(QByteArray, url);
    QFETCH(QNetworkProxy::ProxyType, proxyType);
    QFETCH(QString, hostName);
    QFETCH(int, port);

    qputenv(envVar, url);
    const QList<QNetworkProxy> systemProxy = QNetworkProxyFactory::systemProxyForQuery();
    QCOMPARE(systemProxy.size(), 1);
    QCOMPARE(systemProxy.first().type(), proxyType);
    QCOMPARE(systemProxy.first().hostName(), hostName);
    QCOMPARE(systemProxy.first().port(), static_cast<quint16>(port));
    qunsetenv(envVar);
}

void tst_QNetworkProxyFactory::genericSystemProxy_data()
{
    // We can only use the generic system proxy where available:
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS) || defined(Q_OS_ANDROID) || BOBUI_CONFIG(libproxy)
    QSKIP("Generic system proxy not available on this platform.");
#else
    BOBUIest::addColumn<QByteArray>("envVar");
    BOBUIest::addColumn<QByteArray>("url");
    BOBUIest::addColumn<QNetworkProxy::ProxyType>("proxyType");
    BOBUIest::addColumn<QString>("hostName");
    BOBUIest::addColumn<int>("port");

    BOBUIest::newRow("no proxy") << QByteArray("http_proxy") << QByteArray() << QNetworkProxy::NoProxy
                              << QString() << 0;
    BOBUIest::newRow("socks5") << QByteArray("http_proxy") << QByteArray("socks5://127.0.0.1:4242")
                            << QNetworkProxy::Socks5Proxy << QString("127.0.0.1") << 4242;
    BOBUIest::newRow("http") << QByteArray("http_proxy") << QByteArray("http://example.com:666")
                          << QNetworkProxy::HttpProxy << QString("example.com") << 666;
#endif
}

class QSPFBOBUIhread : public BOBUIhread
{
protected:
    void run() override
    {
        proxies = QNetworkProxyFactory::systemProxyForQuery(query);
    }
public:
    QNetworkProxyQuery query;
    QList<QNetworkProxy> proxies;
};

//regression test for BOBUIBUG-18799
void tst_QNetworkProxyFactory::systemProxyForQueryCalledFromThread()
{
    if (QSysInfo::productType() == QLatin1String("windows") && QSysInfo::productVersion() == QLatin1String("7sp1")) {
        QSKIP("This test fails by the systemProxyForQuery() call hanging - BOBUIQAINFRA-1200");
    }
    QUrl url(QLatin1String("http://bobui-project.org"));
    QNetworkProxyQuery query(url);
    QSPFBOBUIhread thread;
    thread.query = query;
    connect(&thread, SIGNAL(finished()), &BOBUIestEventLoop::instance(), SLOT(exitLoop()));
    thread.start();
    BOBUIestEventLoop::instance().enterLoop(5);
    QVERIFY(thread.isFinished());
    QCOMPARE(thread.proxies, QNetworkProxyFactory::systemProxyForQuery(query));
}

BOBUIEST_MAIN(tst_QNetworkProxyFactory)
#include "tst_qnetworkproxyfactory.moc"
