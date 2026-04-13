// Copyright (C) 2014 Governikus GmbH & Co. KG.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSSLPRESHAREDKEYAUTHENTICATOR_P_H
#define QSSLPRESHAREDKEYAUTHENTICATOR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>
#include <QSharedData>

BOBUI_BEGIN_NAMESPACE

class QSslPreSharedKeyAuthenticatorPrivate : public QSharedData
{
public:
    QSslPreSharedKeyAuthenticatorPrivate();

    QByteArray identityHint;

    QByteArray identity;
    int maximumIdentityLength;

    QByteArray preSharedKey;
    int maximumPreSharedKeyLength;
};

BOBUI_END_NAMESPACE

#endif // QSSLPRESHAREDKEYAUTHENTICATOR_P_H
