// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <qglobal.h>
#if defined(_WIN32)
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#define SOCKET int
#define INVALID_SOCKET -1
#endif

#include <BOBUIest>
#include <QSignalSpy>
#include <BOBUIimer>

#ifndef Q_OS_WIN
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#include <qcoreapplication.h>
#include <bobuicpsocket.h>
#include <bobuicpserver.h>
#include <qhostaddress.h>
#if BOBUI_CONFIG(process)
# include <qprocess.h>
#endif
#include <qstringlist.h>
#include <qplatformdefs.h>
#include <qhostinfo.h>
#include <qnetworkinterface.h>

#include <QNetworkProxy>
#include <QSet>
#include <QList>

#include "../../../network-settings.h"

#if defined(Q_OS_LINUX)
#define SHOULD_CHECK_SYSCALL_SUPPORT
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#endif

class tst_BOBUIcpServer : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase_data();
    void initTestCase();
    void init();
    void cleanup();
    void getSetCheck();
    void constructing();
    void clientServerLoop();
    void ipv6Server();
    void dualStack_data();
    void dualStack();
    void ipv6ServerMapped();
    void crashTests();
    void maxPendingConnections();
    void listenError();
    void waitForConnectionTest();
    void setSocketDescriptor();
    void listenWhileListening();
    void addressReusable();
    void setNewSocketDescriptorBlocking();
#ifndef BOBUI_NO_NETWORKPROXY
    void invalidProxy_data();
    void invalidProxy();
    void proxyFactory_data();
    void proxyFactory();
#endif // !BOBUI_NO_NETWORKPROXY

    void bobuibug14268_peek();

    void serverAddress_data();
    void serverAddress();

    void bobuibug6305_data() { serverAddress_data(); }
    void bobuibug6305();

    void linkLocal();

    void eagainBlockingAccept();

    void canAccessPendingConnectionsWhileNotListening();

    void pauseAccepting();

    void pendingConnectionAvailable_data();
    void pendingConnectionAvailable();

private:
    bool shouldSkipIpv6TestsForBrokenGetsockopt();
#ifdef SHOULD_CHECK_SYSCALL_SUPPORT
    bool ipv6GetsockoptionMissing(int level, int optname);
#endif

    QString crashingServerDir;
};

// Testing get/set functions
void tst_BOBUIcpServer::getSetCheck()
{
    BOBUIcpServer obj1;
    // int BOBUIcpServer::maxPendingConnections()
    // void BOBUIcpServer::setMaxPendingConnections(int)
    obj1.setMaxPendingConnections(0);
    QCOMPARE(0, obj1.maxPendingConnections());
    obj1.setMaxPendingConnections(INT_MIN);
    QCOMPARE(INT_MIN, obj1.maxPendingConnections());
    obj1.setMaxPendingConnections(INT_MAX);
    QCOMPARE(INT_MAX, obj1.maxPendingConnections());
}

void tst_BOBUIcpServer::initTestCase_data()
{
    BOBUIest::addColumn<bool>("setProxy");
    BOBUIest::addColumn<int>("proxyType");

    BOBUIest::newRow("WithoutProxy") << false << 0;
#if BOBUI_CONFIG(socks5)
    BOBUIest::newRow("WithSocks5Proxy") << true << int(QNetworkProxy::Socks5Proxy);
#endif

    crashingServerDir = QFINDTESTDATA("crashingServer");
    QVERIFY2(!crashingServerDir.isEmpty(), qPrintable(
        QString::fromLatin1("Couldn't find crashingServer dir starting from %1.").arg(QDir::currentPath())));
}

void tst_BOBUIcpServer::initTestCase()
{
#ifdef BOBUI_TEST_SERVER
    QVERIFY(BobUINetworkSettings::verifyConnection(BobUINetworkSettings::socksProxyServerName(), 1080));
    QVERIFY(BobUINetworkSettings::verifyConnection(BobUINetworkSettings::httpProxyServerName(), 3128));
    // FTP currently not supported:
    // QVERIFY(BobUINetworkSettings::verifyConnection(BobUINetworkSettings::ftpProxyServerName(), 2121));
    QVERIFY(BobUINetworkSettings::verifyConnection(BobUINetworkSettings::imapServerName(), 143));
#else
    if (!BobUINetworkSettings::verifyTestNetworkSettings())
        QSKIP("No network test server available");
#endif
}

void tst_BOBUIcpServer::init()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy) {
#ifndef BOBUI_NO_NETWORKPROXY
        QFETCH_GLOBAL(int, proxyType);
        if (proxyType == QNetworkProxy::Socks5Proxy) {
            QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::Socks5Proxy, BobUINetworkSettings::socksProxyServerName(), 1080));
        }
#else // !BOBUI_NO_NETWORKPROXY
        QSKIP("No proxy support");
#endif // BOBUI_NO_NETWORKPROXY
    }
}

void tst_BOBUIcpServer::cleanup()
{
#ifndef BOBUI_NO_NETWORKPROXY
    QNetworkProxy::setApplicationProxy(QNetworkProxy::DefaultProxy);
#endif
}

#ifdef SHOULD_CHECK_SYSCALL_SUPPORT
bool tst_BOBUIcpServer::ipv6GetsockoptionMissing(int level, int optname)
{
    int testSocket;

    testSocket = socket(PF_INET6, SOCK_STREAM, 0);

    // If we can't test here, assume it's not missing
    if (testSocket == -1)
        return false;

    bool result = false;
    if (getsockopt(testSocket, level, optname, nullptr, 0) == -1) {
        if (errno == EOPNOTSUPP) {
            result = true;
        }
    }

    close(testSocket);
    return result;
}
#endif //SHOULD_CHECK_SYSCALL_SUPPORT

bool tst_BOBUIcpServer::shouldSkipIpv6TestsForBrokenGetsockopt()
{
#ifdef SHOULD_CHECK_SYSCALL_SUPPORT
    // Following parameters for setsockopt are not supported by all QEMU versions:
    if (ipv6GetsockoptionMissing(SOL_IPV6, IPV6_V6ONLY)) {
        return true;
    }
#endif //SHOULD_CHECK_SYSCALL_SUPPORT

    return false;
}


//----------------------------------------------------------------------------------

void tst_BOBUIcpServer::constructing()
{
    BOBUIcpServer socket;

    // Check the initial state of the BOBUIcpSocket.
    QCOMPARE(socket.isListening(), false);
    QCOMPARE((int)socket.serverPort(), 0);
    QCOMPARE(socket.serverAddress(), QHostAddress());
    QCOMPARE(socket.maxPendingConnections(), 30);
    QCOMPARE(socket.hasPendingConnections(), false);
    QCOMPARE(socket.socketDescriptor(), (qintptr)-1);
    QCOMPARE(socket.serverError(), QAbstractSocket::UnknownSocketError);

    // Check the state of the socket layer?
}

//----------------------------------------------------------------------------------
void tst_BOBUIcpServer::clientServerLoop()
{
    BOBUIcpServer server;

    QSignalSpy spy(&server, SIGNAL(newConnection()));

    QVERIFY(!server.isListening());
    QVERIFY(!server.hasPendingConnections());
    QVERIFY(server.listen(QHostAddress::Any, 11423));
    QVERIFY(server.isListening());

    BOBUIcpSocket client;

    QHostAddress serverAddress = QHostAddress::LocalHost;
    if (!(server.serverAddress() == QHostAddress::Any) && !(server.serverAddress() == QHostAddress::AnyIPv6) && !(server.serverAddress() == QHostAddress::AnyIPv4))
        serverAddress = server.serverAddress();

    client.connectToHost(serverAddress, server.serverPort());
    QVERIFY(client.waitForConnected(5000));

    QVERIFY(server.waitForNewConnection(5000));
    QVERIFY(server.hasPendingConnections());

    QCOMPARE(spy.size(), 1);

    BOBUIcpSocket *serverSocket = server.nextPendingConnection();
    QVERIFY(serverSocket != 0);

    QVERIFY(serverSocket->write("Greetings, client!\n", 19) == 19);
    serverSocket->flush();

    QVERIFY(client.waitForReadyRead(5000));
    QByteArray arr = client.readAll();
    QCOMPARE(arr.constData(), "Greetings, client!\n");

    QVERIFY(client.write("Well, hello to you!\n", 20) == 20);
    client.flush();

    QVERIFY(serverSocket->waitForReadyRead(5000));
    arr = serverSocket->readAll();
    QCOMPARE(arr.constData(), "Well, hello to you!\n");
}

//----------------------------------------------------------------------------------
void tst_BOBUIcpServer::ipv6Server()
{
    if (!BobUINetworkSettings::hasIPv6())
        QSKIP("system doesn't support ipv6!");
    //### need to enter the event loop for the server to get the connection ?? ( windows)
    BOBUIcpServer server;
    if (!server.listen(QHostAddress::LocalHostIPv6, 8944)) {
        QCOMPARE(server.serverError(), QAbstractSocket::UnsupportedSocketOperationError);
        return;
    }

    QCOMPARE(server.serverPort(), quint16(8944));
    QVERIFY(server.serverAddress() == QHostAddress::LocalHostIPv6);

    BOBUIcpSocket client;
    client.connectToHost("::1", 8944);
    QVERIFY(client.waitForConnected(5000));

    QVERIFY(server.waitForNewConnection());
    QVERIFY(server.hasPendingConnections());

    BOBUIcpSocket *serverSocket = 0;
    QVERIFY((serverSocket = server.nextPendingConnection()));
    serverSocket->close();
    delete serverSocket;
}

Q_DECLARE_METATYPE(QHostAddress);

void tst_BOBUIcpServer::dualStack_data()
{
    BOBUIest::addColumn<QHostAddress>("bindAddress");
    BOBUIest::addColumn<bool>("v4ok");
    BOBUIest::addColumn<bool>("v6ok");
    BOBUIest::newRow("any") << QHostAddress(QHostAddress::Any) << true << true;
    BOBUIest::newRow("anyIPv4") << QHostAddress(QHostAddress::AnyIPv4) << true << false;
    BOBUIest::newRow("anyIPv6") << QHostAddress(QHostAddress::AnyIPv6) << false << true;
}

void tst_BOBUIcpServer::dualStack()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy)
        QSKIP("test server proxy doesn't support ipv6");
    if (!BobUINetworkSettings::hasIPv6())
        QSKIP("system doesn't support ipv6!");
    QFETCH(QHostAddress, bindAddress);
    QFETCH(bool, v4ok);
    QFETCH(bool, v6ok);

    BOBUIcpServer server;
    QVERIFY(server.listen(bindAddress));

    BOBUIcpSocket v4client;
    v4client.connectToHost(QHostAddress::LocalHost, server.serverPort());

    BOBUIcpSocket v6client;
    v6client.connectToHost(QHostAddress::LocalHostIPv6, server.serverPort());

    QCOMPARE(v4client.waitForConnected(5000), v4ok);
    QCOMPARE(v6client.waitForConnected(5000), v6ok);
}

//----------------------------------------------------------------------------------
void tst_BOBUIcpServer::ipv6ServerMapped()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy)
        return;

    BOBUIcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost));

    // let's try the normal case
    BOBUIcpSocket client1;
    client1.connectToHost("127.0.0.1", server.serverPort());
    QVERIFY(server.waitForNewConnection(5000));
    delete server.nextPendingConnection();

    if (!BobUINetworkSettings::hasIPv6())
        QSKIP("system doesn't support ipv6!");

    // let's try the mapped one in the nice format
    BOBUIcpSocket client2;
    client2.connectToHost("::ffff:127.0.0.1", server.serverPort());
    QVERIFY(server.waitForNewConnection(5000));
    delete server.nextPendingConnection();

    // let's try the mapped in hex format
    BOBUIcpSocket client3;
    client3.connectToHost("::ffff:7F00:0001", server.serverPort());
    QVERIFY(server.waitForNewConnection(5000));
    delete server.nextPendingConnection();

    // However connecting to the v6 localhost should not work
    BOBUIcpSocket client4;
    client4.connectToHost("::1", server.serverPort());
    QVERIFY(!server.waitForNewConnection(5000));
}

//----------------------------------------------------------------------------------
void tst_BOBUIcpServer::crashTests()
{
    BOBUIcpServer server;
    server.close();
    QVERIFY(server.listen());
}

//----------------------------------------------------------------------------------
void tst_BOBUIcpServer::maxPendingConnections()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy) {
#ifndef BOBUI_NO_NETWORKPROXY
        QFETCH_GLOBAL(int, proxyType);
        if (proxyType == QNetworkProxy::Socks5Proxy)
            QSKIP("With socks5 only 1 connection is allowed ever");
#else // !BOBUI_NO_NETWORKPROXY
        QSKIP("No proxy support");
#endif // BOBUI_NO_NETWORKPROXY
    }
    //### sees to fail sometimes ... a timing issue with the test on windows
    BOBUIcpServer server;
    server.setMaxPendingConnections(2);

    BOBUIcpSocket socket1;
    BOBUIcpSocket socket2;
    BOBUIcpSocket socket3;

    QSignalSpy spy(&server, SIGNAL(newConnection()));
    QVERIFY(server.listen());

    socket1.connectToHost(QHostAddress::LocalHost, server.serverPort());
    socket2.connectToHost(QHostAddress::LocalHost, server.serverPort());
    socket3.connectToHost(QHostAddress::LocalHost, server.serverPort());

    // We must have two and only two connections. First compare waits until
    // two connections have been made. The second compare makes sure no
    // more are accepted. Creating connections happens multithreaded so
    // qWait must be used for that.
    BOBUIRY_COMPARE(spy.size(), 2);
    BOBUIest::qWait(100);
    QCOMPARE(spy.size(), 2);

    QVERIFY(server.hasPendingConnections());
    QVERIFY(server.nextPendingConnection());
    QVERIFY(server.hasPendingConnections());
    QVERIFY(server.nextPendingConnection());
    QVERIFY(!server.hasPendingConnections());
    QCOMPARE(server.nextPendingConnection(), (BOBUIcpSocket*)0);

    QVERIFY(server.waitForNewConnection(5000));

    QVERIFY(server.hasPendingConnections());
    QVERIFY(server.nextPendingConnection());
}

//----------------------------------------------------------------------------------
void tst_BOBUIcpServer::listenError()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy) {
#ifndef BOBUI_NO_NETWORKPROXY
        QFETCH_GLOBAL(int, proxyType);
        if (proxyType == QNetworkProxy::Socks5Proxy)
            QSKIP("With socks5 we can not make hard requirements on the address or port");
#else // !BOBUI_NO_NETWORKPROXY
        QSKIP("No proxy support");
#endif //BOBUI_NO_NETWORKPROXY
    }
    BOBUIcpServer server;
    QVERIFY(!server.listen(QHostAddress("1.2.3.4"), 0));
    QCOMPARE(server.serverError(), QAbstractSocket::SocketAddressNotAvailableError);
    QCOMPARE(server.errorString().toLatin1().constData(), "The address is not available");
}

class ThreadConnector : public BOBUIhread
{
public:
    ThreadConnector(const QHostAddress &host, quint16 port)
        : host(host), port(port)
    { }

    ~ThreadConnector()
    {
        wait();
    }

protected:
    void run() override
    {
        sleep(std::chrono::seconds{2});

        BOBUIcpSocket socket;
        socket.connectToHost(host, port);

        QEventLoop loop;
        BOBUIimer::singleShot(5000, &loop, SLOT(quit()));
        loop.exec();
    }

private:
    QHostAddress host;
    quint16 port;
};

//----------------------------------------------------------------------------------
void tst_BOBUIcpServer::waitForConnectionTest()
{

    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy) {
#ifndef BOBUI_NO_NETWORKPROXY
        QFETCH_GLOBAL(int, proxyType);
        if (proxyType == QNetworkProxy::Socks5Proxy)
            QSKIP("Localhost servers don't work well with SOCKS5");
#else // !BOBUI_NO_NETWORKPROXY
        QSKIP("No proxy support");
#endif // BOBUI_NO_NETWORKPROXY
    }

    BOBUIcpSocket findLocalIpSocket;
    findLocalIpSocket.connectToHost(BobUINetworkSettings::imapServerName(), 143);
    QVERIFY(findLocalIpSocket.waitForConnected(5000));

    BOBUIcpServer server;
    bool timeout = false;
    QVERIFY(server.listen(findLocalIpSocket.localAddress()));
    QVERIFY(!server.waitForNewConnection(1000, &timeout));
    QCOMPARE(server.serverError(), QAbstractSocket::SocketTimeoutError);
    QVERIFY(timeout);

    ThreadConnector connector(findLocalIpSocket.localAddress(), server.serverPort());
    connector.start();

    QVERIFY(server.waitForNewConnection(3000, &timeout));
    QVERIFY(!timeout);
}

//----------------------------------------------------------------------------------
void tst_BOBUIcpServer::setSocketDescriptor()
{
    BOBUIcpServer server;
    QVERIFY(!server.setSocketDescriptor(42));
    QCOMPARE(server.serverError(), QAbstractSocket::UnsupportedSocketOperationError);
    //adopting Open C sockets is not supported, neither is adopting externally created RSocket
#ifdef Q_OS_WIN
    // ensure winsock is started
    WSADATA wsaData;
    QVERIFY(WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR);
#endif

    SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    QVERIFY(sock != INVALID_SOCKET);

    sockaddr_in sin;
    memset(&sin, 0, sizeof(sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = 0;
    sin.sin_addr.s_addr = 0x00000000;
    QVERIFY(::bind(sock, (sockaddr*)&sin, sizeof(sockaddr_in)) == 0);
    QVERIFY(::listen(sock, 10) == 0);
    QVERIFY(server.setSocketDescriptor(sock));

#ifdef Q_OS_WIN
    WSACleanup();
#endif
}

//----------------------------------------------------------------------------------
void tst_BOBUIcpServer::listenWhileListening()
{
    BOBUIcpServer server;
    QVERIFY(server.listen());
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIcpServer::listen() called when already listening");
    QVERIFY(!server.listen());
}

//----------------------------------------------------------------------------------

class SeverWithBlockingSockets : public BOBUIcpServer
{
public:
    SeverWithBlockingSockets()
        : ok(false) { }

    bool ok;

protected:
    void incomingConnection(qintptr socketDescriptor) override
    {
        // how a user woulddo it (qabstractsocketengine is not public)
        unsigned long arg = 0;
#if defined(Q_OS_WIN)
        ok = ::ioctlsocket(socketDescriptor, FIONBIO, &arg) == 0;
        ::closesocket(socketDescriptor);
#else
        ok = ::ioctl(socketDescriptor, FIONBIO, &arg) == 0;
        ::close(socketDescriptor);
#endif
    }
};

void tst_BOBUIcpServer::addressReusable()
{
#if !BOBUI_CONFIG(process)
    QSKIP("No qprocess support");
#else
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy) {
#ifndef BOBUI_NO_NETWORKPROXY
        QFETCH_GLOBAL(int, proxyType);
        if (proxyType == QNetworkProxy::Socks5Proxy)
            QSKIP("With socks5 this test does not make senans at the momment");
#else // !BOBUI_NO_NETWORKPROXY
        QSKIP("No proxy support");
#endif // BOBUI_NO_NETWORKPROXY
    }

    BOBUIcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost, 0));
    quint16 serverPort = server.serverPort();
    qDebug() << "Got port" << serverPort;
    server.close();     // cleanly close

    BOBUIest::qSleep(10);

    // The crashingServer process will crash once it gets a connection.
    QProcess process;
    QString processExe = crashingServerDir + "/crashingServer";
    process.start(processExe, { QString::number(serverPort) });
    QVERIFY2(process.waitForStarted(), qPrintable(
        QString::fromLatin1("Could not start %1: %2").arg(processExe, process.errorString())));
    QVERIFY2(process.waitForReadyRead(5000), qPrintable(process.readAllStandardError()));

    BOBUIcpSocket socket;
    socket.connectToHost(QHostAddress::LocalHost, serverPort);
    QVERIFY(socket.waitForConnected(5000));

    QVERIFY(process.waitForFinished(30000));

    // Give the system some time.
    BOBUIest::qSleep(10);

    // listen again
    QVERIFY2(server.listen(QHostAddress::LocalHost, serverPort),
             qPrintable(server.errorString()));
#endif
}

void tst_BOBUIcpServer::setNewSocketDescriptorBlocking()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy) {
#ifndef BOBUI_NO_NETWORKPROXY
        QFETCH_GLOBAL(int, proxyType);
        if (proxyType == QNetworkProxy::Socks5Proxy)
            QSKIP("With socks5 we can not make the socket descripter blocking");
#else // !BOBUI_NO_NETWORKPROXY
        QSKIP("No proxy support");
#endif // BOBUI_NO_NETWORKPROXY
    }
    SeverWithBlockingSockets server;
    QVERIFY(server.listen());

    BOBUIcpSocket socket;
    socket.connectToHost(QHostAddress::LocalHost, server.serverPort());
    QVERIFY(server.waitForNewConnection(5000));
    QVERIFY(server.ok);
}

#ifndef BOBUI_NO_NETWORKPROXY
void tst_BOBUIcpServer::invalidProxy_data()
{
    BOBUIest::addColumn<int>("type");
    BOBUIest::addColumn<QString>("host");
    BOBUIest::addColumn<int>("port");
    BOBUIest::addColumn<int>("expectedError");

    const QString imapIp = BobUINetworkSettings::imapServerIp().toString();
    const QString httpProxyIp = BobUINetworkSettings::httpProxyServerIp().toString();
    const QString socksIp = BobUINetworkSettings::socksProxyServerIp().toString();
    BOBUIest::newRow("ftp-proxy") << int(QNetworkProxy::FtpCachingProxy) << imapIp << 143
                               << int(QAbstractSocket::UnsupportedSocketOperationError);
    BOBUIest::newRow("http-proxy") << int(QNetworkProxy::HttpProxy) << httpProxyIp << 3128
                                << int(QAbstractSocket::UnsupportedSocketOperationError);

    BOBUIest::newRow("no-such-host") << int(QNetworkProxy::Socks5Proxy)
                                  << "invalid.test.bobui-project.org" << 1080
                                  << int(QAbstractSocket::ProxyNotFoundError);
    BOBUIest::newRow("socks5-on-http") << int(QNetworkProxy::Socks5Proxy) << httpProxyIp << 3128
                                    << int(QAbstractSocket::SocketTimeoutError);
}

void tst_BOBUIcpServer::invalidProxy()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy)
        return;

    QFETCH(int, type);
    QFETCH(QString, host);
    QFETCH(int, port);
    QNetworkProxy::ProxyType proxyType = QNetworkProxy::ProxyType(type);
    QNetworkProxy proxy(proxyType, host, port);

    BOBUIcpServer server;
    server.setProxy(proxy);
    bool listenResult = server.listen();

    QVERIFY(!listenResult);
    QVERIFY(!server.errorString().isEmpty());

    // note: the following test is not a hard failure.
    // Sometimes, error codes change for the better
    BOBUIEST(int(server.serverError()), "expectedError");
}

// copied from tst_qnetworkreply.cpp
class MyProxyFactory: public QNetworkProxyFactory
{
public:
    int callCount;
    QList<QNetworkProxy> toReturn;
    QNetworkProxyQuery lastQuery;
    inline MyProxyFactory() { clear(); }

    inline void clear()
    {
        callCount = 0;
        toReturn = QList<QNetworkProxy>() << QNetworkProxy::DefaultProxy;
        lastQuery = QNetworkProxyQuery();
    }

    virtual QList<QNetworkProxy> queryProxy(const QNetworkProxyQuery &query) override
    {
        lastQuery = query;
        ++callCount;
        return toReturn;
    }
};

void tst_BOBUIcpServer::proxyFactory_data()
{
    BOBUIest::addColumn<QList<QNetworkProxy> >("proxyList");
    BOBUIest::addColumn<QNetworkProxy>("proxyUsed");
    BOBUIest::addColumn<bool>("fails");
    BOBUIest::addColumn<int>("expectedError");

    QList<QNetworkProxy> proxyList;

    // tests that do get to listen

    proxyList << QNetworkProxy(QNetworkProxy::Socks5Proxy, BobUINetworkSettings::socksProxyServerName(), 1080);
    BOBUIest::newRow("socks5")
        << proxyList << proxyList.at(0)
        << false << int(QAbstractSocket::UnknownSocketError);

    proxyList.clear();
    proxyList << QNetworkProxy(QNetworkProxy::HttpCachingProxy, BobUINetworkSettings::httpProxyServerName(), 3128)
              << QNetworkProxy(QNetworkProxy::Socks5Proxy, BobUINetworkSettings::socksProxyServerName(), 1080);
    BOBUIest::newRow("cachinghttp+socks5")
        << proxyList << proxyList.at(1)
        << false << int(QAbstractSocket::UnknownSocketError);

#if 0 // ftp not currently supported
    proxyList.clear();
    proxyList << QNetworkProxy(QNetworkProxy::FtpCachingProxy, BobUINetworkSettings::ftpProxyServerName(), 2121)
              << QNetworkProxy(QNetworkProxy::HttpCachingProxy, BobUINetworkSettings::httpProxyServerName(), 3128)
              << QNetworkProxy(QNetworkProxy::Socks5Proxy, BobUINetworkSettings::socksProxyServerName(), 1080);
    BOBUIest::newRow("ftp+cachinghttp+socks5")
        << proxyList << proxyList.at(2)
        << false << int(QAbstractSocket::UnknownSocketError);
#endif

    // tests that fail to listen
    proxyList.clear();
    proxyList << QNetworkProxy(QNetworkProxy::HttpProxy, BobUINetworkSettings::httpProxyServerName(), 3128);
    BOBUIest::newRow("http")
        << proxyList << proxyList.at(0)
        << true << int(QAbstractSocket::UnsupportedSocketOperationError);

    proxyList.clear();
    proxyList << QNetworkProxy(QNetworkProxy::HttpCachingProxy, BobUINetworkSettings::httpProxyServerName(), 3128);
    BOBUIest::newRow("cachinghttp")
        << proxyList << QNetworkProxy()
        << true << int(QAbstractSocket::UnsupportedSocketOperationError);

#if 0 // ftp not currently supported
    proxyList.clear();
    proxyList << QNetworkProxy(QNetworkProxy::FtpCachingProxy, BobUINetworkSettings::ftpProxyServerName(), 2121);
    BOBUIest::newRow("ftp")
        << proxyList << QNetworkProxy()
        << true << int(QAbstractSocket::UnsupportedSocketOperationError);

    proxyList.clear();
    proxyList << QNetworkProxy(QNetworkProxy::FtpCachingProxy, BobUINetworkSettings::ftpProxyServerName(), 2121)
              << QNetworkProxy(QNetworkProxy::HttpCachingProxy, BobUINetworkSettings::httpProxyServerName(), 3128);
    BOBUIest::newRow("ftp+cachinghttp")
        << proxyList << QNetworkProxy()
        << true << int(QAbstractSocket::UnsupportedSocketOperationError);
#endif
}

void tst_BOBUIcpServer::proxyFactory()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy)
        return;

    QFETCH(QList<QNetworkProxy>, proxyList);
    QFETCH(QNetworkProxy, proxyUsed);
    QFETCH(bool, fails);

    MyProxyFactory *factory = new MyProxyFactory;
    factory->toReturn = proxyList;
    QNetworkProxyFactory::setApplicationProxyFactory(factory);

    BOBUIcpServer server;
    bool listenResult = server.listen();

    // Verify that the factory was called properly
    QCOMPARE(factory->callCount, 1);
    QCOMPARE(factory->lastQuery, QNetworkProxyQuery(0, QString(), QNetworkProxyQuery::TcpServer));

    QCOMPARE(listenResult, !fails);
    QCOMPARE(server.errorString().isEmpty(), !fails);

    // note: the following test is not a hard failure.
    // Sometimes, error codes change for the better
    BOBUIEST(int(server.serverError()), "expectedError");
}
#endif // !BOBUI_NO_NETWORKPROXY

class BobUIbug14268Helper : public QObject
{
    Q_OBJECT
public:
    QByteArray lastDataPeeked;
public slots:
    void newConnection() {
        BOBUIcpServer* server=static_cast<BOBUIcpServer*>(sender());
        BOBUIcpSocket* s=server->nextPendingConnection();
        connect(s,SIGNAL(readyRead()),this,SLOT(onServerReadyRead()));
    }
    void onServerReadyRead() {
        BOBUIcpSocket* clientSocket=static_cast<BOBUIcpSocket*>(sender());
        lastDataPeeked = clientSocket->peek(128*1024).toHex();
        BOBUIestEventLoop::instance().exitLoop();
    }
};

// there is a similar test inside tst_bobuicpsocket that uses the waitFor* functions instead
void tst_BOBUIcpServer::bobuibug14268_peek()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy)
        return;

    BOBUIcpServer server;
    server.listen();

    BobUIbug14268Helper helper;
    QObject::connect(&server, SIGNAL(newConnection()), &helper, SLOT(newConnection()));

    BOBUIcpSocket client;
    client.connectToHost(QHostAddress::LocalHost, server.serverPort());
    QVERIFY(client.waitForConnected(2000));

    client.write("abc\n");
    BOBUIestEventLoop::instance().enterLoop(5);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());
    QCOMPARE(helper.lastDataPeeked, QByteArray("6162630a"));

    client.write("def\n");
    BOBUIestEventLoop::instance().enterLoop(5);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());
    QCOMPARE(helper.lastDataPeeked, QByteArray("6162630a6465660a"));

    client.write("ghi\n");
    BOBUIestEventLoop::instance().enterLoop(5);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());
    QCOMPARE(helper.lastDataPeeked, QByteArray("6162630a6465660a6768690a"));
}

void tst_BOBUIcpServer::serverAddress_data()
{
    BOBUIest::addColumn<QHostAddress>("listenAddress");
    BOBUIest::addColumn<QHostAddress>("serverAddress");
    if (BobUINetworkSettings::hasIPv6())
        BOBUIest::newRow("Any") << QHostAddress(QHostAddress::Any) << QHostAddress(QHostAddress::Any);
    else
        BOBUIest::newRow("Any") << QHostAddress(QHostAddress::Any) << QHostAddress(QHostAddress::AnyIPv4);
    BOBUIest::newRow("AnyIPv4") << QHostAddress(QHostAddress::AnyIPv4) << QHostAddress(QHostAddress::AnyIPv4);
    if (BobUINetworkSettings::hasIPv6())
        BOBUIest::newRow("AnyIPv6") << QHostAddress(QHostAddress::AnyIPv6) << QHostAddress(QHostAddress::AnyIPv6);
    const auto ifaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : ifaces) {
        if ((iface.flags() & QNetworkInterface::IsUp) == 0)
            continue;
        const auto entries = iface.addressEntries();
        for (const QNetworkAddressEntry &entry : entries) {
            BOBUIest::newRow(qPrintable(entry.ip().toString())) << entry.ip() << entry.ip();
        }
    }
}

void tst_BOBUIcpServer::serverAddress()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy)
        return;

    QFETCH(QHostAddress, listenAddress);
    QFETCH(QHostAddress, serverAddress);
    BOBUIcpServer server;

    if (shouldSkipIpv6TestsForBrokenGetsockopt()
        && listenAddress == QHostAddress(QHostAddress::Any)) {
        QSKIP("Syscalls needed for ipv6 sockoptions missing functionality");
    }

    // TODO: why does this QSKIP?
    if (!server.listen(listenAddress))
        QSKIP(qPrintable(server.errorString()));
    QCOMPARE(server.serverAddress(), serverAddress);
}

// on OS X, calling listen() multiple times would succeed each time, which is
// most definitely not wanted.
void tst_BOBUIcpServer::bobuibug6305()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy)
        return;

    QFETCH(QHostAddress, listenAddress);

    BOBUIcpServer server;
    QVERIFY2(server.listen(listenAddress), qPrintable(server.errorString()));

    BOBUIcpServer server2;
    QVERIFY(!server2.listen(listenAddress, server.serverPort())); // second listen should fail
}

void tst_BOBUIcpServer::linkLocal()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy)
        return;

    QList <QHostAddress> addresses;
    QSet <QString> scopes;
    QHostAddress localMaskv4("169.254.0.0");
    QHostAddress localMaskv6("fe80::");
    const auto ifaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : ifaces) {
        //Windows preallocates link local addresses to interfaces that are down.
        //These may or may not work depending on network driver (they do not work for the Bluetooth PAN driver)
        if (iface.flags() & QNetworkInterface::IsUp) {
#if defined(Q_OS_WIN)
            // Do not connect to the Teredo Tunneling interface on Windows Xp.
            if (iface.humanReadableName() == QString("Teredo Tunneling Pseudo-Interface"))
                continue;
#elif defined(Q_OS_DARWIN)
            // Do not add "utun" interfaces on macOS: nothing ever gets received
            // (we don't know why)
            if (iface.name().startsWith("utun"))
                continue;
            // Do not use the iBridge interfae
            if (iface.hardwareAddress() == "AC:DE:48:00:11:22")
                continue;
            // Do no use the Apple Wireless Direct Link interfaces
            if (iface.name().startsWith("awdl"))
                continue;
#endif
            const auto entries = iface.addressEntries();
            for (const QNetworkAddressEntry &addressEntry : entries) {
                QHostAddress addr = addressEntry.ip();
                if (addr.isInSubnet(localMaskv4, 16)) {
                    addresses << addr;
                    qDebug() << addr;
                }
                else if (!addr.scopeId().isEmpty() && addr.isInSubnet(localMaskv6, 64)) {
                    scopes << addr.scopeId();
                    addresses << addr;
                    qDebug() << addr;
                }
            }
        }
    }
    if (addresses.isEmpty())
        QSKIP("no link local addresses");

    QList<BOBUIcpServer*> servers;
    quint16 port = 0;
    for (const QHostAddress &addr : std::as_const(addresses)) {
        BOBUIcpServer *server = new BOBUIcpServer;
        QVERIFY(server->listen(addr, port));
        port = server->serverPort(); //listen to same port on different interfaces
        servers << server;
    }

    QList<BOBUIcpSocket*> clients;
    for (const QHostAddress &addr : std::as_const(addresses)) {
        //unbound socket
        BOBUIcpSocket *socket = new BOBUIcpSocket;
        socket->connectToHost(addr, port);
        QVERIFY(socket->waitForConnected(5000));
        clients << socket;
        //bound socket
        socket = new BOBUIcpSocket;
        QVERIFY(socket->bind(addr));
        socket->connectToHost(addr, port);
        QVERIFY(socket->waitForConnected(5000));
        clients << socket;
    }

    //each server should have two connections
    for (BOBUIcpServer *server : std::as_const(servers)) {
        //qDebug() << "checking for connections" << server->serverAddress() << ":" << server->serverPort();
        QVERIFY(server->waitForNewConnection(5000));
        BOBUIcpSocket* remote = server->nextPendingConnection();
        QVERIFY(remote != nullptr);
        remote->close();
        delete remote;
        if (!server->hasPendingConnections())
            QVERIFY(server->waitForNewConnection(5000));
        remote = server->nextPendingConnection();
        QVERIFY(remote != nullptr);
        remote->close();
        delete remote;
        QVERIFY(!server->hasPendingConnections());
    }

    //Connecting to the same address with different scope should normally fail
    //However it will pass if there are two interfaces connected to the same physical network,
    //e.g. connected via wired and wireless interfaces, or two wired NICs.
    //which is a reasonably common case.
    //So this is not auto tested.

    qDeleteAll(clients);
    qDeleteAll(servers);
}

void tst_BOBUIcpServer::eagainBlockingAccept()
{
    BOBUIcpServer server;
    server.listen(QHostAddress::LocalHost, 7896);

    // Receiving a new connection causes TemporaryError, but shouldn't pause accepting.
    BOBUIcpSocket s;
    s.connectToHost(QHostAddress::LocalHost, 7896);
    QSignalSpy spy(&server, SIGNAL(newConnection()));
    BOBUIRY_COMPARE_WITH_TIMEOUT(spy.size(), 1, 500);
    s.close();

    // To test try again, should connect just fine.
    s.connectToHost(QHostAddress::LocalHost, 7896);
    BOBUIRY_COMPARE_WITH_TIMEOUT(spy.size(), 2, 500);
    s.close();
    server.close();
}

class NonListeningTcpServer : public BOBUIcpServer
{
public:
    void addSocketFromOutside(BOBUIcpSocket* s)
    {
        addPendingConnection(s);
    }
};

void tst_BOBUIcpServer::canAccessPendingConnectionsWhileNotListening()
{
    NonListeningTcpServer server;
    BOBUIcpSocket socket;
    server.addSocketFromOutside(&socket);
    QCOMPARE(&socket, server.nextPendingConnection());
}

void tst_BOBUIcpServer::pauseAccepting()
{
    BOBUIcpServer server;
    QSignalSpy spy(&server, &BOBUIcpServer::newConnection);
    QVERIFY(server.listen());

    QFETCH_GLOBAL(bool, setProxy);
    const auto address = QHostAddress(setProxy ? BobUINetworkSettings::socksProxyServerIp()
                                               : QHostAddress::LocalHost);

    const int NumSockets = 6;
    BOBUIcpSocket sockets[NumSockets];
    sockets[0].connectToHost(address, server.serverPort());
    QVERIFY(spy.wait());
    QCOMPARE(spy.size(), 1);

    server.pauseAccepting();
    for (int i = 1; i < NumSockets; ++i)
        sockets[i].connectToHost(address, server.serverPort());
    QVERIFY(!spy.wait(400));
    QCOMPARE(spy.size(), 1);

    server.resumeAccepting();
    if (setProxy) {
        QEXPECT_FAIL("", "The socks proxy does weird things after accepting the first connection",
                     Abort);
    }
    QVERIFY(spy.wait());
    QCOMPARE(spy.size(), 6);
}


// Only adds the socket to the pending connections list after emitNextSocket is
// called. It's very artificial, but it allows us to test the behavior of
// the pendingConnectionAvailable signal when a server doesn't add the socket
// during the incomingConnection virtual function.
class DerivedServer : public BOBUIcpServer
{
public:
    explicit DerivedServer(QObject *parent = nullptr)
        : BOBUIcpServer(parent)
    {
    }

    void emitNextSocket()
    {
        if (m_socketDescriptors.isEmpty())
            return;
        auto *socket = new BOBUIcpSocket(this);
        socket->setSocketDescriptor(m_socketDescriptors.back());
        m_socketDescriptors.pop_back();
        addPendingConnection(socket);
    }
protected:
    void incomingConnection(qintptr socketDescriptor) override
    {
        m_socketDescriptors.push_back(socketDescriptor);
    }
private:
    QList<qintptr> m_socketDescriptors;
};

void tst_BOBUIcpServer::pendingConnectionAvailable_data()
{
    BOBUIest::addColumn<bool>("useDerivedServer");
    BOBUIest::newRow("BOBUIcpServer") << false;
    BOBUIest::newRow("DerivedServer") << true;
}

void tst_BOBUIcpServer::pendingConnectionAvailable()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy)
        QSKIP("This feature does not differentiate with or without proxy");
    QFETCH(bool, useDerivedServer);

    BOBUIcpServer *server = useDerivedServer ? new DerivedServer : new BOBUIcpServer;
    if (!server->listen(QHostAddress::LocalHost, 0)) {
        qWarning() << "Server failed to listen:" << server->errorString();
        QSKIP("Server failed to listen");
    }
    QSignalSpy newConnectionSpy(server, &BOBUIcpServer::newConnection);
    QSignalSpy pendingConnectionSpy(server, &BOBUIcpServer::pendingConnectionAvailable);

    BOBUIcpSocket socket;
    socket.connectToHost(QHostAddress::LocalHost, server->serverPort());

    QVERIFY(newConnectionSpy.wait());
    QVERIFY(socket.waitForConnected());
    QCOMPARE(socket.state(), BOBUIcpSocket::ConnectedState);

    int expectedPendingConnections = useDerivedServer ? 0 : 1;
    QCOMPARE(pendingConnectionSpy.size(), expectedPendingConnections);

    if (useDerivedServer)
        static_cast<DerivedServer *>(server)->emitNextSocket();
    QCOMPARE(pendingConnectionSpy.size(), 1);
}

BOBUIEST_MAIN(tst_BOBUIcpServer)
#include "tst_bobuicpserver.moc"
