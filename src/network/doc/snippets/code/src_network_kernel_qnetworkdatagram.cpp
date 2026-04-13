// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
    void Server::readPendingDatagrams()
    {
        while (udpSocket->hasPendingDatagrams()) {
            QNetworkDatagram datagram = udpSocket->receiveDatagram();
            QByteArray replyData = processThePayload(datagram.data());
            udpSocket->writeDatagram(datagram.makeReply(replyData));
        }
    }
//! [0]

//! [1]
    udpSocket->writeDatagram(std::move(datagram).makeReply(replyData));
//! [1]
