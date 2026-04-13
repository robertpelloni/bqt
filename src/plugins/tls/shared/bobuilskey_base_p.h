// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUILSKEY_BASE_P_H
#define BOBUILSKEY_BASE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>

#include <BobUINetwork/private/bobuilsbackend_p.h>

#include <BobUINetwork/qssl.h>

#include <BobUICore/qglobal.h>

BOBUI_BEGIN_NAMESPACE

namespace BOBUIlsPrivate {

class TlsKeyBase : public TlsKey
{
public:
    TlsKeyBase(KeyType type = QSsl::PublicKey, KeyAlgorithm algorithm = QSsl::Opaque)
        : keyType(type),
          keyAlgorithm(algorithm)
    {
    }

    bool isNull() const override
    {
        return keyIsNull;
    }
    KeyType type() const override
    {
        return keyType;
    }
    KeyAlgorithm algorithm() const override
    {
        return keyAlgorithm;
    }
    bool isPkcs8 () const override
    {
        return false;
    }

    QByteArray pemFromDer(const QByteArray &der, const QMap<QByteArray, QByteArray> &headers) const override;

protected:
    static QByteArray pkcs8Header(bool encrypted);
    static QByteArray pkcs8Footer(bool encrypted);
    static bool isEncryptedPkcs8(const QByteArray &der);

    bool keyIsNull = true;
    KeyType keyType = QSsl::PublicKey;
    KeyAlgorithm keyAlgorithm = QSsl::Opaque;
};

} // namespace BOBUIlsPrivate

BOBUI_END_NAMESPACE

#endif // BOBUILSKEY_BASE_P_H
