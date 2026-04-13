// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QNETWORKCOOKIEJAR_P_H
#define QNETWORKCOOKIEJAR_P_H

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
#include "private/qobject_p.h"
#include "qnetworkcookie.h"

BOBUI_BEGIN_NAMESPACE

class QNetworkCookieJarPrivate: public QObjectPrivate
{
public:
    QList<QNetworkCookie> allCookies;

    Q_DECLARE_PUBLIC(QNetworkCookieJar)
};

BOBUI_END_NAMESPACE

#endif
