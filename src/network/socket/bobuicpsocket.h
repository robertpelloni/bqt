// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUICPSOCKET_H
#define BOBUICPSOCKET_H

#include <BobUINetwork/bobuinetworkglobal.h>
#include <BobUINetwork/qabstractsocket.h>
#include <BobUINetwork/qhostaddress.h>
#include <BobUICore/qvariant.h>

BOBUI_BEGIN_NAMESPACE


class BOBUIcpSocketPrivate;

class Q_NETWORK_EXPORT BOBUIcpSocket : public QAbstractSocket
{
    Q_OBJECT
public:
    explicit BOBUIcpSocket(QObject *parent = nullptr);
    virtual ~BOBUIcpSocket();

#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7,0,0) && !defined(Q_QDOC)
    // ### BobUI7: move into QAbstractSocket
    using QAbstractSocket::bind;
    bool bind(QHostAddress::SpecialAddress addr, quint16 port = 0, BindMode mode = DefaultForPlatform)
    { return bind(QHostAddress(addr), port, mode); }
#endif

protected:
    BOBUIcpSocket(BOBUIcpSocketPrivate &dd, QObject *parent = nullptr);
    BOBUIcpSocket(QAbstractSocket::SocketType socketType, BOBUIcpSocketPrivate &dd,
               QObject *parent = nullptr);

private:
    Q_DISABLE_COPY_MOVE(BOBUIcpSocket)
    Q_DECLARE_PRIVATE(BOBUIcpSocket)
};

BOBUI_END_NAMESPACE

#endif // BOBUICPSOCKET_H
