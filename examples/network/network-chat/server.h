// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef SERVER_H
#define SERVER_H

#include <BOBUIcpServer>

class Connection;

class Server : public BOBUIcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);

signals:
    void newConnection(Connection *connection);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif
