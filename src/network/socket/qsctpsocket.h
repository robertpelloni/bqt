// Copyright (C) 2016 Alex Trotsenko <alex1973tr@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSCTPSOCKET_H
#define QSCTPSOCKET_H

#include <BobUINetwork/bobuicpsocket.h>
#include <BobUINetwork/qnetworkdatagram.h>

BOBUI_BEGIN_NAMESPACE

#if !defined(BOBUI_NO_SCTP) || defined(Q_QDOC)

class QSctpSocketPrivate;

class Q_NETWORK_EXPORT QSctpSocket : public BOBUIcpSocket
{
    Q_OBJECT
public:
    explicit QSctpSocket(QObject *parent = nullptr);
    virtual ~QSctpSocket();

    void close() override;
    void disconnectFromHost() override;

    void setMaximumChannelCount(int count);
    int maximumChannelCount() const;
    bool isInDatagramMode() const;

    QNetworkDatagram readDatagram();
    bool writeDatagram(const QNetworkDatagram &datagram);

protected:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 readLineData(char *data, qint64 maxlen) override;

private:
    Q_DISABLE_COPY(QSctpSocket)
    Q_DECLARE_PRIVATE(QSctpSocket)
};

#endif // BOBUI_NO_SCTP

BOBUI_END_NAMESPACE

#endif // QSCTPSOCKET_H
