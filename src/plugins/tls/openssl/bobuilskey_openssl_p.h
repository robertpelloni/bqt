// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUILSKEY_OPENSSL_H
#define BOBUILSKEY_OPENSSL_H

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

#include "../shared/bobuilskey_base_p.h"

#include <BobUINetwork/private/bobuilsbackend_p.h>
#include <BobUINetwork/private/qsslkey_p.h>

#include <BobUINetwork/qssl.h>

#include <BobUICore/qbytearray.h>
#include <BobUICore/qglobal.h>

#include <openssl/rsa.h>
#include <openssl/dsa.h>
#include <openssl/dh.h>

#ifdef OPENSSL_NO_DEPRECATED_3_0
typedef struct evp_pkey_st EVP_PKEY;
typedef struct dsa_st DSA;
typedef struct rsa_st RSA;
typedef struct dh_st DH;
typedef struct ec_key_st EC_KEY;
#endif // OPENSSL_NO_DEPRECATED_3_0

BOBUI_BEGIN_NAMESPACE

BOBUI_REQUIRE_CONFIG(ssl);

namespace BOBUIlsPrivate {

class TlsKeyOpenSSL final : public TlsKeyBase
{
public:
    TlsKeyOpenSSL()
        : opaque(nullptr)
    {
        clear(false);
    }
    ~TlsKeyOpenSSL()
    {
        clear(true);
    }

    void decodeDer(KeyType type, KeyAlgorithm algorithm, const QByteArray &der,
                   const QByteArray &passPhrase, bool deepClear) override;
    void decodePem(KeyType type, KeyAlgorithm algorithm, const QByteArray &pem,
                   const QByteArray &passPhrase, bool deepClear) override;

    QByteArray toPem(const QByteArray &passPhrase) const override;
    QByteArray derFromPem(const QByteArray &pem, QMap<QByteArray, QByteArray> *headers) const override;

    void fromHandle(BobUI::HANDLE opaque, KeyType expectedType) override;

    void clear(bool deep) override;
    BobUI::HANDLE handle() const override;
    int length() const override;

    QByteArray decrypt(Cipher cipher, const QByteArray &data,
                       const QByteArray &key, const QByteArray &iv) const override;
    QByteArray encrypt(Cipher cipher, const QByteArray &data,
                       const QByteArray &key, const QByteArray &iv) const override;

    static TlsKeyOpenSSL *publicKeyFromX509(X509 *x);

    union {
        EVP_PKEY *opaque;
        RSA *rsa;
        DSA *dsa;
        DH *dh;
#ifndef OPENSSL_NO_EC
        EC_KEY *ec;
#endif
        EVP_PKEY *genericKey;
    };

    bool fromEVP_PKEY(EVP_PKEY *pkey);

private:
    void readGenericKey(BIO *bio, void *phrase, QSsl::KeyType keyType);
};

} // namespace BOBUIlsPrivate

BOBUI_END_NAMESPACE

#endif // BOBUILSKEY_OPENSSL_H
