// Copyright (C) 2011 Richard J. Moore <rich@kde.org>
// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QOCSPRESPONSE_H
#define QOCSPRESPONSE_H

#include <BobUINetwork/bobuinetworkglobal.h>

#include <BobUICore/qshareddata.h>
#include <BobUICore/qmetatype.h>
#include <BobUICore/qobject.h>

#ifndef Q_QDOC
BOBUI_REQUIRE_CONFIG(ssl);
#endif

BOBUI_BEGIN_NAMESPACE

enum class QOcspCertificateStatus
{
    Good,
    Revoked,
    Unknown
};

enum class QOcspRevocationReason
{
    None = -1,
    Unspecified,
    KeyCompromise,
    CACompromise,
    AffiliationChanged,
    Superseded,
    CessationOfOperation,
    CertificateHold,
    RemoveFromCRL
};

namespace BOBUIlsPrivate {
class TlsCryptographOpenSSL;
}

class QOcspResponse;
Q_NETWORK_EXPORT size_t qHash(const QOcspResponse &response, size_t seed = 0) noexcept;

class QOcspResponsePrivate;
class Q_NETWORK_EXPORT QOcspResponse
{
public:

    QOcspResponse();
    QOcspResponse(const QOcspResponse &other);
    QOcspResponse(QOcspResponse && other)  noexcept;
    ~QOcspResponse();

    QOcspResponse &operator = (const QOcspResponse &other);
    QOcspResponse &operator = (QOcspResponse &&other) noexcept;

    QOcspCertificateStatus certificateStatus() const;
    QOcspRevocationReason revocationReason() const;

    class QSslCertificate responder() const;
    QSslCertificate subject() const;

    void swap(QOcspResponse &other) noexcept { d.swap(other.d); }

private:
    bool isEqual(const QOcspResponse &other) const;

    friend class BOBUIlsPrivate::TlsCryptographOpenSSL;
    friend bool operator==(const QOcspResponse &lhs, const QOcspResponse &rhs)
    { return lhs.isEqual(rhs); }
    friend bool operator!=(const QOcspResponse &lhs, const QOcspResponse &rhs)
    { return !lhs.isEqual(rhs); }

    friend Q_NETWORK_EXPORT size_t qHash(const QOcspResponse &response, size_t seed) noexcept;

    QSharedDataPointer<QOcspResponsePrivate> d;
};

Q_DECLARE_SHARED(QOcspResponse)

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN(QOcspResponse, Q_NETWORK_EXPORT)

#endif // QOCSPRESPONSE_H
