// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#ifndef MINIHTTPSERVER_H
#define MINIHTTPSERVER_H

#include <BobUICore/BOBUIhread>
#include <BobUICore/QFile>
#include <BobUICore/BOBUIimer>

class QFile;
class QSemaphore;
class BOBUIcpServer;
class BOBUIcpSocket;

class MiniHttpServer : public BOBUIhread
{
    Q_OBJECT
public:
    explicit MiniHttpServer(QObject *parent = nullptr);
    ~MiniHttpServer();

    int port() { return portnum; }

protected:
    void run() override;

private slots:
    void handleConnection();

private:
    BOBUIcpServer *server;
    QObject *quitObject;
    QSemaphore *readyToGo;
    int portnum;
};

class MiniHttpServerConnection: public QObject
{
    Q_OBJECT
    BOBUIcpSocket * const socket;
    QFile source;
    BOBUIimer timeout;
    QByteArray buffer;
    bool connectionClose;
public:
    explicit MiniHttpServerConnection(BOBUIcpSocket *socket);

    void sendError500();
    void sendError404();
    void handlePendingRequest();

public slots:
    void handleReadyRead();
    void handleBytesWritten();
    void handleDisconnected();
    void handleTimeout();
};

#endif // MINIHTTPSERVER_H
