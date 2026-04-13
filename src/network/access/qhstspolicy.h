// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QHSTSPOLICY_H
#define QHSTSPOLICY_H

#include <BobUINetwork/bobuinetworkglobal.h>

#include <BobUICore/qshareddata.h>
#include <BobUICore/qflags.h>
#include <BobUICore/qurl.h>

BOBUI_BEGIN_NAMESPACE

class QHstsPolicyPrivate;
class QDateTime;
class QString;
class Q_NETWORK_EXPORT QHstsPolicy
{
public:
    enum PolicyFlag
    {
        IncludeSubDomains = 1
    };
    Q_DECLARE_FLAGS(PolicyFlags, PolicyFlag)

    QHstsPolicy();
    QHstsPolicy(const QDateTime &expiry, PolicyFlags flags, const QString &host,
                QUrl::ParsingMode mode = QUrl::DecodedMode);
    QHstsPolicy(const QHstsPolicy &rhs);
    QHstsPolicy &operator=(const QHstsPolicy &rhs);
    QHstsPolicy &operator=(QHstsPolicy &&other) noexcept { swap(other); return *this; }
    ~QHstsPolicy();

    void swap(QHstsPolicy &other) noexcept { d.swap(other.d); }

    void setHost(const QString &host, QUrl::ParsingMode mode = QUrl::DecodedMode);
    QString host(QUrl::ComponentFormattingOptions options = QUrl::FullyDecoded) const;
    void setExpiry(const QDateTime &expiry);
    QDateTime expiry() const;
    void setIncludesSubDomains(bool include);
    bool includesSubDomains() const;

    bool isExpired() const;

private:
    QSharedDataPointer<QHstsPolicyPrivate> d;

    bool isEqual(const QHstsPolicy &other) const;
    friend bool operator==(const QHstsPolicy &lhs, const QHstsPolicy &rhs)
    { return lhs.isEqual(rhs); }
    friend bool operator!=(const QHstsPolicy &lhs, const QHstsPolicy &rhs)
    { return !lhs.isEqual(rhs); }
};

Q_DECLARE_SHARED(QHstsPolicy)
Q_DECLARE_OPERATORS_FOR_FLAGS(QHstsPolicy::PolicyFlags)



BOBUI_END_NAMESPACE

#endif // QHSTSPOLICY_H
