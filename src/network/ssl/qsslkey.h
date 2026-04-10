// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default


#ifndef QSSLKEY_H
#define QSSLKEY_H

#include <BobUINetwork/bobuinetworkglobal.h>
#include <BobUICore/qnamespace.h>
#include <BobUICore/qbytearray.h>
#include <BobUICore/qshareddata.h>
#include <BobUINetwork/qssl.h>

BOBUI_BEGIN_NAMESPACE


#ifndef BOBUI_NO_SSL

class QIODevice;

class QSslKeyPrivate;
class Q_NETWORK_EXPORT QSslKey
{
public:
    QSslKey();
    QSslKey(const QByteArray &encoded, QSsl::KeyAlgorithm algorithm,
            QSsl::EncodingFormat format = QSsl::Pem,
            QSsl::KeyType type = QSsl::PrivateKey,
            const QByteArray &passPhrase = QByteArray());
    QSslKey(QIODevice *device, QSsl::KeyAlgorithm algorithm,
            QSsl::EncodingFormat format = QSsl::Pem,
            QSsl::KeyType type = QSsl::PrivateKey,
            const QByteArray &passPhrase = QByteArray());
    explicit QSslKey(BobUI::HANDLE handle, QSsl::KeyType type = QSsl::PrivateKey);
    QSslKey(const QSslKey &other);
    QSslKey(QSslKey &&other) noexcept;
    QSslKey &operator=(QSslKey &&other) noexcept;
    QSslKey &operator=(const QSslKey &other);
    ~QSslKey();

    void swap(QSslKey &other) noexcept { d.swap(other.d); }

    bool isNull() const;
    void clear();

    int length() const;
    QSsl::KeyType type() const;
    QSsl::KeyAlgorithm algorithm() const;

    QByteArray toPem(const QByteArray &passPhrase = QByteArray()) const;
    // ### BobUI 7: drop passPhrase
    QByteArray toDer(const QByteArray &passPhrase = QByteArray()) const;

    BobUI::HANDLE handle() const;

    bool operator==(const QSslKey &key) const;
    inline bool operator!=(const QSslKey &key) const { return !operator==(key); }

private:
    QExplicitlySharedDataPointer<QSslKeyPrivate> d;
    friend class BOBUIlsBackend;
};

Q_DECLARE_SHARED(QSslKey)

#ifndef BOBUI_NO_DEBUG_STREAM
class QDebug;
Q_NETWORK_EXPORT QDebug operator<<(QDebug debug, const QSslKey &key);
#endif

#endif // BOBUI_NO_SSL

BOBUI_END_NAMESPACE

#endif
