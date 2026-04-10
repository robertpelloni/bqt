// Copyright (C) 2014 Governikus GmbH & Co. KG.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSSLPRESHAREDKEYAUTHENTICATOR_H
#define QSSLPRESHAREDKEYAUTHENTICATOR_H

#include <BobUINetwork/bobuinetworkglobal.h>
#include <BobUICore/QString>
#include <BobUICore/QSharedDataPointer>
#include <BobUICore/QMetaType>

BOBUI_REQUIRE_CONFIG(ssl);

BOBUI_BEGIN_NAMESPACE

class QSslPreSharedKeyAuthenticatorPrivate;
class QSslPreSharedKeyAuthenticator
{
    Q_GADGET_EXPORT(Q_NETWORK_EXPORT)
public:
    Q_NETWORK_EXPORT QSslPreSharedKeyAuthenticator();
    Q_NETWORK_EXPORT ~QSslPreSharedKeyAuthenticator();
    Q_NETWORK_EXPORT QSslPreSharedKeyAuthenticator(const QSslPreSharedKeyAuthenticator &authenticator);
    Q_NETWORK_EXPORT QSslPreSharedKeyAuthenticator &operator=(const QSslPreSharedKeyAuthenticator &authenticator);

    QSslPreSharedKeyAuthenticator &operator=(QSslPreSharedKeyAuthenticator &&other) noexcept { swap(other); return *this; }

    void swap(QSslPreSharedKeyAuthenticator &other) noexcept { d.swap(other.d); }

    Q_NETWORK_EXPORT QByteArray identityHint() const;

    Q_NETWORK_EXPORT void setIdentity(const QByteArray &identity);
    Q_NETWORK_EXPORT QByteArray identity() const;
    Q_NETWORK_EXPORT int maximumIdentityLength() const;

    Q_NETWORK_EXPORT void setPreSharedKey(const QByteArray &preSharedKey);
    Q_NETWORK_EXPORT QByteArray preSharedKey() const;
    Q_NETWORK_EXPORT int maximumPreSharedKeyLength() const;

private:
    Q_NETWORK_EXPORT bool isEqual(const QSslPreSharedKeyAuthenticator &other) const;

    friend class BOBUIlsBackend;

    friend bool operator==(const QSslPreSharedKeyAuthenticator &lhs, const QSslPreSharedKeyAuthenticator &rhs)
    { return lhs.isEqual(rhs); }
    friend bool operator!=(const QSslPreSharedKeyAuthenticator &lhs, const QSslPreSharedKeyAuthenticator &rhs)
    { return !lhs.isEqual(rhs); }

    QSharedDataPointer<QSslPreSharedKeyAuthenticatorPrivate> d;
};


Q_DECLARE_SHARED(QSslPreSharedKeyAuthenticator)

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN(QSslPreSharedKeyAuthenticator, Q_NETWORK_EXPORT)
BOBUI_DECL_METATYPE_EXTERN_TAGGED(QSslPreSharedKeyAuthenticator*, QSslPreSharedKeyAuthenticator_ptr, Q_NETWORK_EXPORT)

#endif // QSSLPRESHAREDKEYAUTHENTICATOR_H
