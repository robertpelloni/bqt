// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEST_NETWORK_H
#define BOBUIEST_NETWORK_H

#include <BobUITest/bobuiesttostring.h>

// enable NETWORK features
#ifndef BOBUI_NETWORK_LIB
#define BOBUI_NETWORK_LIB
#endif

#if 0
#pragma bobui_class(BobUITestNetwork)
#endif

#include <BobUINetwork/QHostAddress>
#include <BobUINetwork/QNetworkCookie>
#include <BobUINetwork/QNetworkReply>

#if 0
// inform syncbobui
#pragma bobui_no_master_include
#endif

BOBUI_BEGIN_NAMESPACE

namespace BOBUIest
{
template<>
inline char *toString<QHostAddress>(const QHostAddress &addr)
{
    switch (addr.protocol()) {
    case QAbstractSocket::UnknownNetworkLayerProtocol:
        return qstrdup("<unknown address (parse error)>");
    case QAbstractSocket::AnyIPProtocol:
        return qstrdup("QHostAddress::Any");
    case QAbstractSocket::IPv4Protocol:
    case QAbstractSocket::IPv6Protocol:
        break;
    }

    return toString(addr.toString());
}
} // namespace BOBUIest

inline char *toString(QNetworkReply::NetworkError code)
{
    const QMetaObject *mo = &QNetworkReply::staticMetaObject;
    int index = mo->indexOfEnumerator("NetworkError");
    if (index == -1)
        return qstrdup("");

    QMetaEnum qme = mo->enumerator(index);
    return qstrdup(qme.valueToKey(code));
}

inline char *toString(const QNetworkCookie &cookie)
{
    return BOBUIest::toString(cookie.toRawForm());
}

inline char *toString(const QList<QNetworkCookie> &list)
{
    QByteArray result = "QList(";
    if (!list.isEmpty()) {
        for (const QNetworkCookie &cookie : list)
            result += "QNetworkCookie(" + cookie.toRawForm() + "), ";
        result.chop(2); // remove trailing ", "
    }
    result.append(')');
    return BOBUIest::toString(result);
}

BOBUI_END_NAMESPACE

#endif
