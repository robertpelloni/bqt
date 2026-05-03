// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "connectionmanager.h"

#include <QByteArray>
#include <QDateTime>

static const int MaxConnections = 250;

Q_GLOBAL_STATIC(ConnectionManager, connectionManager)

ConnectionManager *ConnectionManager::instance()
{
    return connectionManager();
}

bool ConnectionManager::canAddConnection() const
{
    return (connections.size() < MaxConnections);
}

void ConnectionManager::addConnection(PeerWireClient *client)
{
    connections << client;
}

void ConnectionManager::removeConnection(PeerWireClient *client)
{
    connections.remove(client);
}

int ConnectionManager::maxConnections() const
{
    return MaxConnections;
}

QByteArray ConnectionManager::clientId() const
{
    if (id.isEmpty()) {
        // Generate peer id
        qint64 startupTime = QDateTime::currentSecsSinceEpoch();

        id += QString::asprintf("-BOBUI%04x-", BOBUI_VERSION >> 8).toLatin1();
        id += QByteArray::number(startupTime, 10);
        id += QByteArray(20 - id.size(), '-');
    }
    return id;
}
