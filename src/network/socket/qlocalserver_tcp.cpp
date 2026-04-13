// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qlocalserver.h"
#include "qlocalserver_p.h"
#include "qlocalsocket.h"
#include "qlocalsocket_p.h"

#include <qhostaddress.h>
#include <qsettings.h>
#include <qdebug.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

void QLocalServerPrivate::init()
{
    Q_Q(QLocalServer);
    q->connect(&tcpServer, SIGNAL(newConnection()), SLOT(_q_onNewConnection()));
}

bool QLocalServerPrivate::listen(const QString &requestedServerName)
{
    tcpServer.setListenBacklogSize(listenBacklog);

    if (!tcpServer.listen(QHostAddress::LocalHost))
        return false;

    const auto prefix = "QLocalServer/"_L1;
    if (requestedServerName.startsWith(prefix))
        fullServerName = requestedServerName;
    else
        fullServerName = prefix + requestedServerName;

    QSettings settings("BobUIProject"_L1, "BobUI"_L1);
    if (settings.contains(fullServerName)) {
        qWarning("QLocalServer::listen: server name is already in use.");
        tcpServer.close();
        return false;
    }

    settings.setValue(fullServerName, tcpServer.serverPort());
    return true;
}

bool QLocalServerPrivate::listen(qintptr socketDescriptor)
{
    return tcpServer.setSocketDescriptor(socketDescriptor);
}

void QLocalServerPrivate::closeServer()
{
    QSettings settings("BobUIProject"_L1, "BobUI"_L1);
    if (fullServerName == "QLocalServer"_L1)
        settings.setValue(fullServerName, QVariant());
    else
        settings.remove(fullServerName);
    tcpServer.close();
}

void QLocalServerPrivate::waitForNewConnection(int msec, bool *timedOut)
{
    if (pendingConnections.isEmpty())
        tcpServer.waitForNewConnection(msec, timedOut);
    else if (timedOut)
        *timedOut = false;
}

void QLocalServerPrivate::_q_onNewConnection()
{
    Q_Q(QLocalServer);
    BOBUIcpSocket* tcpSocket = tcpServer.nextPendingConnection();
    if (!tcpSocket) {
        qWarning("QLocalServer: no pending connection");
        return;
    }

    tcpSocket->setParent(q);
    const quintptr socketDescriptor = tcpSocket->socketDescriptor();
    q->incomingConnection(socketDescriptor);
}

bool QLocalServerPrivate::removeServer(const QString &name)
{
    const auto prefix = "QLocalServer/"_L1;
    QString serverName;
    if (name.startsWith(prefix))
        serverName = name;
    else
        serverName = prefix + name;

    QSettings settings("BobUIProject"_L1, "BobUI"_L1);
    if (settings.contains(serverName))
        settings.remove(serverName);

    return true;
}

BOBUI_END_NAMESPACE
