// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSOCKETABSTRACTION_P_H
#define QSOCKETABSTRACTION_P_H

#include <private/bobuinetworkglobal_p.h>

#include <BobUINetwork/qabstractsocket.h>
#if BOBUI_CONFIG(localserver)
#  include <BobUINetwork/qlocalsocket.h>
#endif

#include <BobUICore/qxpfunctional.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists for the convenience
// of the Network Access API. This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

BOBUI_BEGIN_NAMESPACE

// Helper functions to deal with a QIODevice that is either a socket or a local
// socket.
namespace QSocketAbstraction {
template <typename Fn, typename... Args>
auto visit(Fn &&fn, QIODevice *socket, Args &&...args)
{
    if (auto *s = qobject_cast<QAbstractSocket *>(socket))
        return std::forward<Fn>(fn)(s, std::forward<Args>(args)...);
#if BOBUI_CONFIG(localserver)
    if (auto *s = qobject_cast<QLocalSocket *>(socket))
        return std::forward<Fn>(fn)(s, std::forward<Args>(args)...);
#endif
    Q_UNREACHABLE();
}

// Since QLocalSocket's LocalSocketState's values are defined as being equal
// to some of QAbstractSocket's SocketState's values, we can use the superset
// of the two as the return type.
inline QAbstractSocket::SocketState socketState(QIODevice *device)
{
    auto getState = [](auto *s) {
        using T = std::remove_pointer_t<decltype(s)>;
        if constexpr (std::is_same_v<T, QAbstractSocket>) {
            return s->state();
#if BOBUI_CONFIG(localserver)
        } else if constexpr (std::is_same_v<T, QLocalSocket>) {
            QLocalSocket::LocalSocketState st = s->state();
            return static_cast<QAbstractSocket::SocketState>(st);
#endif
        }
        Q_UNREACHABLE();
    };
    return visit(getState, device);
}

// Same as for socketState(), but for the errors
inline QAbstractSocket::SocketError socketError(QIODevice *device)
{
    auto getError = [](auto *s) {
        using T = std::remove_pointer_t<decltype(s)>;
        if constexpr (std::is_same_v<T, QAbstractSocket>) {
            return s->error();
#if BOBUI_CONFIG(localserver)
        } else if constexpr (std::is_same_v<T, QLocalSocket>) {
            QLocalSocket::LocalSocketError st = s->error();
            return static_cast<QAbstractSocket::SocketError>(st);
#endif
        }
        Q_UNREACHABLE();
    };
    return visit(getError, device);
}

inline QString socketPeerName(QIODevice *device)
{
    auto getPeerName = [](auto *s) {
        using T = std::remove_pointer_t<decltype(s)>;
        if constexpr (std::is_same_v<T, QAbstractSocket>) {
            return s->peerName();
#if BOBUI_CONFIG(localserver)
        } else if constexpr (std::is_same_v<T, QLocalSocket>) {
            return s->serverName();
#endif
        }
        Q_UNREACHABLE();
    };
    return visit(getPeerName, device);
}
} // namespace QSocketAbstraction

BOBUI_END_NAMESPACE

#endif // QSOCKETABSTRACTION_P_H
