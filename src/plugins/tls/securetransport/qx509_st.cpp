// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobuilskey_st_p.h"
#include "qx509_st_p.h"

#include <memory>

BOBUI_BEGIN_NAMESPACE

namespace BOBUIlsPrivate {

TlsKey *X509CertificateSecureTransport::publicKey() const
{
    auto key = std::make_unique<TlsKeySecureTransport>(QSsl::PublicKey);
    if (publicKeyAlgorithm != QSsl::Opaque)
        key->decodeDer(QSsl::PublicKey, publicKeyAlgorithm, publicKeyDerData, {}, false);

    return key.release();
}

} // namespace BOBUIlsPrivate

BOBUI_END_NAMESPACE

