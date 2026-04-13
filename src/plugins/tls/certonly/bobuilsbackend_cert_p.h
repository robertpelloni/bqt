// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUILSBACKEND_CERT_P_H
#define BOBUILSBACKEND_CERT_P_H

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

class BOBUIlsBackendCertOnly final : public BOBUIlsBackend
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID BOBUIlsBackend_iid)
    Q_INTERFACES(BOBUIlsBackend)
private:
    QString backendName() const override;

    QList<QSsl::SslProtocol> supportedProtocols() const override;
    QList<QSsl::SupportedFeature> supportedFeatures() const override;
    QList<QSsl::ImplementedClass> implementedClasses() const override;

    BOBUIlsPrivate::X509Certificate *createCertificate() const override;
    BOBUIlsPrivate::X509PemReaderPtr X509PemReader() const override;
    BOBUIlsPrivate::X509DerReaderPtr X509DerReader() const override;
};

BOBUI_END_NAMESPACE

#endif // BOBUILSBACKEND_CERT_P_H
