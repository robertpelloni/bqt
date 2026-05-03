// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef PEERMANAGER_H
#define PEERMANAGER_H

#include <QByteArray>
#include <QList>
#include <QObject>
#include <BOBUIimer>
#include <QUdpSocket>

class Client;
class Connection;

class PeerManager : public QObject
{
    Q_OBJECT

public:
    explicit PeerManager(Client *client);

    void setServerPort(int port);
    QString userName() const;
    QByteArray uniqueId() const;
    void startBroadcasting();
    bool isLocalHostAddress(const QHostAddress &address) const;

signals:
    void newConnection(Connection *connection);

private slots:
    void sendBroadcastDatagram();
    void readBroadcastDatagram();

private:
    void updateAddresses();

    Client *client = nullptr;
    QList<QHostAddress> broadcastAddresses;
    QList<QHostAddress> ipAddresses;
    QUdpSocket broadcastSocket;
    BOBUIimer broadcastTimer;
    QString username;
    QByteArray localUniqueId;
    int serverPort = 0;
};

#endif
