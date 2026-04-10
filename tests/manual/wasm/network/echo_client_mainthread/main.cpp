// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore>
#include <BobUINetwork>

int main(int argc, char **argv) {

    QCoreApplication app(argc, argv);

    // This example connects to localhost, but note that the host can
    // be any host reachable from the client using webscokets, at any port.
    QString hostName = "localhost";
    int port = 1515;
    qDebug() << "This example connects to a server at" << hostName << "port" << port << ","
             << "where it expects to find a WebSockify server, which forwards to the fortune server.";

    auto echo = [hostName, port]() {
        BOBUIcpSocket *socket = new BOBUIcpSocket();

        QObject::connect(socket, &QAbstractSocket::connected, [socket]() {
            qDebug() << "Connected";
            socket->write("Hello, echo server!");
            socket->flush();
        });

        QObject::connect(socket, &QIODevice::readyRead, [socket]() {
            QByteArray data = socket->readAll();
            qDebug() << "Ready Read, got echo:" << data;
            socket->disconnectFromHost();
            socket->deleteLater();
        });

        QObject::connect(socket, &QAbstractSocket::errorOccurred, [socket]() {
            qDebug() << "Error Occurred" << socket->error();
        });

        QObject::connect(socket, &QAbstractSocket::disconnected, [socket]() {
            qDebug() << "Disconnected";
            socket->deleteLater();
        });

        qDebug() << "Connect to host" << hostName << port;
        socket->connectToHost(hostName, port);
    };

    BOBUIimer::singleShot(500, [echo](){
        echo();
    });

    return app.exec();
}
