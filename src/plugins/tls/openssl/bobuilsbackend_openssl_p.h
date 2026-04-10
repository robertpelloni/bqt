// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUILSBACKEND_OPENSSL_P_H
#define BOBUILSBACKEND_OPENSSL_P_H

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

#include <BobUINetwork/qssldiffiehellmanparameters.h>
#include <BobUINetwork/qsslcertificate.h>

#include <BobUINetwork/private/bobuilsbackend_p.h>

#include <BobUICore/qglobal.h>
#include <BobUICore/qlist.h>

#include <openssl/ssl.h>

BOBUI_BEGIN_NAMESPACE

class BOBUIlsBackendOpenSSL final : public BOBUIlsBackend
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID BOBUIlsBackend_iid)
    Q_INTERFACES(BOBUIlsBackend)

public:

    static QString getErrorsFromOpenSsl();
    static void logAndClearErrorQueue();
    static void clearErrorQueue();

    // Index used in SSL_get_ex_data to get the matching TlsCryptographerOpenSSL:
    static int s_indexForSSLExtraData;

    static QString msgErrorsDuringHandshake();
    static QSslCipher bobui_OpenSSL_cipher_to_QSslCipher(const SSL_CIPHER *cipher);
private:
    static bool ensureLibraryLoaded();
    QString backendName() const override;
    bool isValid() const override;
    long tlsLibraryVersionNumber() const override;
    QString tlsLibraryVersionString() const override;
    long tlsLibraryBuildVersionNumber() const override;
    QString tlsLibraryBuildVersionString() const override;

    void ensureInitialized() const override;
    void ensureCiphersAndCertsLoaded() const;
    static void resetDefaultCiphers();

    QList<QSsl::SslProtocol> supportedProtocols() const override;
    QList<QSsl::SupportedFeature> supportedFeatures() const override;
    QList<QSsl::ImplementedClass> implementedClasses() const override;

    // QSslKey:
    BOBUIlsPrivate::TlsKey *createKey() const override;

    // QSslCertificate:
    BOBUIlsPrivate::X509Certificate *createCertificate() const override;
    QList<QSslCertificate> systemCaCertificates() const override;

    BOBUIlsPrivate::TlsCryptograph *createTlsCryptograph() const override;
    BOBUIlsPrivate::DtlsCookieVerifier *createDtlsCookieVerifier() const override;
    BOBUIlsPrivate::DtlsCryptograph *createDtlsCryptograph(QDtls *q, int mode) const override;

    BOBUIlsPrivate::X509ChainVerifyPtr X509Verifier() const override;
    BOBUIlsPrivate::X509PemReaderPtr X509PemReader() const override;
    BOBUIlsPrivate::X509DerReaderPtr X509DerReader() const override;
    BOBUIlsPrivate::X509Pkcs12ReaderPtr X509Pkcs12Reader() const override;

    // Elliptic curves:
    QList<int> ellipticCurvesIds() const override;
    int curveIdFromShortName(const QString &name) const override;
    int curveIdFromLongName(const QString &name) const override;
    QString shortNameForId(int cid) const override;
    QString longNameForId(int cid) const override;
    bool isTlsNamedCurve(int cid) const override;

    // DH parameters:
    using DHParams = QSslDiffieHellmanParameters;
    int dhParametersFromDer(const QByteArray &derData, QByteArray *data) const override;
    int dhParametersFromPem(const QByteArray &pemData, QByteArray *data) const override;

    void forceAutotestSecurityLevel() override;
};

Q_DECLARE_LOGGING_CATEGORY(lcTlsBackend)

BOBUI_END_NAMESPACE

#endif // BOBUILSBACKEND_OPENSSL_P_H


