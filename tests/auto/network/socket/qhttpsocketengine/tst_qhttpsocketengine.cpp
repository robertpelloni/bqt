// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUITest/BOBUIest>
#include <BobUITest/BOBUIestEventLoop>
#include <BobUICore/QQueue>
#include <BobUICore/QString>
#include <BobUICore/QCoreApplication>

#include <private/qhttpsocketengine_p.h>
#include <qhostinfo.h>
#include <qhostaddress.h>
#include <bobuicpsocket.h>
#include <qdebug.h>
#include <qelapsedtimer.h>
#include <bobuicpserver.h>

#include "../../../network-settings.h"

using namespace std::chrono_literals;

class tst_QHttpSocketEngine : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void construction();
    void errorTest_data();
    void errorTest();
    void simpleConnectToIMAP();
    void simpleErrorsAndStates();

    void tcpSocketBlockingTest();
    void tcpSocketNonBlockingTest();
    void downloadBigFile();
   // void tcpLoopbackPerformance();
    void passwordAuth();
    void ensureEofTriggersNotification();

protected slots:
    void tcpSocketNonBlocking_hostFound();
    void tcpSocketNonBlocking_connected();
    void tcpSocketNonBlocking_closed();
    void tcpSocketNonBlocking_readyRead();
    void tcpSocketNonBlocking_bytesWritten(qint64);
    void exitLoopSlot();
    void downloadBigFileSlot();

private:
    BOBUIcpSocket *tcpSocketNonBlocking_socket;
    QStringList tcpSocketNonBlocking_data;
    qint64 tcpSocketNonBlocking_totalWritten;
    BOBUIcpSocket *tmpSocket;
    qint64 bytesAvailable;
};

class MiniHttpServer: public BOBUIcpServer
{
    Q_OBJECT
    BOBUIcpSocket *client;
    QList<QByteArray> dataToTransmit;

public:
    QByteArray receivedData;

    MiniHttpServer(const QList<QByteArray> &data) : client(0), dataToTransmit(data)
    {
        listen();
        connect(this, SIGNAL(newConnection()), this, SLOT(doAccept()));
    }

public slots:
    void doAccept()
    {
        client = nextPendingConnection();
        connect(client, SIGNAL(readyRead()), this, SLOT(sendData()));
    }

    void sendData()
    {
        receivedData += client->readAll();
        int idx = client->property("dataTransmitionIdx").toInt();
        if (receivedData.contains("\r\n\r\n") ||
            receivedData.contains("\n\n")) {
            if (idx < dataToTransmit.size())
                client->write(dataToTransmit.at(idx++));
            if (idx == dataToTransmit.size()) {
                client->disconnectFromHost();
                disconnect(client, 0, this, 0);
                client = 0;
            } else {
                client->setProperty("dataTransmitionIdx", idx);
            }
        }
    }
};

void tst_QHttpSocketEngine::initTestCase()
{
#ifdef BOBUI_TEST_SERVER
    QVERIFY(BobUINetworkSettings::verifyConnection(BobUINetworkSettings::httpProxyServerName(), 3128));
    QVERIFY(BobUINetworkSettings::verifyConnection(BobUINetworkSettings::socksProxyServerName(), 1080));
    QVERIFY(BobUINetworkSettings::verifyConnection(BobUINetworkSettings::httpServerName(), 80));
    QVERIFY(BobUINetworkSettings::verifyConnection(BobUINetworkSettings::imapServerName(), 143));
#else
    if (!BobUINetworkSettings::verifyTestNetworkSettings())
        QSKIP("No network test server available");
#endif
}

void tst_QHttpSocketEngine::init()
{
    tmpSocket = 0;
    bytesAvailable = 0;
}

//---------------------------------------------------------------------------
void tst_QHttpSocketEngine::construction()
{
    QHttpSocketEngine socketDevice;

    QVERIFY(!socketDevice.isValid());

    // Initialize device
    QVERIFY(socketDevice.initialize(QAbstractSocket::TcpSocket, QAbstractSocket::IPv4Protocol));
    QVERIFY(socketDevice.isValid());
    QCOMPARE(socketDevice.protocol(), QAbstractSocket::IPv4Protocol);
    QCOMPARE(socketDevice.socketType(), QAbstractSocket::TcpSocket);
    QCOMPARE(socketDevice.state(), QAbstractSocket::UnconnectedState);
   // QVERIFY(socketDevice.socketDescriptor() != -1);
    QCOMPARE(socketDevice.localAddress(), QHostAddress());
    QCOMPARE(socketDevice.localPort(), quint16(0));
    QCOMPARE(socketDevice.peerAddress(), QHostAddress());
    QCOMPARE(socketDevice.peerPort(), quint16(0));
    QCOMPARE(socketDevice.error(), QAbstractSocket::UnknownSocketError);

    //BOBUIest::ignoreMessage(BobUIWarningMsg, "QSocketLayer::bytesAvailable() was called in QAbstractSocket::UnconnectedState");
    QCOMPARE(socketDevice.bytesAvailable(), 0);

    //BOBUIest::ignoreMessage(BobUIWarningMsg, "QSocketLayer::hasPendingDatagrams() was called in QAbstractSocket::UnconnectedState");
    QVERIFY(!socketDevice.hasPendingDatagrams());
}

//---------------------------------------------------------------------------
void tst_QHttpSocketEngine::errorTest_data()
{
    BOBUIest::addColumn<QString>("hostname");
    BOBUIest::addColumn<int>("port");
    BOBUIest::addColumn<QString>("username");
    BOBUIest::addColumn<QString>("response");
    BOBUIest::addColumn<int>("expectedError");

    QQueue<QByteArray> responses;
    BOBUIest::newRow("proxy-host-not-found") << "this-host-does-not-exist." << 1080 << QString()
                                          << QString()
                                          << int(QAbstractSocket::ProxyNotFoundError);
    BOBUIest::newRow("proxy-connection-refused") << BobUINetworkSettings::socksProxyServerName() << 2 << QString()
                                              << QString()
                                              << int(QAbstractSocket::ProxyConnectionRefusedError);

    BOBUIest::newRow("garbage1") << QString() << 0 << QString()
                              << "This is not HTTP\r\n\r\n"
                              << int(QAbstractSocket::ProxyProtocolError);

    BOBUIest::newRow("garbage2") << QString() << 0 << QString()
                              << "This is not HTTP"
                              << int(QAbstractSocket::ProxyProtocolError);

    BOBUIest::newRow("garbage3") << QString() << 0 << QString()
                              << ""
                              << int(QAbstractSocket::ProxyConnectionClosedError);

    BOBUIest::newRow("forbidden") << QString() << 0 << QString()
                               << "HTTP/1.0 403 Forbidden\r\n\r\n"
                               << int(QAbstractSocket::SocketAccessError);

    BOBUIest::newRow("method-not-allowed") << QString() << 0 << QString()
                                        << "HTTP/1.0 405 Method Not Allowed\r\n\r\n"
                                        << int(QAbstractSocket::SocketAccessError);

    BOBUIest::newRow("proxy-authentication-too-short")
        << QString() << 0 << "foo"
        << "HTTP/1.0 407 Proxy Authentication Required\r\n\r\n"
        << int(QAbstractSocket::ProxyProtocolError);

    BOBUIest::newRow("proxy-authentication-invalid-method")
        << QString() << 0 << "foo"
        << "HTTP/1.0 407 Proxy Authentication Required\r\n"
           "Proxy-Authenticate: Frobnicator\r\n\r\n"
        << int(QAbstractSocket::ProxyProtocolError);

    BOBUIest::newRow("proxy-authentication-required")
        << QString() << 0 << "foo"
        << "HTTP/1.0 407 Proxy Authentication Required\r\n"
           "Proxy-Connection: close\r\n"
           "Proxy-Authenticate: Basic, realm=wonderland\r\n\r\n"
        << int(QAbstractSocket::ProxyAuthenticationRequiredError);

    BOBUIest::newRow("proxy-authentication-required2")
        << QString() << 0 << "foo"
        << "HTTP/1.0 407 Proxy Authentication Required\r\n"
           "Proxy-Connection: keep-alive\r\n"
           "Proxy-Authenticate: Basic, realm=wonderland\r\n\r\n"
           "\1"
           "HTTP/1.0 407 Proxy Authentication Required\r\n"
           "Proxy-Authenticate: Basic, realm=wonderland\r\n\r\n"
        << int(QAbstractSocket::ProxyAuthenticationRequiredError);

    BOBUIest::newRow("proxy-authentication-required-noclose")
        << QString() << 0 << "foo"
        << "HTTP/1.0 407 Proxy Authentication Required\r\n"
           "Proxy-Authenticate: Basic\r\n"
           "\r\n"
        << int(QAbstractSocket::ProxyAuthenticationRequiredError);

    BOBUIest::newRow("connection-refused") << QString() << 0 << QString()
                                        << "HTTP/1.0 503 Service Unavailable\r\n\r\n"
                                        << int(QAbstractSocket::ConnectionRefusedError);

    BOBUIest::newRow("host-not-found") << QString() << 0 << QString()
                                    << "HTTP/1.0 404 Not Found\r\n\r\n"
                                    << int(QAbstractSocket::HostNotFoundError);

    BOBUIest::newRow("weird-http-reply") << QString() << 0 << QString()
                                      << "HTTP/1.0 206 Partial Content\r\n\r\n"
                                      << int(QAbstractSocket::ProxyProtocolError);
}

void tst_QHttpSocketEngine::errorTest()
{
    QFETCH(QString, hostname);
    QFETCH(int, port);
    QFETCH(QString, username);
    QFETCH(QString, response);
    QFETCH(int, expectedError);

    MiniHttpServer server(response.toLatin1().split('\1'));

    if (hostname.isEmpty()) {
        hostname = "127.0.0.1";
        port = server.serverPort();
    }
    BOBUIcpSocket socket;
    socket.setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, hostname, port, username, username));
    socket.connectToHost("0.1.2.3", 12345);

    connect(&socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
            &BOBUIestEventLoop::instance(), SLOT(exitLoop()));
    BOBUIestEventLoop::instance().enterLoop(30);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());

    QCOMPARE(int(socket.error()), expectedError);
}

//---------------------------------------------------------------------------
void tst_QHttpSocketEngine::simpleConnectToIMAP()
{
    QHttpSocketEngine socketDevice;

    // Initialize device
    QVERIFY(socketDevice.initialize(QAbstractSocket::TcpSocket, QAbstractSocket::IPv4Protocol));
    QCOMPARE(socketDevice.state(), QAbstractSocket::UnconnectedState);

    socketDevice.setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, BobUINetworkSettings::httpProxyServerName(), 3128));
    QVERIFY(!socketDevice.connectToHost(BobUINetworkSettings::imapServerIp(), 143));
    QCOMPARE(socketDevice.state(), QAbstractSocket::ConnectingState);
    QVERIFY(socketDevice.waitForWrite());
    QCOMPARE(socketDevice.state(), QAbstractSocket::ConnectedState);
    QCOMPARE(socketDevice.peerAddress(), BobUINetworkSettings::imapServerIp());
    QVERIFY(!socketDevice.localAddress().isNull());
    QVERIFY(socketDevice.localPort() > 0);

    // Wait for the greeting
    QVERIFY(socketDevice.waitForRead());

    // Read the greeting
    qint64 available = int(socketDevice.bytesAvailable());
    QVERIFY(available > 0);
    QByteArray array;
    array.resize(int(available));
    QVERIFY(socketDevice.read(array.data(), array.size()) == available);

    // Check that the greeting is what we expect it to be
    QVERIFY2(BobUINetworkSettings::compareReplyIMAP(array), array.constData());


    // Write a logout message
    QByteArray array2 = "XXXX LOGOUT\r\n";
    QVERIFY(socketDevice.write(array2.data(),
                              array2.size()) == array2.size());

    // Wait for the response
    QVERIFY(socketDevice.waitForRead());

    available = int(socketDevice.bytesAvailable());
    QVERIFY(available > 0);
    array.resize(int(available));
    QVERIFY(socketDevice.read(array.data(), array.size()) == available);

    // Check that the greeting is what we expect it to be
    QCOMPARE(array.constData(), "* BYE LOGOUT received\r\nXXXX OK Completed\r\n");

    // Wait for the response
    QVERIFY(socketDevice.waitForRead());
    char c;
    QCOMPARE(socketDevice.read(&c, sizeof(c)), qint64(-1));
    QCOMPARE(socketDevice.error(), QAbstractSocket::RemoteHostClosedError);
    QCOMPARE(socketDevice.state(), QAbstractSocket::UnconnectedState);
}

//---------------------------------------------------------------------------
void tst_QHttpSocketEngine::simpleErrorsAndStates()
{
    {
        QHttpSocketEngine socketDevice;

        // Initialize device
        QVERIFY(socketDevice.initialize(QAbstractSocket::TcpSocket, QAbstractSocket::IPv4Protocol));

        socketDevice.setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, BobUINetworkSettings::httpProxyServerName(), 3128));

        QCOMPARE(socketDevice.state(), QAbstractSocket::UnconnectedState);
        QVERIFY(!socketDevice.connectToHost(QHostAddress(BobUINetworkSettings::socksProxyServerName()), 8088));
        QCOMPARE(socketDevice.state(), QAbstractSocket::ConnectingState);
        if (socketDevice.waitForWrite(30s)) {
            QVERIFY(socketDevice.state() == QAbstractSocket::ConnectedState ||
                    socketDevice.state() == QAbstractSocket::UnconnectedState);
        } else {
            QCOMPARE(socketDevice.error(), QAbstractSocket::SocketTimeoutError);
        }
    }

}

/*
//---------------------------------------------------------------------------
void tst_QHttpSocketEngine::tcpLoopbackPerformance()
{
    BOBUIcpServer server;

    // Bind to any port on all interfaces
    QVERIFY(server.bind(QHostAddress("0.0.0.0"), 0));
    QCOMPARE(server.state(), QAbstractSocket::BoundState);
    quint16 port = server.localPort();

    // Listen for incoming connections
    QVERIFY(server.listen());
    QCOMPARE(server.state(), QAbstractSocket::ListeningState);

    // Initialize a Tcp socket
    QHttpSocketEngine client;
    QVERIFY(client.initialize(QAbstractSocket::TcpSocket));

    client.setProxy(QHostAddress("80.232.37.158"), 1081);

    // Connect to our server
    if (!client.connectToHost(QHostAddress("127.0.0.1"), port)) {
        QVERIFY(client.waitForWrite());
        QVERIFY(client.connectToHost(QHostAddress("127.0.0.1"), port));
    }

    // The server accepts the connectio
    int socketDescriptor = server.accept();
    QVERIFY(socketDescriptor > 0);

    // A socket device is initialized on the server side, passing the
    // socket descriptor from accept(). It's pre-connected.
    QSocketLayer serverSocket;
    QVERIFY(serverSocket.initialize(socketDescriptor));
    QCOMPARE(serverSocket.state(), QAbstractSocket::ConnectedState);

    const int messageSize = 1024 * 256;
    QByteArray message1(messageSize, '@');
    QByteArray answer(messageSize, '@');

    QElapsedTimer timer;
    timer.start();
    qlonglong readBytes = 0;
    while (timer.elapsed() < 30000) {
        qlonglong written = serverSocket.write(message1.data(), message1.size());
        while (written > 0) {
            client.waitForRead();
            if (client.bytesAvailable() > 0) {
                qlonglong readNow = client.read(answer.data(), answer.size());
                written -= readNow;
                readBytes += readNow;
            }
        }
    }

    qDebug("\t\t%.1fMB/%.1fs: %.1fMB/s",
           readBytes / (1024.0 * 1024.0),
           timer.elapsed() / 1024.0,
           (readBytes / (timer.elapsed() / 1000.0)) / (1024 * 1024));
}
*/



void tst_QHttpSocketEngine::tcpSocketBlockingTest()
{
    QHttpSocketEngineHandler http;

    BOBUIcpSocket socket;

    // Connect
    socket.connectToHost(BobUINetworkSettings::imapServerName(), 143);
    QVERIFY(socket.waitForConnected());
    QCOMPARE(socket.state(), BOBUIcpSocket::ConnectedState);

    // Read greeting
    QVERIFY(socket.waitForReadyRead(30000));
    QString s = socket.readLine();
    QVERIFY2(BobUINetworkSettings::compareReplyIMAP(s.toLatin1()), qPrintable(s));

    // Write NOOP
    QCOMPARE((int) socket.write("1 NOOP\r\n", 8), 8);

    if (!socket.canReadLine())
        QVERIFY(socket.waitForReadyRead(30000));

    // Read response
    s = socket.readLine();
    QCOMPARE(s.toLatin1().constData(), "1 OK Completed\r\n");

    // Write LOGOUT
    QCOMPARE((int) socket.write("2 LOGOUT\r\n", 10), 10);

    if (!socket.canReadLine())
        QVERIFY(socket.waitForReadyRead(30000));

    // Read two lines of respose
    s = socket.readLine();
    QCOMPARE(s.toLatin1().constData(), "* BYE LOGOUT received\r\n");

    if (!socket.canReadLine())
        QVERIFY(socket.waitForReadyRead(30000));

    s = socket.readLine();
    QCOMPARE(s.toLatin1().constData(), "2 OK Completed\r\n");

    // Close the socket
    socket.close();

    // Check that it's closed
    QCOMPARE(socket.state(), BOBUIcpSocket::UnconnectedState);
}

//----------------------------------------------------------------------------------

void tst_QHttpSocketEngine::tcpSocketNonBlockingTest()
{
    QHttpSocketEngineHandler http;

    BOBUIcpSocket socket;
    connect(&socket, SIGNAL(hostFound()), SLOT(tcpSocketNonBlocking_hostFound()));
    connect(&socket, SIGNAL(connected()), SLOT(tcpSocketNonBlocking_connected()));
    connect(&socket, SIGNAL(disconnected()), SLOT(tcpSocketNonBlocking_closed()));
    connect(&socket, SIGNAL(bytesWritten(qint64)), SLOT(tcpSocketNonBlocking_bytesWritten(qint64)));
    connect(&socket, SIGNAL(readyRead()), SLOT(tcpSocketNonBlocking_readyRead()));
    tcpSocketNonBlocking_socket = &socket;

    // Connect
    socket.connectToHost(BobUINetworkSettings::imapServerName(), 143);
    QVERIFY(socket.state() == BOBUIcpSocket::HostLookupState ||
            socket.state() == BOBUIcpSocket::ConnectingState);

    BOBUIestEventLoop::instance().enterLoop(30);
    if (BOBUIestEventLoop::instance().timeout()) {
        QFAIL("Timed out");
    }

    if (socket.state() == BOBUIcpSocket::ConnectingState) {
        BOBUIestEventLoop::instance().enterLoop(30);
        if (BOBUIestEventLoop::instance().timeout()) {
            QFAIL("Timed out");
        }
    }

    QCOMPARE(socket.state(), BOBUIcpSocket::ConnectedState);

    BOBUIestEventLoop::instance().enterLoop(30);
    if (BOBUIestEventLoop::instance().timeout()) {
        QFAIL("Timed out");
    }

    // Read greeting
    QVERIFY(!tcpSocketNonBlocking_data.isEmpty());
    QByteArray data = tcpSocketNonBlocking_data.at(0).toLatin1();
    QVERIFY2(BobUINetworkSettings::compareReplyIMAP(data), data.constData());


    tcpSocketNonBlocking_data.clear();

    tcpSocketNonBlocking_totalWritten = 0;

    // Write NOOP
    QCOMPARE((int) socket.write("1 NOOP\r\n", 8), 8);


    BOBUIestEventLoop::instance().enterLoop(30);
    if (BOBUIestEventLoop::instance().timeout()) {
        QFAIL("Timed out");
    }

    QCOMPARE(tcpSocketNonBlocking_totalWritten, 8);


    BOBUIestEventLoop::instance().enterLoop(30);
    if (BOBUIestEventLoop::instance().timeout()) {
        QFAIL("Timed out");
    }


    // Read response
    QVERIFY(!tcpSocketNonBlocking_data.isEmpty());
    QCOMPARE(tcpSocketNonBlocking_data.at(0).toLatin1().constData(), "1 OK Completed\r\n");
    tcpSocketNonBlocking_data.clear();


    tcpSocketNonBlocking_totalWritten = 0;

    // Write LOGOUT
    QCOMPARE((int) socket.write("2 LOGOUT\r\n", 10), 10);

    BOBUIestEventLoop::instance().enterLoop(30);
    if (BOBUIestEventLoop::instance().timeout()) {
        QFAIL("Timed out");
    }

    QCOMPARE(tcpSocketNonBlocking_totalWritten, 10);

    // Wait for greeting
    BOBUIestEventLoop::instance().enterLoop(30);
    if (BOBUIestEventLoop::instance().timeout()) {
        QFAIL("Timed out");
    }

    // Read two lines of respose
    QCOMPARE(tcpSocketNonBlocking_data.at(0).toLatin1().constData(), "* BYE LOGOUT received\r\n");
    QCOMPARE(tcpSocketNonBlocking_data.at(1).toLatin1().constData(), "2 OK Completed\r\n");
    tcpSocketNonBlocking_data.clear();

    // Close the socket
    socket.close();

    // Check that it's closed
    QCOMPARE(socket.state(), BOBUIcpSocket::UnconnectedState);
}

void tst_QHttpSocketEngine::tcpSocketNonBlocking_hostFound()
{
    BOBUIestEventLoop::instance().exitLoop();
}

void tst_QHttpSocketEngine::tcpSocketNonBlocking_connected()
{
    BOBUIestEventLoop::instance().exitLoop();
}

void tst_QHttpSocketEngine::tcpSocketNonBlocking_readyRead()
{
    while (tcpSocketNonBlocking_socket->canReadLine())
        tcpSocketNonBlocking_data.append(tcpSocketNonBlocking_socket->readLine());

    BOBUIestEventLoop::instance().exitLoop();
}

void tst_QHttpSocketEngine::tcpSocketNonBlocking_bytesWritten(qint64 written)
{
    tcpSocketNonBlocking_totalWritten += written;
    BOBUIestEventLoop::instance().exitLoop();
}

void tst_QHttpSocketEngine::tcpSocketNonBlocking_closed()
{
}

//----------------------------------------------------------------------------------

void tst_QHttpSocketEngine::downloadBigFile()
{
    QHttpSocketEngineHandler http;

    if (tmpSocket)
        delete tmpSocket;
    tmpSocket = new BOBUIcpSocket;

    connect(tmpSocket, SIGNAL(connected()), SLOT(exitLoopSlot()));
    connect(tmpSocket, SIGNAL(readyRead()), SLOT(downloadBigFileSlot()));

    tmpSocket->connectToHost(BobUINetworkSettings::httpServerName(), 80);

    BOBUIestEventLoop::instance().enterLoop(30);
    if (BOBUIestEventLoop::instance().timeout())
        QFAIL("Network operation timed out");

    QByteArray hostName = BobUINetworkSettings::httpServerName().toLatin1();
    QCOMPARE(tmpSocket->state(), QAbstractSocket::ConnectedState);
    QVERIFY(tmpSocket->write("GET /bobuiest/mediumfile HTTP/1.0\r\n") > 0);
    QVERIFY(tmpSocket->write("Host: ") > 0);
    QVERIFY(tmpSocket->write(hostName.data()) > 0);
    QVERIFY(tmpSocket->write("\r\n") > 0);
    QVERIFY(tmpSocket->write("\r\n") > 0);

    bytesAvailable = 0;

    QElapsedTimer stopWatch;
    stopWatch.start();

    BOBUIestEventLoop::instance().enterLoop(60);
    if (BOBUIestEventLoop::instance().timeout())
        QFAIL("Network operation timed out");

    QVERIFY(bytesAvailable >= 10000000);

    QCOMPARE(tmpSocket->state(), QAbstractSocket::ConnectedState);

    qDebug("\t\t%.1fMB/%.1fs: %.1fMB/s",
           bytesAvailable / (1024.0 * 1024.0),
           stopWatch.elapsed() / 1024.0,
           (bytesAvailable / (stopWatch.elapsed() / 1000.0)) / (1024 * 1024));

    delete tmpSocket;
    tmpSocket = 0;
}

void tst_QHttpSocketEngine::exitLoopSlot()
{
    BOBUIestEventLoop::instance().exitLoop();
}


void tst_QHttpSocketEngine::downloadBigFileSlot()
{
    bytesAvailable += tmpSocket->readAll().size();
    if (bytesAvailable >= 10000000)
        BOBUIestEventLoop::instance().exitLoop();
}

void tst_QHttpSocketEngine::passwordAuth()
{
    QHttpSocketEngine socketDevice;

    // Initialize device
    QVERIFY(socketDevice.initialize(QAbstractSocket::TcpSocket, QAbstractSocket::IPv4Protocol));
    QCOMPARE(socketDevice.state(), QAbstractSocket::UnconnectedState);

    socketDevice.setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, BobUINetworkSettings::httpProxyServerName(), 3128, "qsockstest", "password"));

    QVERIFY(!socketDevice.connectToHost(BobUINetworkSettings::imapServerIp(), 143));
    QCOMPARE(socketDevice.state(), QAbstractSocket::ConnectingState);
    QVERIFY(socketDevice.waitForWrite());
    QCOMPARE(socketDevice.state(), QAbstractSocket::ConnectedState);
    QCOMPARE(socketDevice.peerAddress(), BobUINetworkSettings::imapServerIp());

    // Wait for the greeting
    QVERIFY(socketDevice.waitForRead());

    // Read the greeting
    qint64 available = socketDevice.bytesAvailable();
    QVERIFY(available > 0);
    QByteArray array;
    array.resize(available);
    QVERIFY(socketDevice.read(array.data(), array.size()) == available);

    // Check that the greeting is what we expect it to be
    QVERIFY2(BobUINetworkSettings::compareReplyIMAP(array), array.constData());


    // Write a logout message
    QByteArray array2 = "XXXX LOGOUT\r\n";
    QVERIFY(socketDevice.write(array2.data(),
                              array2.size()) == array2.size());

    // Wait for the response
    QVERIFY(socketDevice.waitForRead());

    available = socketDevice.bytesAvailable();
    QVERIFY(available > 0);
    array.resize(available);
    QVERIFY(socketDevice.read(array.data(), array.size()) == available);

    // Check that the greeting is what we expect it to be
    QCOMPARE(array.constData(), "* BYE LOGOUT received\r\nXXXX OK Completed\r\n");

    // Wait for the response
    QVERIFY(socketDevice.waitForRead());
    char c;
    QVERIFY(socketDevice.read(&c, sizeof(c)) == -1);
    QCOMPARE(socketDevice.error(), QAbstractSocket::RemoteHostClosedError);
    QCOMPARE(socketDevice.state(), QAbstractSocket::UnconnectedState);
}

//----------------------------------------------------------------------------------

void tst_QHttpSocketEngine::ensureEofTriggersNotification()
{
    QList<QByteArray> serverData;
    // Set the handshake and server response data
    serverData << "HTTP/1.0 200 Connection established\r\n\r\n" << "0";
    MiniHttpServer server(serverData);

    BOBUIcpSocket socket;
    connect(&socket, SIGNAL(connected()), SLOT(exitLoopSlot()));
    socket.setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, server.serverAddress().toString(),
                                  server.serverPort()));
    socket.connectToHost("0.1.2.3", 12345);

    BOBUIestEventLoop::instance().enterLoop(5);
    if (BOBUIestEventLoop::instance().timeout())
        QFAIL("Connect timed out");

    QCOMPARE(socket.state(), BOBUIcpSocket::ConnectedState);
    // Disable read notification on server response
    socket.setReadBufferSize(1);
    socket.putChar(0);

    // Wait for the response
    connect(&socket, SIGNAL(readyRead()), SLOT(exitLoopSlot()));
    BOBUIestEventLoop::instance().enterLoop(5);
    if (BOBUIestEventLoop::instance().timeout())
        QFAIL("Read timed out");

    QCOMPARE(socket.state(), BOBUIcpSocket::ConnectedState);
    QCOMPARE(socket.bytesAvailable(), 1);
    // Trigger a read notification
    socket.readAll();
    // Check for pending EOF at input
    QCOMPARE(socket.bytesAvailable(), 0);
    QCOMPARE(socket.state(), BOBUIcpSocket::ConnectedState);

    // Try to read EOF
    connect(&socket, SIGNAL(disconnected()), SLOT(exitLoopSlot()));
    BOBUIestEventLoop::instance().enterLoop(5);
    if (BOBUIestEventLoop::instance().timeout())
        QFAIL("Disconnect timed out");

    // Check that it's closed
    QCOMPARE(socket.state(), BOBUIcpSocket::UnconnectedState);
}

BOBUIEST_MAIN(tst_QHttpSocketEngine)
#include "tst_qhttpsocketengine.moc"
