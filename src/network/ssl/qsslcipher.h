// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default


#ifndef QSSLCIPHER_H
#define QSSLCIPHER_H

#include <BobUINetwork/bobuinetworkglobal.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qscopedpointer.h>
#include <BobUINetwork/qssl.h>

#include <memory>

BOBUI_BEGIN_NAMESPACE


#ifndef BOBUI_NO_SSL

class QSslCipherPrivate;
class Q_NETWORK_EXPORT QSslCipher
{
public:
    QSslCipher();
    explicit QSslCipher(const QString &name);
    QSslCipher(const QString &name, QSsl::SslProtocol protocol);
    QSslCipher(const QSslCipher &other);
    QSslCipher &operator=(QSslCipher &&other) noexcept { swap(other); return *this; }
    QSslCipher &operator=(const QSslCipher &other);
    ~QSslCipher();

    void swap(QSslCipher &other) noexcept
    { d.swap(other.d); }

    bool operator==(const QSslCipher &other) const;
    inline bool operator!=(const QSslCipher &other) const { return !operator==(other); }

    bool isNull() const;
    QString name() const;
    int supportedBits() const;
    int usedBits() const;

    QString keyExchangeMethod() const;
    QString authenticationMethod() const;
    QString encryptionMethod() const;
    QString protocolString() const;
    QSsl::SslProtocol protocol() const;

private:
    // ### BobUI 7: make implicitly shared
    std::unique_ptr<QSslCipherPrivate> d;
    friend class BOBUIlsBackend;
};

Q_DECLARE_SHARED(QSslCipher)

#ifndef BOBUI_NO_DEBUG_STREAM
class QDebug;
Q_NETWORK_EXPORT QDebug operator<<(QDebug debug, const QSslCipher &cipher);
#endif

#endif // BOBUI_NO_SSL

BOBUI_END_NAMESPACE

#endif

