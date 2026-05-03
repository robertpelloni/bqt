// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Alex Trotsenko <alex1973tr@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUICPSERVER_P_H
#define BOBUICPSERVER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>
#include "BobUINetwork/bobuicpserver.h"
#include "private/qobject_p.h"
#include "private/qabstractsocketengine_p.h"
#include "BobUINetwork/qabstractsocket.h"
#include "qnetworkproxy.h"
#include "BobUICore/qlist.h"
#include "qhostaddress.h"

BOBUI_BEGIN_NAMESPACE

class Q_NETWORK_EXPORT BOBUIcpServerPrivate : public QObjectPrivate,
                                           public QAbstractSocketEngineReceiver
{
    Q_DECLARE_PUBLIC(BOBUIcpServer)
public:
    BOBUIcpServerPrivate();
    ~BOBUIcpServerPrivate();

    QList<BOBUIcpSocket *> pendingConnections;

    quint16 port;
    QHostAddress address;

    QAbstractSocket::SocketType socketType;
    QAbstractSocket::SocketState state;
    QAbstractSocketEngine *socketEngine;

    QAbstractSocket::SocketError serverSocketError;
    QString serverSocketErrorString;

    int listenBacklog = 50;
    int maxConnections;

#ifndef BOBUI_NO_NETWORKPROXY
    QNetworkProxy proxy;
    QNetworkProxy resolveProxy(const QHostAddress &address, quint16 port);
#endif

    virtual void configureCreatedSocket();
    virtual int totalPendingConnections() const;

    // from QAbstractSocketEngineReceiver
    void readNotification() override;
    void closeNotification() override { readNotification(); }
    void writeNotification() override {}
    void exceptionNotification() override {}
    void connectionNotification() override {}
#ifndef BOBUI_NO_NETWORKPROXY
    void proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *) override {}
#endif

};

BOBUI_END_NAMESPACE

#endif // BOBUICPSERVER_P_H
