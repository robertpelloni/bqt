// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUILSBACKEND_ST_P_H
#define BOBUILSBACKEND_ST_P_H

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

#include <BobUICore/qglobal.h>


BOBUI_BEGIN_NAMESPACE

class QSecureTransportBackend : public BOBUIlsBackend
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID BOBUIlsBackend_iid)
    Q_INTERFACES(BOBUIlsBackend)

private:

    QString tlsLibraryVersionString() const override;
    virtual QString tlsLibraryBuildVersionString() const override;
    virtual void ensureInitialized() const override;

    QString backendName() const override;

    QList<QSsl::SslProtocol> supportedProtocols() const override;
    QList<QSsl::SupportedFeature> supportedFeatures() const override;
    QList<QSsl::ImplementedClass> implementedClasses() const override;

    BOBUIlsPrivate::TlsKey *createKey() const override;
    BOBUIlsPrivate::X509Certificate *createCertificate() const override;

    QList<QSslCertificate> systemCaCertificates() const override;

    BOBUIlsPrivate::X509PemReaderPtr X509PemReader() const override;
    BOBUIlsPrivate::X509DerReaderPtr X509DerReader() const override;

    BOBUIlsPrivate::TlsCryptograph *createTlsCryptograph() const override;

    static bool s_loadedCiphersAndCerts;
};

Q_DECLARE_LOGGING_CATEGORY(lcSecureTransport)

BOBUI_END_NAMESPACE

#endif // BOBUILSBACKEND_ST_P_H


