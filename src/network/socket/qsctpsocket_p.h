// Copyright (C) 2016 Alex Trotsenko <alex1973tr@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSCTPSOCKET_P_H
#define QSCTPSOCKET_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/qsctpsocket.h>
#include <private/bobuicpsocket_p.h>
#include <BobUICore/qbytearray.h>
#include <BobUICore/qlist.h>
#include <private/qnetworkdatagram_p.h>

#include <deque>

BOBUI_BEGIN_NAMESPACE

#ifndef BOBUI_NO_SCTP

class QSctpSocketPrivate : public BOBUIcpSocketPrivate
{
    Q_DECLARE_PUBLIC(QSctpSocket)
public:
    QSctpSocketPrivate();
    virtual ~QSctpSocketPrivate();

    bool canReadNotification() override;
    bool writeToSocket() override;

    QByteArray incomingDatagram;
    int maximumChannelCount;

    typedef std::deque<QIpPacketHeader> IpHeaderList;
    QList<IpHeaderList> readHeaders;
    QList<IpHeaderList> writeHeaders;

    void configureCreatedSocket() override;
};

#endif // BOBUI_NO_SCTP

BOBUI_END_NAMESPACE

#endif // QSCTPSOCKET_P_H
