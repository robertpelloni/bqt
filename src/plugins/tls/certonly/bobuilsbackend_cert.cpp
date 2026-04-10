// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobuilsbackend_cert_p.h"

#include "../shared/qx509_generic_p.h"

#include <qssl.h>

#include <qlist.h>

BOBUI_BEGIN_NAMESPACE

QString BOBUIlsBackendCertOnly::backendName() const
{
    return builtinBackendNames[nameIndexCertOnly];
}


QList<QSsl::SslProtocol> BOBUIlsBackendCertOnly::supportedProtocols() const
{
    return {};
}

QList<QSsl::SupportedFeature> BOBUIlsBackendCertOnly::supportedFeatures() const
{
    return {};
}

QList<QSsl::ImplementedClass> BOBUIlsBackendCertOnly::implementedClasses() const
{
    QList<QSsl::ImplementedClass> classes;
    classes << QSsl::ImplementedClass::Certificate;

    return classes;
}

BOBUIlsPrivate::X509Certificate *BOBUIlsBackendCertOnly::createCertificate() const
{
    return new BOBUIlsPrivate::X509CertificateGeneric;
}

BOBUIlsPrivate::X509PemReaderPtr BOBUIlsBackendCertOnly::X509PemReader() const
{
    return BOBUIlsPrivate::X509CertificateGeneric::certificatesFromPem;
}

BOBUIlsPrivate::X509DerReaderPtr BOBUIlsBackendCertOnly::X509DerReader() const
{
    return BOBUIlsPrivate::X509CertificateGeneric::certificatesFromDer;
}

BOBUI_END_NAMESPACE

#include "moc_bobuilsbackend_cert_p.cpp"

