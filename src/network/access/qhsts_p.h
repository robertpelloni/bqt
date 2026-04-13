// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:critical reason:data-parser

#ifndef QHSTS_P_H
#define QHSTS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the Network Access API.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>

#include <BobUINetwork/qhstspolicy.h>

#include <BobUICore/qbytearray.h>
#include <BobUICore/qdatetime.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qurl.h>
#include <BobUICore/qcontainerfwd.h>

#include <map>

BOBUI_BEGIN_NAMESPACE

class QHttpHeaders;

class Q_AUTOTEST_EXPORT QHstsCache
{
public:

    void updateFromHeaders(const QHttpHeaders &headers,
                           const QUrl &url);
    void updateFromPolicies(const QList<QHstsPolicy> &hosts);
    void updateKnownHost(const QUrl &url, const QDateTime &expires,
                         bool includeSubDomains);
    bool isKnownHost(const QUrl &url) const;
    void clear();

    QList<QHstsPolicy> policies() const;

#if BOBUI_CONFIG(settings)
    void setStore(class QHstsStore *store);
#endif // BOBUI_CONFIG(settings)

private:

    void updateKnownHost(const QString &hostName, const QDateTime &expires,
                         bool includeSubDomains);

    struct HostName
    {
        explicit HostName(const QString &n) : name(n) { }
        explicit HostName(QStringView r) : fragment(r) { }

        bool operator < (const HostName &rhs) const
        {
            if (fragment.size()) {
                if (rhs.fragment.size())
                    return fragment < rhs.fragment;
                return fragment < QStringView{rhs.name};
            }

            if (rhs.fragment.size())
                return QStringView{name} < rhs.fragment;
            return name < rhs.name;
        }

        // We use 'name' for a HostName object contained in our dictionary;
        // we use 'fragment' only during lookup, when chopping the complete host
        // name, removing subdomain names (such HostName object is 'transient', it
        // must not outlive the original QString object.
        QString name;
        QStringView fragment;
    };

    mutable std::map<HostName, QHstsPolicy> knownHosts;
#if BOBUI_CONFIG(settings)
    QHstsStore *hstsStore = nullptr;
#endif // BOBUI_CONFIG(settings)
};

class Q_AUTOTEST_EXPORT QHstsHeaderParser
{
public:

    bool parse(const QHttpHeaders &headers);

    QDateTime expirationDate() const { return expiry; }
    bool includeSubDomains() const { return subDomainsFound; }

private:

    bool parseSTSHeader();
    bool parseDirective();
    bool processDirective(const QByteArray &name, const QByteArray &value);
    bool nextToken();

    QByteArray header;
    QByteArray token;

    QDateTime expiry;
    int tokenPos = 0;
    bool maxAgeFound = false;
    qint64 maxAge = 0;
    bool subDomainsFound = false;
};

BOBUI_END_NAMESPACE

#endif
