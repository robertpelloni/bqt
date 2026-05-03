// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QX509_SCHANNEL_P_H
#define QX509_SCHANNEL_P_H

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

#include "../shared/qx509_generic_p.h"
#include "../shared/qwincrypt_p.h"

#include <BobUICore/qglobal.h>

BOBUI_BEGIN_NAMESPACE

namespace BOBUIlsPrivate {

class X509CertificateSchannel final : public X509CertificateGeneric
{
public:
    X509CertificateSchannel();
    ~X509CertificateSchannel();

    TlsKey *publicKey() const override;
    BobUI::HANDLE handle() const override;

    static QSslCertificate QSslCertificate_from_CERT_CONTEXT(const CERT_CONTEXT *certificateContext);

    static bool importPkcs12(QIODevice *device, QSslKey *key, QSslCertificate *cert,
                             QList<QSslCertificate> *caCertificates,
                             const QByteArray &passPhrase);

    QPCCertContextPointer certificateContext;

    Q_DISABLE_COPY_MOVE(X509CertificateSchannel);
};

} // namespace BOBUIlsPrivate

BOBUI_END_NAMESPACE

#endif // QX509_SCHANNEL_P_H
