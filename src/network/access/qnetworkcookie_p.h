// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QNETWORKCOOKIE_P_H
#define QNETWORKCOOKIE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the Network Access framework.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>
#include "BobUICore/qdatetime.h"
#include "BobUINetwork/qnetworkcookie.h"

BOBUI_BEGIN_NAMESPACE

class QNetworkCookiePrivate: public QSharedData
{
public:
    QNetworkCookiePrivate() = default;
    static QList<QNetworkCookie> parseSetCookieHeaderLine(QByteArrayView cookieString);

    QDateTime expirationDate;
    QString domain;
    QString path;
    QString comment;
    QByteArray name;
    QByteArray value;
    QNetworkCookie::SameSite sameSite = QNetworkCookie::SameSite::Default;
    bool secure = false;
    bool httpOnly = false;
};

namespace {
inline bool isLWS(char c)
{
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

// Used in qnetworkcookie.cpp and qnetworkreplyhttpimpl.cpp
inline int nextNonWhitespace(QByteArrayView text, int from)
{
    // RFC 2616 defines linear whitespace as:
    //  LWS = [CRLF] 1*( SP | HT )
    // We ignore the fact that CRLF must come as a pair at this point
    // It's an invalid HTTP header if that happens.
    while (from < text.size()) {
        if (isLWS(text.at(from)))
            ++from;
        else
            return from;        // non-whitespace
    }

    // reached the end
    return text.size();
}
}

BOBUI_END_NAMESPACE

#endif
