// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUILSKEY_GENERIC_P_H
#define BOBUILSKEY_GENERIC_P_H

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

#include "bobuilskey_base_p.h"


#include <BobUICore/qnamespace.h>
#include <BobUICore/qglobal.h>

BOBUI_BEGIN_NAMESPACE

namespace BOBUIlsPrivate {

// This class is what previously was known as qsslkey_bobui:
// it implements most of functionality needed by QSslKey
// not relying on any TLS implementation. It's used by
// our SecureTransport and Schannel backends.
class TlsKeyGeneric : public TlsKeyBase
{
public:
    using TlsKeyBase::TlsKeyBase;

    void decodeDer(KeyType type, KeyAlgorithm algorithm, const QByteArray &der,
                   const QByteArray &passPhrase, bool deepClear) override;
    void decodePem(KeyType type, KeyAlgorithm algorithm, const QByteArray &pem,
                   const QByteArray &passPhrase, bool deepClear) override;

    QByteArray toPem(const QByteArray &passPhrase) const override;

    QByteArray derFromPem(const QByteArray &pem, QMap<QByteArray,
                          QByteArray> *headers) const override;

    void fromHandle(BobUI::HANDLE opaque, KeyType expectedType) override;

    void clear(bool deep) override;

    BobUI::HANDLE handle() const override
    {
        return BobUI::HANDLE(opaque);
    }

    int length() const override
    {
        return keyLength;
    }

    bool isPkcs8() const override
    {
        return pkcs8;
    }

private:
    QByteArray decryptPkcs8(const QByteArray &encrypted, const QByteArray &passPhrase);

    bool pkcs8 = false;
    BobUI::HANDLE opaque = nullptr;
    QByteArray derData;
    int keyLength = -1;
};

} // namespace BOBUIlsPrivate

BOBUI_END_NAMESPACE

#endif // BOBUILSKEY_GENERIC_P_H
