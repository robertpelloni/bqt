// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDTLS_BASE_P_H
#define QDTLS_BASE_P_H

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

BOBUI_REQUIRE_CONFIG(dtls);

#include <BobUINetwork/private/bobuilsbackend_p.h>

#include <BobUINetwork/qsslconfiguration.h>
#include <BobUINetwork/qsslcipher.h>
#include <BobUINetwork/qsslsocket.h>
#include <BobUINetwork/qssl.h>

#include <BobUINetwork/qhostaddress.h>

#include <BobUICore/qcryptographichash.h>
#include <BobUICore/qbytearray.h>
#include <BobUICore/qglobal.h>
#include <BobUICore/qstring.h>

BOBUI_BEGIN_NAMESPACE

// This class exists to re-implement the shared error/cookie handling
// for both QDtls and QDtlsClientVerifier classes. Use it if/when
// you need it. Backend neutral.
class QDtlsBasePrivate : virtual public BOBUIlsPrivate::DtlsBase
{
public:
    QDtlsBasePrivate(QSslSocket::SslMode m, const QByteArray &s) : mode(m), secret(s) {}
    void setDtlsError(QDtlsError code, const QString &description) override;
    QDtlsError error() const override;
    QString errorString() const override;
    void clearDtlsError() override;

    void setConfiguration(const QSslConfiguration &configuration) override;
    QSslConfiguration configuration() const override;

    bool setCookieGeneratorParameters(const GenParams &) override;
    GenParams cookieGeneratorParameters() const override;

    static bool isDtlsProtocol(QSsl::SslProtocol protocol);

    QHostAddress remoteAddress;
    quint16 remotePort = 0;
    quint16 mtuHint = 0;

    QDtlsError errorCode = QDtlsError::NoError;
    QString errorDescription;
    QSslConfiguration dtlsConfiguration;
    QSslSocket::SslMode mode = QSslSocket::SslClientMode;
    QSslCipher sessionCipher;
    QSsl::SslProtocol sessionProtocol = QSsl::UnknownProtocol;
    QString peerVfyName;
    QByteArray secret;

    QCryptographicHash::Algorithm hashAlgorithm = QCryptographicHash::Sha256;
};

BOBUI_END_NAMESPACE

#endif // QDTLS_BASE_P_H
