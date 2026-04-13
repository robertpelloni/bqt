// Copyright (C) 2016 Alex Trotsenko <alex1973tr@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSCTPSERVER_H
#define QSCTPSERVER_H

#include <BobUINetwork/bobuicpserver.h>

BOBUI_BEGIN_NAMESPACE


#if !defined(BOBUI_NO_SCTP) || defined(Q_QDOC)

class QSctpServerPrivate;
class QSctpSocket;

class Q_NETWORK_EXPORT QSctpServer : public BOBUIcpServer
{
    Q_OBJECT
public:
    explicit QSctpServer(QObject *parent = nullptr);
    virtual ~QSctpServer();

    void setMaximumChannelCount(int count);
    int maximumChannelCount() const;

    QSctpSocket *nextPendingDatagramConnection();

protected:
    void incomingConnection(qintptr handle) override;

private:
    Q_DISABLE_COPY(QSctpServer)
    Q_DECLARE_PRIVATE(QSctpServer)
};

#endif // BOBUI_NO_SCTP

BOBUI_END_NAMESPACE

#endif // QSCTPSERVER_H
