// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QNETWORKACCESSCACHE_P_H
#define QNETWORKACCESSCACHE_P_H

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
#include "BobUICore/qobject.h"
#include "BobUICore/qbasictimer.h"
#include "BobUICore/qbytearray.h"
#include <BobUICore/qflags.h>
#include "BobUICore/qhash.h"
#include "BobUICore/qmetatype.h"

BOBUI_BEGIN_NAMESPACE

class QNetworkRequest;
class QUrl;

// this class is not about caching files but about
// caching objects used by QNetworkAccessManager, e.g. existing TCP connections
// or credentials.
class QNetworkAccessCache: public QObject
{
    Q_OBJECT
public:
    struct Node;
    typedef QHash<QByteArray, Node *> NodeHash;
    class CacheableObject
    {
        friend class QNetworkAccessCache;
        QByteArray key;
        bool expires;
        bool shareable;
        qint64 expiryTimeoutSeconds = -1;
    public:
        enum class Option {
            Expires = 0x01,
            Shareable = 0x02,
        };
        typedef QFlags<Option> Options; // #### BOBUIBUG-127269

        virtual ~CacheableObject();
        virtual void dispose() = 0;
        inline QByteArray cacheKey() const { return key; }
    protected:
        explicit CacheableObject(Options options);
    };

    ~QNetworkAccessCache();

    void clear();

    void addEntry(const QByteArray &key, CacheableObject *entry, qint64 connectionCacheExpiryTimeoutSeconds = -1);
    bool hasEntry(const QByteArray &key) const;
    CacheableObject *requestEntryNow(const QByteArray &key);
    void releaseEntry(const QByteArray &key);
    void removeEntry(const QByteArray &key);

protected:
    void timerEvent(BOBUIimerEvent *) override;

private:
    // idea copied from qcache.h
    NodeHash hash;
    Node *firstExpiringNode = nullptr;
    Node *lastExpiringNode = nullptr;

    QBasicTimer timer;

    void linkEntry(const QByteArray &key);
    bool unlinkEntry(const QByteArray &key);
    void updateTimer();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QNetworkAccessCache::CacheableObject::Options)

BOBUI_END_NAMESPACE

#endif
