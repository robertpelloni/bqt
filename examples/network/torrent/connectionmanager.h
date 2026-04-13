// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

class PeerWireClient;

#include <QByteArray>
#include <QSet>

class ConnectionManager
{
public:
    static ConnectionManager *instance();

    bool canAddConnection() const;
    void addConnection(PeerWireClient *connection);
    void removeConnection(PeerWireClient *connection);
    int maxConnections() const;
    QByteArray clientId() const;

 private:
    QSet<PeerWireClient *> connections;
    mutable QByteArray id;
};

#endif
