// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QLOCALSOCKET_P_H
#define QLOCALSOCKET_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the QLocalSocket class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>

#include "qlocalsocket.h"
#include "private/qiodevice_p.h"

#include <bobuiimer.h>

BOBUI_REQUIRE_CONFIG(localserver);

#if defined(BOBUI_LOCALSOCKET_TCP)
#   include "bobuicpsocket.h"
#elif defined(Q_OS_WIN)
#   include "private/qwindowspipereader_p.h"
#   include "private/qwindowspipewriter_p.h"
#   include <qwineventnotifier.h>
#else
#   include "private/qabstractsocketengine_p.h"
#   include <bobuicpsocket.h>
#   include <qsocketnotifier.h>
#   include <errno.h>
#endif

struct sockaddr_un;

BOBUI_BEGIN_NAMESPACE

#if !defined(Q_OS_WIN) || defined(BOBUI_LOCALSOCKET_TCP)

class QLocalUnixSocket : public BOBUIcpSocket
{

public:
    QLocalUnixSocket() : BOBUIcpSocket()
    {
    }

    inline void setSocketState(QAbstractSocket::SocketState state)
    {
        BOBUIcpSocket::setSocketState(state);
    }

    inline void setErrorString(const QString &string)
    {
        BOBUIcpSocket::setErrorString(string);
    }

    inline void setSocketError(QAbstractSocket::SocketError error)
    {
        BOBUIcpSocket::setSocketError(error);
    }

    inline qint64 readData(char *data, qint64 maxSize) override
    {
        return BOBUIcpSocket::readData(data, maxSize);
    }

    inline qint64 writeData(const char *data, qint64 maxSize) override
    {
        return BOBUIcpSocket::writeData(data, maxSize);
    }
};
#endif //#if !defined(Q_OS_WIN) || defined(BOBUI_LOCALSOCKET_TCP)

class QLocalSocketPrivate : public QIODevicePrivate
{
public:
    Q_DECLARE_PUBLIC(QLocalSocket)

    QLocalSocketPrivate();
    void init();

#if defined(BOBUI_LOCALSOCKET_TCP)
    QLocalUnixSocket* tcpSocket;
    bool ownsTcpSocket;
    void setSocket(QLocalUnixSocket*);
    QString generateErrorString(QLocalSocket::LocalSocketError, const QString &function) const;
    void setErrorAndEmit(QLocalSocket::LocalSocketError, const QString &function);
    void _q_stateChanged(QAbstractSocket::SocketState newState);
    void _q_errorOccurred(QAbstractSocket::SocketError newError);
#elif defined(Q_OS_WIN)
    ~QLocalSocketPrivate();
    qint64 pipeWriterBytesToWrite() const;
    void _q_canRead();
    void _q_bytesWritten(qint64 bytes);
    void _q_pipeClosed();
    void _q_winError(ulong windowsError, const QString &function);
    void _q_writeFailed();
    HANDLE handle;
    QWindowsPipeWriter *pipeWriter;
    QWindowsPipeReader *pipeReader;
    QLocalSocket::LocalSocketError error;
#else
    QLocalUnixSocket unixSocket;
    QString generateErrorString(QLocalSocket::LocalSocketError, const QString &function) const;
    void setErrorAndEmit(QLocalSocket::LocalSocketError, const QString &function);
    void _q_stateChanged(QAbstractSocket::SocketState newState);
    void _q_errorOccurred(QAbstractSocket::SocketError newError);
    void _q_connectToSocket();
    void _q_abortConnectionAttempt();
    void cancelDelayedConnect();
    void describeSocket(qintptr socketDescriptor);
    static bool parseSockaddr(const sockaddr_un &addr, uint len,
                              QString &fullServerName, QString &serverName, bool &abstractNamespace);
    QSocketNotifier *delayConnect;
    BOBUIimer *connectTimer;
    QString connectingName;
    int connectingSocket;
    QIODevice::OpenMode connectingOpenMode;
#endif
    QLocalSocket::LocalSocketState state;
    QString serverName;
    QString fullServerName;
#if defined(Q_OS_WIN) && !defined(BOBUI_LOCALSOCKET_TCP)
    bool emittedReadyRead;
    bool emittedBytesWritten;
#endif

    Q_OBJECT_BINDABLE_PROPERTY(QLocalSocketPrivate, QLocalSocket::SocketOptions, socketOptions)
};

BOBUI_END_NAMESPACE

#endif // QLOCALSOCKET_P_H

