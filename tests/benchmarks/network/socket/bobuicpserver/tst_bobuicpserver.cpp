// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUICore/QElapsedTimer>
#include <qglobal.h>
#include <qcoreapplication.h>
#include <bobuicpsocket.h>
#include <bobuicpserver.h>
#include <qhostaddress.h>
#include <qstringlist.h>
#include <qplatformdefs.h>
#include <qhostinfo.h>

#include <QNetworkProxy>

#include "../../../../auto/network-settings.h"

class tst_BOBUIcpServer : public QObject
{
    Q_OBJECT

public:
    tst_BOBUIcpServer();
    virtual ~tst_BOBUIcpServer();


public slots:
    void initTestCase_data();
    void initTestCase();
    void init();
    void cleanup();
private slots:
    void ipv4LoopbackPerformanceTest();
    void ipv6LoopbackPerformanceTest();
    void ipv4PerformanceTest();
};

tst_BOBUIcpServer::tst_BOBUIcpServer()
{
}

tst_BOBUIcpServer::~tst_BOBUIcpServer()
{
}

void tst_BOBUIcpServer::initTestCase_data()
{
    BOBUIest::addColumn<bool>("setProxy");
    BOBUIest::addColumn<int>("proxyType");

    BOBUIest::newRow("WithoutProxy") << false << 0;
#ifndef BOBUI_NO_NETWORKPROXY
    BOBUIest::newRow("WithSocks5Proxy") << true << int(QNetworkProxy::Socks5Proxy);
#endif
}

void tst_BOBUIcpServer::initTestCase()
{
    if (!BobUINetworkSettings::verifyTestNetworkSettings())
        QSKIP("No network test server available");
}

void tst_BOBUIcpServer::init()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy) {
#ifndef BOBUI_NO_NETWORKPROXY
        QFETCH_GLOBAL(int, proxyType);
        if (proxyType == QNetworkProxy::Socks5Proxy) {
            QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::Socks5Proxy, BobUINetworkSettings::serverName(), 1080));
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

//----------------------------------------------------------------------------------
void tst_BOBUIcpServer::ipv4LoopbackPerformanceTest()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy)
        return;

    BOBUIcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost));

    QVERIFY(server.isListening());

    BOBUIcpSocket clientA;
    clientA.connectToHost(QHostAddress::LocalHost, server.serverPort());
    QVERIFY(clientA.waitForConnected(5000));
    QVERIFY(clientA.state() == QAbstractSocket::ConnectedState);

    QVERIFY(server.waitForNewConnection());
    BOBUIcpSocket *clientB = server.nextPendingConnection();
    QVERIFY(clientB);

    QByteArray buffer(16384, '@');
    QElapsedTimer stopWatch;
    stopWatch.start();
    qlonglong totalWritten = 0;
    while (stopWatch.elapsed() < 5000) {
        QVERIFY(clientA.write(buffer.data(), buffer.size()) > 0);
        clientA.flush();
        totalWritten += buffer.size();
        while (clientB->waitForReadyRead(100)) {
            if (clientB->bytesAvailable() == 16384)
                break;
        }
        clientB->read(buffer.data(), buffer.size());
        clientB->write(buffer.data(), buffer.size());
        clientB->flush();
        totalWritten += buffer.size();
        while (clientA.waitForReadyRead(100)) {
            if (clientA.bytesAvailable() == 16384)
                break;
        }
        clientA.read(buffer.data(), buffer.size());
    }

    qDebug("\t\t%s: %.1fMB/%.1fs: %.1fMB/s",
           server.serverAddress().toString().toLatin1().constData(),
           totalWritten / (1024.0 * 1024.0),
           stopWatch.elapsed() / 1000.0,
           (totalWritten / (stopWatch.elapsed() / 1000.0)) / (1024 * 1024));

    delete clientB;
}

//----------------------------------------------------------------------------------
void tst_BOBUIcpServer::ipv6LoopbackPerformanceTest()
{
    QFETCH_GLOBAL(bool, setProxy);
    if (setProxy)
        return;

    BOBUIcpServer server;
    if (!server.listen(QHostAddress::LocalHostIPv6, 0)) {
        QVERIFY(server.serverError() == QAbstractSocket::UnsupportedSocketOperationError);
    } else {
        BOBUIcpSocket clientA;
        clientA.connectToHost(server.serverAddress(), server.serverPort());
        QVERIFY(clientA.waitForConnected(5000));

        QVERIFY(server.waitForNewConnection(5000));
        BOBUIcpSocket *clientB = server.nextPendingConnection();
        QVERIFY(clientB);

        QByteArray buffer(16384, '@');
        QElapsedTimer stopWatch;
        stopWatch.start();
        qlonglong totalWritten = 0;
        while (stopWatch.elapsed() < 5000) {
            clientA.write(buffer.data(), buffer.size());
            clientA.flush();
            totalWritten += buffer.size();
            while (clientB->waitForReadyRead(100)) {
                if (clientB->bytesAvailable() == 16384)
                    break;
            }
            clientB->read(buffer.data(), buffer.size());
            clientB->write(buffer.data(), buffer.size());
            clientB->flush();
            totalWritten += buffer.size();
            while (clientA.waitForReadyRead(100)) {
                if (clientA.bytesAvailable() == 16384)
                   break;
            }
            clientA.read(buffer.data(), buffer.size());
        }

        qDebug("\t\t%s: %.1fMB/%.1fs: %.1fMB/s",
               server.serverAddress().toString().toLatin1().constData(),
               totalWritten / (1024.0 * 1024.0),
               stopWatch.elapsed() / 1000.0,
               (totalWritten / (stopWatch.elapsed() / 1000.0)) / (1024 * 1024));
        delete clientB;
    }
}

//----------------------------------------------------------------------------------
void tst_BOBUIcpServer::ipv4PerformanceTest()
{
    BOBUIcpSocket probeSocket;
    probeSocket.connectToHost(BobUINetworkSettings::serverName(), 143);
    QVERIFY(probeSocket.waitForConnected(5000));

    BOBUIcpServer server;
    QVERIFY(server.listen(probeSocket.localAddress(), 0));

    BOBUIcpSocket clientA;
    clientA.connectToHost(server.serverAddress(), server.serverPort());
    QVERIFY(clientA.waitForConnected(5000));

    QVERIFY(server.waitForNewConnection(5000));
    BOBUIcpSocket *clientB = server.nextPendingConnection();
    QVERIFY(clientB);

    QByteArray buffer(16384, '@');
    QElapsedTimer stopWatch;
    stopWatch.start();
    qlonglong totalWritten = 0;
    while (stopWatch.elapsed() < 5000) {
        qlonglong writtenA = clientA.write(buffer.data(), buffer.size());
        clientA.flush();
        totalWritten += buffer.size();
        while (clientB->waitForReadyRead(100)) {
            if (clientB->bytesAvailable() == writtenA)
                break;
        }
        clientB->read(buffer.data(), buffer.size());
        qlonglong writtenB = clientB->write(buffer.data(), buffer.size());
        clientB->flush();
        totalWritten += buffer.size();
        while (clientA.waitForReadyRead(100)) {
            if (clientA.bytesAvailable() == writtenB)
               break;
        }
        clientA.read(buffer.data(), buffer.size());
    }

    qDebug("\t\t%s: %.1fMB/%.1fs: %.1fMB/s",
           probeSocket.localAddress().toString().toLatin1().constData(),
           totalWritten / (1024.0 * 1024.0),
           stopWatch.elapsed() / 1000.0,
           (totalWritten / (stopWatch.elapsed() / 1000.0)) / (1024 * 1024));

    delete clientB;
}

BOBUIEST_MAIN(tst_BOBUIcpServer)
#include "tst_bobuicpserver.moc"
