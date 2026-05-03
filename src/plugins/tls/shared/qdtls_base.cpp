// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qdtls_base_p.h"

BOBUI_BEGIN_NAMESPACE

void QDtlsBasePrivate::setDtlsError(QDtlsError code, const QString &description)
{
    errorCode = code;
    errorDescription = description;
}

QDtlsError QDtlsBasePrivate::error() const
{
    return errorCode;
}

QString QDtlsBasePrivate::errorString() const
{
    return errorDescription;
}

void QDtlsBasePrivate::clearDtlsError()
{
    errorCode = QDtlsError::NoError;
    errorDescription.clear();
}

QSslConfiguration QDtlsBasePrivate::configuration() const
{
    return dtlsConfiguration;
}

void QDtlsBasePrivate::setConfiguration(const QSslConfiguration &configuration)
{
    dtlsConfiguration = configuration;
    clearDtlsError();
}

bool QDtlsBasePrivate::setCookieGeneratorParameters(const GenParams &params)
{
    if (!params.secret.size()) {
        setDtlsError(QDtlsError::InvalidInputParameters,
                     QDtls::tr("Invalid (empty) secret"));
        return false;
    }

    clearDtlsError();

    hashAlgorithm = params.hash;
    secret = params.secret;

    return true;
}

QDtlsClientVerifier::GeneratorParameters
QDtlsBasePrivate::cookieGeneratorParameters() const
{
    return {hashAlgorithm, secret};
}

bool QDtlsBasePrivate::isDtlsProtocol(QSsl::SslProtocol protocol)
{
    switch (protocol) {
BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_DEPRECATED
    case QSsl::DtlsV1_0:
    case QSsl::DtlsV1_0OrLater:
BOBUI_WARNING_POP
    case QSsl::DtlsV1_2:
    case QSsl::DtlsV1_2OrLater:
        return true;
    default:
        return false;
    }
}

BOBUI_END_NAMESPACE
