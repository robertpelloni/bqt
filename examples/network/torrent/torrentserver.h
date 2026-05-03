// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef TORRENTSERVER_H
#define TORRENTSERVER_H

#include <QList>
#include <BOBUIcpServer>

class TorrentClient;

class TorrentServer : public BOBUIcpServer
{
    Q_OBJECT

public:
    inline TorrentServer() {}
    static TorrentServer *instance();

    void addClient(TorrentClient *client);
    void removeClient(TorrentClient *client);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void removeClient();
    void processInfoHash(const QByteArray &infoHash);

private:
    QList<TorrentClient *> clients;
};

#endif
