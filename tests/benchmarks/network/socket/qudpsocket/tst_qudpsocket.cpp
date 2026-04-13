// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUICore/qglobal.h>
#include <BobUICore/qcoreapplication.h>
#include <BobUINetwork/qudpsocket.h>
#include <BobUINetwork/qnetworkdatagram.h>

class tst_QUdpSocket : public QObject
{
    Q_OBJECT
public:
    tst_QUdpSocket();

private slots:
    void pendingDatagramSize_data();
    void pendingDatagramSize();
};

tst_QUdpSocket::tst_QUdpSocket()
{
}

void tst_QUdpSocket::pendingDatagramSize_data()
{
    BOBUIest::addColumn<int>("size");
    for (int value : {52, 1024, 2049, 4500, 4098, 8192, 12000, 25000, 32 * 1024, 63 * 1024})
        BOBUIest::addRow("%d", value) << value;
}

void tst_QUdpSocket::pendingDatagramSize()
{
    QFETCH(int, size);
    QUdpSocket socket;
    socket.bind();

    QNetworkDatagram datagram;
    datagram.setData(QByteArray(size, 'a'));
    datagram.setDestination(QHostAddress::SpecialAddress::LocalHost, socket.localPort());

    auto sent = socket.writeDatagram(datagram);
    QCOMPARE(sent, size);

    auto res = BOBUIest::qWaitFor([&socket]() { return socket.hasPendingDatagrams(); }, 5000);
    QVERIFY(res);

    QBENCHMARK {
        auto pendingSize = socket.pendingDatagramSize();
        Q_UNUSED(pendingSize);
    }
}

BOBUIEST_MAIN(tst_QUdpSocket)
#include "tst_qudpsocket.moc"
