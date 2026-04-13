// Copyright (C) 2011 Richard J. Moore <rich@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSSLCERTIFICATEEXTENSION_H
#define QSSLCERTIFICATEEXTENSION_H

#include <BobUINetwork/bobuinetworkglobal.h>
#include <BobUICore/qnamespace.h>
#include <BobUICore/qshareddata.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qvariant.h>

BOBUI_BEGIN_NAMESPACE

class QSslCertificateExtensionPrivate;

class Q_NETWORK_EXPORT QSslCertificateExtension
{
public:
    QSslCertificateExtension();
    QSslCertificateExtension(const QSslCertificateExtension &other);
    QSslCertificateExtension &operator=(QSslCertificateExtension &&other) noexcept { swap(other); return *this; }
    QSslCertificateExtension &operator=(const QSslCertificateExtension &other);
    ~QSslCertificateExtension();

    void swap(QSslCertificateExtension &other) noexcept { d.swap(other.d); }

    QString oid() const;
    QString name() const;
    QVariant value() const;
    bool isCritical() const;

    bool isSupported() const;

private:
    friend class QSslCertificatePrivate;
    QSharedDataPointer<QSslCertificateExtensionPrivate> d;
};

Q_DECLARE_SHARED(QSslCertificateExtension)

BOBUI_END_NAMESPACE


#endif // QSSLCERTIFICATEEXTENSION_H


