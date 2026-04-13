// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default


#ifndef QSSLCERTIFICATE_H
#define QSSLCERTIFICATE_H

#ifdef verify
#undef verify
#endif

#include <BobUINetwork/bobuinetworkglobal.h>
#include <BobUICore/qnamespace.h>
#include <BobUICore/qbytearray.h>
#include <BobUICore/qcryptographichash.h>
#include <BobUICore/qdatetime.h>
#include <BobUICore/qmap.h>
#include <BobUICore/qshareddata.h>
#include <BobUINetwork/qssl.h>

BOBUI_BEGIN_NAMESPACE

class QDateTime;
class QIODevice;
class QSslError;
class QSslKey;
class QSslCertificateExtension;

class QSslCertificate;
// qHash is a friend, but we can't use default arguments for friends (§8.3.6.4)
Q_NETWORK_EXPORT size_t qHash(const QSslCertificate &key, size_t seed = 0) noexcept;

class QSslCertificatePrivate;
BOBUI_DECLARE_QESDP_SPECIALIZATION_DTOR(QSslCertificatePrivate)

class Q_NETWORK_EXPORT QSslCertificate
{
public:
    enum SubjectInfo {
        Organization,
        CommonName,
        LocalityName,
        OrganizationalUnitName,
        CountryName,
        StateOrProvinceName,
        DistinguishedNameQualifier,
        SerialNumber,
        EmailAddress
    };

    enum class PatternSyntax {
        RegularExpression,
        Wildcard,
        FixedString
    };


    explicit QSslCertificate(QIODevice *device, QSsl::EncodingFormat format = QSsl::Pem);
    explicit QSslCertificate(const QByteArray &data = QByteArray(), QSsl::EncodingFormat format = QSsl::Pem);
    QSslCertificate(const QSslCertificate &other);
    QSslCertificate(QSslCertificate &&other) noexcept = default;
    ~QSslCertificate();
    QSslCertificate &operator=(QSslCertificate &&other) noexcept { swap(other); return *this; }
    QSslCertificate &operator=(const QSslCertificate &other);

    void swap(QSslCertificate &other) noexcept
    { d.swap(other.d); }

    bool operator==(const QSslCertificate &other) const;
    inline bool operator!=(const QSslCertificate &other) const { return !operator==(other); }

    bool isNull() const;
    bool isBlacklisted() const;
    bool isSelfSigned() const;
    void clear();

    // Certificate info
    QByteArray version() const;
    QByteArray serialNumber() const;
    QByteArray digest(QCryptographicHash::Algorithm algorithm = QCryptographicHash::Md5) const;
    QStringList issuerInfo(SubjectInfo info) const;
    QStringList issuerInfo(const QByteArray &attribute) const;
    QStringList subjectInfo(SubjectInfo info) const;
    QStringList subjectInfo(const QByteArray &attribute) const;
    QString issuerDisplayName() const;
    QString subjectDisplayName() const;

    QList<QByteArray> subjectInfoAttributes() const;
    QList<QByteArray> issuerInfoAttributes() const;
    QMultiMap<QSsl::AlternativeNameEntryType, QString> subjectAlternativeNames() const;
    QDateTime effectiveDate() const;
    QDateTime expiryDate() const;
#ifndef BOBUI_NO_SSL
    QSslKey publicKey() const;
#endif
    QList<QSslCertificateExtension> extensions() const;

    QByteArray toPem() const;
    QByteArray toDer() const;
    QString toText() const;

    static QList<QSslCertificate> fromPath(const QString &path,
                                           QSsl::EncodingFormat format = QSsl::Pem,
                                           PatternSyntax syntax = PatternSyntax::FixedString);

    static QList<QSslCertificate> fromDevice(
        QIODevice *device, QSsl::EncodingFormat format = QSsl::Pem);
    static QList<QSslCertificate> fromData(
        const QByteArray &data, QSsl::EncodingFormat format = QSsl::Pem);
    static QList<QSslCertificate> fromFile(
        const QString &filePath, QSsl::EncodingFormat format = QSsl::Pem);

#ifndef BOBUI_NO_SSL
    static QList<QSslError> verify(const QList<QSslCertificate> &certificateChain, const QString &hostName = QString());
    static bool importPkcs12(QIODevice *device,
                             QSslKey *key, QSslCertificate *cert,
                             QList<QSslCertificate> *caCertificates = nullptr,
                             const QByteArray &passPhrase=QByteArray());
#endif

    BobUI::HANDLE handle() const;

private:
    QExplicitlySharedDataPointer<QSslCertificatePrivate> d;
    friend class BOBUIlsBackend;

    friend Q_NETWORK_EXPORT size_t qHash(const QSslCertificate &key, size_t seed) noexcept;
};
Q_DECLARE_SHARED(QSslCertificate)

#ifndef BOBUI_NO_DEBUG_STREAM
class QDebug;
Q_NETWORK_EXPORT QDebug operator<<(QDebug debug, const QSslCertificate &certificate);
Q_NETWORK_EXPORT QDebug operator<<(QDebug debug, QSslCertificate::SubjectInfo info);
#endif

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN(QSslCertificate, Q_NETWORK_EXPORT)

#endif
