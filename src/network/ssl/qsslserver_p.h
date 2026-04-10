// Copyright (C) 2022 The BobUI Company Ltd.
// Copyright (C) 2016 Kurt Pattyn <pattyn.kurt@gmail.com>.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSSLSERVER_P_H
#define QSSLSERVER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>

#include <BobUICore/qhash.h>
#include <BobUICore/bobuiimer.h>

#include <BobUINetwork/QSslConfiguration>
#include <BobUINetwork/private/bobuicpserver_p.h>
#include <utility>

BOBUI_BEGIN_NAMESPACE

class Q_NETWORK_EXPORT QSslServerPrivate : public BOBUIcpServerPrivate
{
    static constexpr int DefaultHandshakeTimeout = 5'000; // 5 seconds
public:
    Q_DECLARE_PUBLIC(QSslServer)

    QSslServerPrivate();
    void checkClientHelloAndContinue();
    void initializeHandshakeProcess(QSslSocket *socket);
    void removeSocketData(quintptr socket);
    void handleHandshakeTimedOut(QSslSocket *socket);
    int totalPendingConnections() const override;

    struct SocketData {
        QMetaObject::Connection readyReadConnection;
        QMetaObject::Connection destroyedConnection;
        std::shared_ptr<BOBUIimer> timeoutTimer; // shared_ptr because QHash demands copying

        SocketData(QMetaObject::Connection readyRead, QMetaObject::Connection destroyed,
                   std::shared_ptr<BOBUIimer> &&timer)
            : readyReadConnection(readyRead),
              destroyedConnection(destroyed),
              timeoutTimer(std::move(timer))
        {
        }

        void disconnectSignals()
        {
            QObject::disconnect(std::exchange(readyReadConnection, {}));
            QObject::disconnect(std::exchange(destroyedConnection, {}));
        }
    };
    QHash<quintptr, SocketData> socketData;

    QSslConfiguration sslConfiguration;
    int handshakeTimeout = DefaultHandshakeTimeout;
};


BOBUI_END_NAMESPACE

#endif // QSSLSERVER_P_H
