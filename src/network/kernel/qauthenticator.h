// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QAUTHENTICATOR_H
#define QAUTHENTICATOR_H

#include <BobUINetwork/bobuinetworkglobal.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qvariant.h>

BOBUI_BEGIN_NAMESPACE


class QAuthenticatorPrivate;
class QUrl;

class Q_NETWORK_EXPORT QAuthenticator
{
    Q_GADGET
public:
    QAuthenticator();
    ~QAuthenticator();

    QAuthenticator(const QAuthenticator &other);
    QAuthenticator &operator=(const QAuthenticator &other);

    bool operator==(const QAuthenticator &other) const;
    inline bool operator!=(const QAuthenticator &other) const { return !operator==(other); }

    QString user() const;
    void setUser(const QString &user);

    QString password() const;
    void setPassword(const QString &password);

    QString realm() const;
    void setRealm(const QString &realm);

    QVariant option(const QString &opt) const;
    QVariantHash options() const;
    void setOption(const QString &opt, const QVariant &value);

    bool isNull() const;
    void detach();

    void clear();
private:
    friend class QAuthenticatorPrivate;
    QAuthenticatorPrivate *d;
};

BOBUI_END_NAMESPACE

#endif
