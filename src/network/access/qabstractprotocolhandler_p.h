// Copyright (C) 2014 BlackBerry Limited. All rights reserved.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QABSTRACTPROTOCOLHANDLER_H
#define QABSTRACTPROTOCOLHANDLER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the Network Access API.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>

#include <BobUICore/bobuipreprocessorsupport.h>

BOBUI_REQUIRE_CONFIG(http);

BOBUI_BEGIN_NAMESPACE

class QHttpNetworkConnectionChannel;
class QHttpNetworkReply;
class QIODevice;
class QHttpNetworkConnection;

class QAbstractProtocolHandler {
public:
    QAbstractProtocolHandler(QHttpNetworkConnectionChannel *channel);
    virtual ~QAbstractProtocolHandler();

    virtual void _q_receiveReply() = 0;
    virtual void _q_readyRead() = 0;
    virtual bool sendRequest() = 0;
    // Called when the reply is being destroyed and removing itself from any other internals
    virtual bool tryRemoveReply(QHttpNetworkReply *reply)
    {
        Q_UNUSED(reply);
        // base implementation is a noop
        return false;
    }
    void setReply(QHttpNetworkReply *reply);

protected:
    QHttpNetworkConnectionChannel *m_channel;
    QHttpNetworkReply *m_reply;
    QIODevice *m_socket;
    QHttpNetworkConnection *m_connection;
};

BOBUI_END_NAMESPACE

#endif // QABSTRACTPROTOCOLHANDLER_H
