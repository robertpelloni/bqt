// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUICPSERVER_H
#define BOBUICPSERVER_H

#include <BobUINetwork/bobuinetworkglobal.h>
#include <BobUICore/qobject.h>
#include <BobUINetwork/qabstractsocket.h>
#include <BobUINetwork/qhostaddress.h>

BOBUI_BEGIN_NAMESPACE


class BOBUIcpServerPrivate;
#ifndef BOBUI_NO_NETWORKPROXY
class QNetworkProxy;
#endif
class BOBUIcpSocket;

class Q_NETWORK_EXPORT BOBUIcpServer : public QObject
{
    Q_OBJECT
public:
    explicit BOBUIcpServer(QObject *parent = nullptr);
    virtual ~BOBUIcpServer();

    bool listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);
    void close();

    bool isListening() const;

    void setMaxPendingConnections(int numConnections);
    int maxPendingConnections() const;

    void setListenBacklogSize(int size);
    int listenBacklogSize() const;

    quint16 serverPort() const;
    QHostAddress serverAddress() const;

    qintptr socketDescriptor() const;
    bool setSocketDescriptor(qintptr socketDescriptor);

    bool waitForNewConnection(int msec = 0, bool *timedOut = nullptr);
    virtual bool hasPendingConnections() const;
    virtual BOBUIcpSocket *nextPendingConnection();

    QAbstractSocket::SocketError serverError() const;
    QString errorString() const;

    void pauseAccepting();
    void resumeAccepting();

#ifndef BOBUI_NO_NETWORKPROXY
    void setProxy(const QNetworkProxy &networkProxy);
    QNetworkProxy proxy() const;
#endif

protected:
    virtual void incomingConnection(qintptr handle);
    void addPendingConnection(BOBUIcpSocket* socket);

    BOBUIcpServer(QAbstractSocket::SocketType socketType, BOBUIcpServerPrivate &dd,
               QObject *parent = nullptr);

Q_SIGNALS:
    void newConnection();
    void pendingConnectionAvailable(QPrivateSignal);
    void acceptError(QAbstractSocket::SocketError socketError);

private:
    Q_DISABLE_COPY(BOBUIcpServer)
    Q_DECLARE_PRIVATE(BOBUIcpServer)
};

BOBUI_END_NAMESPACE

#endif // BOBUICPSERVER_H
