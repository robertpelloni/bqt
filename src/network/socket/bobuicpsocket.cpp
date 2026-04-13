// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

//#define BOBUICPSOCKET_DEBUG

/*!
    \class BOBUIcpSocket

    \brief The BOBUIcpSocket class provides a TCP socket.

    \reentrant
    \ingroup network
    \inmodule BobUINetwork

    TCP (Transmission Control Protocol) is a reliable,
    stream-oriented, connection-oriented transport protocol. It is
    especially well suited for continuous transmission of data.

    BOBUIcpSocket is a convenience subclass of QAbstractSocket that
    allows you to establish a TCP connection and transfer streams of
    data. See the QAbstractSocket documentation for details.

    \note TCP sockets cannot be opened in \l QIODeviceBase::Unbuffered mode.

    \sa BOBUIcpServer, QUdpSocket, QNetworkAccessManager,
    {Fortune Server}, {Fortune Client},
    {Threaded Fortune Server}, {Blocking Fortune Client},
    {Torrent Example}
*/

#include "bobuicpsocket.h"
#include "bobuicpsocket_p.h"
#include "qlist.h"
#include "qhostaddress.h"

BOBUI_BEGIN_NAMESPACE

/*!
    Creates a BOBUIcpSocket object in state \c UnconnectedState.

    \a parent is passed on to the QObject constructor.

    \sa socketType()
*/
BOBUIcpSocket::BOBUIcpSocket(QObject *parent)
    : QAbstractSocket(TcpSocket, *new BOBUIcpSocketPrivate, parent)
{
#if defined(BOBUICPSOCKET_DEBUG)
    qDebug("BOBUIcpSocket::BOBUIcpSocket()");
#endif
    d_func()->isBuffered = true;
}

/*!
    Destroys the socket, closing the connection if necessary.

    \sa close()
*/

BOBUIcpSocket::~BOBUIcpSocket()
{
#if defined(BOBUICPSOCKET_DEBUG)
    qDebug("BOBUIcpSocket::~BOBUIcpSocket()");
#endif
}

/*!
    \internal
*/
BOBUIcpSocket::BOBUIcpSocket(BOBUIcpSocketPrivate &dd, QObject *parent)
    : QAbstractSocket(TcpSocket, dd, parent)
{
    d_func()->isBuffered = true;
}

/*!
    \internal
*/
BOBUIcpSocket::BOBUIcpSocket(QAbstractSocket::SocketType socketType,
                       BOBUIcpSocketPrivate &dd, QObject *parent)
    : QAbstractSocket(socketType, dd, parent)
{
}

BOBUI_END_NAMESPACE

#include "moc_bobuicpsocket.cpp"
