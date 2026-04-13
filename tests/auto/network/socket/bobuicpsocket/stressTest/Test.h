// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#ifndef TEST_H
#define TEST_H

//------------------------------------------------------------------------------

#include <BOBUIcpServer>
#include <BOBUIcpSocket>

//------------------------------------------------------------------------------
class My4Socket : public BOBUIcpSocket
{
    Q_OBJECT
public:
    My4Socket(QObject *parent);

    void sendTest(quint32 num);
    bool safeShutDown;

private slots:
    void read();
    void closed();
};

//------------------------------------------------------------------------------
class My4Server : public BOBUIcpServer
{
    Q_OBJECT
public:
    My4Server(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socket) override;

private slots:
    void stopServer();

private:
    My4Socket *m_socket;
};

//------------------------------------------------------------------------------
class Test : public QObject
{
    Q_OBJECT

public:
    enum Type {
        BobUI4Client,
        BobUI4Server,
    };
    Test(Type type);
};

//------------------------------------------------------------------------------
#endif  // TEST_H
