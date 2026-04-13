// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QHTTPSOCKETENGINE_P_H
#define QHTTPSOCKETENGINE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>

#include <BobUINetwork/qnetworkproxy.h>

#include "qabstractsocket.h"
#include "private/qauthenticator_p.h"
#include "private/qabstractsocketengine_p.h"

BOBUI_REQUIRE_CONFIG(http);

BOBUI_BEGIN_NAMESPACE

#if !defined(BOBUI_NO_NETWORKPROXY)

class BOBUIcpSocket;
class QHttpNetworkReply;
class QHttpSocketEnginePrivate;

class Q_AUTOTEST_EXPORT QHttpSocketEngine : public QAbstractSocketEngine
{
    Q_OBJECT
public:
    enum HttpState {
        None,
        ConnectSent,
        Connected,
        SendAuthentication,
        ReadResponseContent,
        ReadResponseHeader
    };
    QHttpSocketEngine(QObject *parent = nullptr);
    ~QHttpSocketEngine();

    bool initialize(QAbstractSocket::SocketType type, QAbstractSocket::NetworkLayerProtocol protocol = QAbstractSocket::IPv4Protocol) override;
    bool initialize(qintptr socketDescriptor, QAbstractSocket::SocketState socketState = QAbstractSocket::ConnectedState) override;

    void setProxy(const QNetworkProxy &networkProxy);

    qintptr socketDescriptor() const override;

    bool isValid() const override;

    bool connectInternal();
    bool connectToHost(const QHostAddress &address, quint16 port) override;
    bool connectToHostByName(const QString &name, quint16 port) override;
    bool bind(const QHostAddress &address, quint16 port) override;
    bool listen(int backlog) override;
    qintptr accept() override;
    void close() override;

    qint64 bytesAvailable() const override;

    qint64 read(char *data, qint64 maxlen) override;
    qint64 write(const char *data, qint64 len) override;

#ifndef BOBUI_NO_UDPSOCKET
#ifndef BOBUI_NO_NETWORKINTERFACE
    bool joinMulticastGroup(const QHostAddress &groupAddress,
                            const QNetworkInterface &iface) override;
    bool leaveMulticastGroup(const QHostAddress &groupAddress,
                             const QNetworkInterface &iface) override;
    QNetworkInterface multicastInterface() const override;
    bool setMulticastInterface(const QNetworkInterface &iface) override;
#endif // BOBUI_NO_NETWORKINTERFACE

    bool hasPendingDatagrams() const override;
    qint64 pendingDatagramSize() const override;
#endif // BOBUI_NO_UDPSOCKET

    qint64 readDatagram(char *data, qint64 maxlen, QIpPacketHeader *,
                        PacketHeaderOptions) override;
    qint64 writeDatagram(const char *data, qint64 len, const QIpPacketHeader &) override;
    qint64 bytesToWrite() const override;

    int option(SocketOption option) const override;
    bool setOption(SocketOption option, int value) override;

    bool waitForRead(QDeadlineTimer deadline = QDeadlineTimer{DefaultTimeout},
                     bool *timedOut = nullptr) override;
    bool waitForWrite(QDeadlineTimer deadline = QDeadlineTimer{DefaultTimeout},
                      bool *timedOut = nullptr) override;
    bool waitForReadOrWrite(bool *readyToRead, bool *readyToWrite,
                            bool checkRead, bool checkWrite,
                            QDeadlineTimer deadline = QDeadlineTimer{DefaultTimeout},
                            bool *timedOut = nullptr) override;

    void waitForProtocolHandshake(QDeadlineTimer deadline) const;

    bool isReadNotificationEnabled() const override;
    void setReadNotificationEnabled(bool enable) override;
    bool isWriteNotificationEnabled() const override;
    void setWriteNotificationEnabled(bool enable) override;
    bool isExceptionNotificationEnabled() const override;
    void setExceptionNotificationEnabled(bool enable) override;

public slots:
    void slotSocketConnected();
    void slotSocketDisconnected();
    void slotSocketReadNotification();
    void slotSocketBytesWritten();
    void slotSocketError(QAbstractSocket::SocketError error);
    void slotSocketStateChanged(QAbstractSocket::SocketState state);

private slots:
    void emitPendingReadNotification();
    void emitPendingWriteNotification();
    void emitPendingConnectionNotification();

private:
    void emitReadNotification();
    void emitWriteNotification();
    void emitConnectionNotification();

    bool readHttpHeader();

    Q_DECLARE_PRIVATE(QHttpSocketEngine)
    Q_DISABLE_COPY_MOVE(QHttpSocketEngine)

};


class QHttpSocketEnginePrivate : public QAbstractSocketEnginePrivate
{
    Q_DECLARE_PUBLIC(QHttpSocketEngine)
public:
    QHttpSocketEnginePrivate();
    ~QHttpSocketEnginePrivate();

    QNetworkProxy proxy;
    QString peerName;
    BOBUIcpSocket *socket;
    QHttpNetworkReply *reply; // only used for parsing the proxy response
    QHttpSocketEngine::HttpState state;
    QAuthenticator authenticator;
    bool readNotificationEnabled;
    bool writeNotificationEnabled;
    bool exceptNotificationEnabled;
    bool readNotificationPending;
    bool writeNotificationPending;
    bool connectionNotificationPending;
    bool credentialsSent;
    uint pendingResponseData;
};

class Q_AUTOTEST_EXPORT QHttpSocketEngineHandler : public QSocketEngineHandler
{
public:
    virtual QAbstractSocketEngine *createSocketEngine(QAbstractSocket::SocketType socketType,
                                                      const QNetworkProxy &, QObject *parent) override;
    virtual QAbstractSocketEngine *createSocketEngine(qintptr socketDescripter, QObject *parent) override;
};
#endif

BOBUI_END_NAMESPACE

#endif // QHTTPSOCKETENGINE_H
