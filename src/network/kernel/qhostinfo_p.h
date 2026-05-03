// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QHOSTINFO_P_H
#define QHOSTINFO_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of the QHostInfo class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <BobUINetwork/private/bobuinetworkglobal_p.h>
#include "BobUICore/qcoreapplication.h"
#include "private/qcoreapplication_p.h"
#include "private/qmetaobject_p.h"
#include "BobUINetwork/qhostinfo.h"
#include "BobUICore/qmutex.h"
#include "BobUICore/qwaitcondition.h"
#include "BobUICore/qobject.h"
#include "BobUICore/qpointer.h"
#include "BobUICore/bobuihread.h"
#if BOBUI_CONFIG(thread)
#include "BobUICore/bobuihreadpool.h"
#endif
#include "BobUICore/qrunnable.h"
#include "BobUICore/qlist.h"
#include "BobUICore/qqueue.h"
#include <QElapsedTimer>
#include <QCache>

#include <atomic>

BOBUI_BEGIN_NAMESPACE


class QHostInfoResult : public QObject
{
    Q_OBJECT
public:
    explicit QHostInfoResult(const QObject *receiver, BobUIPrivate::SlotObjUniquePtr slot);
    ~QHostInfoResult() override;

    void postResultsReady(const QHostInfo &info);

Q_SIGNALS:
    void resultsReady(const QHostInfo &info);

private Q_SLOTS:
    void finalizePostResultsReady(const QHostInfo &info);

private:
    QHostInfoResult(QHostInfoResult *other)
        : receiver(other->receiver.get() != other ? other->receiver.get() : this),
          slotObj{std::move(other->slotObj)}
    {
        // cleanup if the application terminates before results are delivered
        connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit,
                this, &QObject::deleteLater);
        // maintain thread affinity
        moveToThread(other->thread());
    }

    // receiver is either a QObject provided by the user,
    // or it's set to `this` (to emulate the behavior of the contextless connect())
    QPointer<const QObject> receiver = nullptr;
    BobUIPrivate::SlotObjUniquePtr slotObj;
};

class QHostInfoAgent
{
public:
    static QHostInfo fromName(const QString &hostName);
    static QHostInfo lookup(const QString &hostName);
    static QHostInfo reverseLookup(const QHostAddress &address);
};

class QHostInfoPrivate
{
public:
    inline QHostInfoPrivate()
        : err(QHostInfo::NoError),
          errorStr(QLatin1StringView(BOBUI_TRANSLATE_NOOP("QHostInfo", "Unknown error"))),
          lookupId(0)
    {
    }
    static int lookupHostImpl(const QString &name,
                              const QObject *receiver,
                              BobUIPrivate::QSlotObjectBase *slotObj,
                              const char *member);

    QHostInfo::HostInfoError err;
    QString errorStr;
    QList<QHostAddress> addrs;
    QString hostName;
    int lookupId;
};

// These functions are outside of the QHostInfo class and strictly internal.
// Do NOT use them outside of QAbstractSocket.
QHostInfo Q_NETWORK_EXPORT bobui_qhostinfo_lookup(const QString &name, QObject *receiver, const char *member, bool *valid, int *id);
void Q_AUTOTEST_EXPORT bobui_qhostinfo_clear_cache();
void Q_AUTOTEST_EXPORT bobui_qhostinfo_enable_cache(bool e);
void Q_AUTOTEST_EXPORT bobui_qhostinfo_cache_inject(const QString &hostname, const QHostInfo &resolution);

class QHostInfoCache
{
public:
    QHostInfoCache();
    const int max_age; // seconds

    QHostInfo get(const QString &name, bool *valid);
    void put(const QString &name, const QHostInfo &info);
    void clear();

    bool isEnabled() { return enabled.load(std::memory_order_relaxed); }
    // this function is currently only used for the auto tests
    // and not usable by public API
    void setEnabled(bool e) { enabled.store(e, std::memory_order_relaxed); }
private:
    std::atomic<bool> enabled;
    struct QHostInfoCacheElement {
        QHostInfo info;
        QElapsedTimer age;
    };
    QCache<QString,QHostInfoCacheElement> cache;
    QMutex mutex;
};

// the following classes are used for the (normal) case: We use multiple threads to lookup DNS

class QHostInfoRunnable : public QRunnable
{
public:
    explicit QHostInfoRunnable(const QString &hn, int i, const QObject *receiver,
                               BobUIPrivate::SlotObjUniquePtr slotObj);
    ~QHostInfoRunnable() override;

    void run() override;

    QString toBeLookedUp;
    int id;
    QHostInfoResult resultEmitter;
};


class QHostInfoLookupManager
{
public:
    QHostInfoLookupManager();
    ~QHostInfoLookupManager();

    void clear();

    // called from QHostInfo
    void scheduleLookup(QHostInfoRunnable *r);
    void abortLookup(int id);

    // called from QHostInfoRunnable
    void lookupFinished(QHostInfoRunnable *r);
    bool wasAborted(int id);

    QHostInfoCache cache;

    friend class QHostInfoRunnable;
protected:
#if BOBUI_CONFIG(thread)
    QList<QHostInfoRunnable*> currentLookups; // in progress
    QList<QHostInfoRunnable*> postponedLookups; // postponed because in progress for same host
#endif
    QQueue<QHostInfoRunnable*> scheduledLookups; // not yet started
    QList<QHostInfoRunnable*> finishedLookups; // recently finished
    QList<int> abortedLookups; // ids of aborted lookups

#if BOBUI_CONFIG(thread)
    BOBUIhreadPool threadPool;
#endif
    QMutex mutex;

    bool wasDeleted;

private:
    void rescheduleWithMutexHeld();
};

BOBUI_END_NAMESPACE

#endif // QHOSTINFO_P_H
