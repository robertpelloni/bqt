// Copyright (C) 2014 Jeremy Lainé <jeremy.laine@m4x.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUILS_ST_P_H
#define BOBUILS_ST_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the BobUINetwork library.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>

#include "bobuilsbackend_st_p.h"

#include <BobUICore/qobject.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qglobal.h>
#include <BobUICore/qlist.h>

#include <BobUINetwork/qabstractsocket.h>
#include <BobUINetwork/private/qsslsocket_p.h>

// SecureTransport was deprecated in macOS 10.15 and iOS 13, and is no
// longer supported. We should be using Network.framework instead.
// See BOBUIBUG-119171 for more information.
BOBUI_WARNING_DISABLE_DEPRECATED

#include <Security/Security.h>
#include <Security/SecureTransport.h>

BOBUI_BEGIN_NAMESPACE

namespace BOBUIlsPrivate {

class QSecureTransportContext
{
public:
    explicit QSecureTransportContext(SSLContextRef context);
    ~QSecureTransportContext();

    operator SSLContextRef () const;
    void reset(SSLContextRef newContext);
private:
    SSLContextRef context;

    Q_DISABLE_COPY_MOVE(QSecureTransportContext)
};

class TlsCryptographSecureTransport : public TlsCryptograph
{
public:
    TlsCryptographSecureTransport();
    ~TlsCryptographSecureTransport() override;

    void init(QSslSocket *qObj, QSslSocketPrivate *dObj) override;
    void continueHandshake() override;
    void disconnected() override;
    void disconnectFromHost() override;
    QSslCipher sessionCipher() const override;
    QSsl::SslProtocol sessionProtocol() const override;
    void startClientEncryption() override;
    void startServerEncryption() override;
    void transmit() override;
    QList<QSslError> tlsErrors() const override;

    SSLCipherSuite SSLCipherSuite_from_QSslCipher(const QSslCipher &ciph);

private:
    // SSL context management/properties:
    bool initSslContext();
    void destroySslContext();
    bool setSessionCertificate(QString &errorDescription,
                               QAbstractSocket::SocketError &errorCode);
    bool setSessionProtocol();
    // Aux. functions to do a verification during handshake phase:
    bool canIgnoreTrustVerificationFailure() const;
    bool verifySessionProtocol() const;
    bool verifyPeerTrust();

    bool checkSslErrors();
    bool startHandshake();

    bool isHandshakeComplete() const;

    // IO callbacks:
    static OSStatus ReadCallback(TlsCryptographSecureTransport *socket, char *data, size_t *dataLength);
    static OSStatus WriteCallback(TlsCryptographSecureTransport *plainSocket, const char *data, size_t *dataLength);

    QSecureTransportContext context;
    bool renegotiating = false;
    QSslSocket *q = nullptr;
    QSslSocketPrivate *d = nullptr;
    bool shutdown = false;
    QList<QSslError> sslErrors;

    Q_DISABLE_COPY_MOVE(TlsCryptographSecureTransport)
};

} // namespace BOBUIlsPrivate

BOBUI_END_NAMESPACE

#endif // BOBUILS_ST_P_H
