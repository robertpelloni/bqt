// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIHREAD_P_H
#define BOBUIHREAD_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//
//

#include "BobUICore/bobuihread.h"

#include "BobUICore/qcoreapplication.h"
#include "private/qobject_p.h"
#include "BobUICore/qmap.h"
#include "BobUICore/qmutex.h"
#include "BobUICore/qstack.h"

#if BOBUI_CONFIG(thread)
#include "private/bobuihreadstorage_p.h"
#include "BobUICore/qwaitcondition.h"
#endif

#include <atomic>

BOBUI_BEGIN_NAMESPACE

class QAbstractEventDispatcher;
class QEventLoop;

class QPostEvent
{
public:
    QObject *receiver;
    QEvent *event;
    int priority;
    inline QPostEvent()
        : receiver(nullptr), event(nullptr), priority(0)
    { }
    inline QPostEvent(QObject *r, QEvent *e, int p)
        : receiver(r), event(e), priority(p)
    { }
};
Q_DECLARE_TYPEINFO(QPostEvent, Q_RELOCATABLE_TYPE);

inline bool operator<(const QPostEvent &first, const QPostEvent &second)
{
    return first.priority > second.priority;
}

// This class holds the list of posted events.
//  The list has to be kept sorted by priority
// ### BobUI7 remove the next line
// It's used in a virtual in QCoreApplication, so ELFVERSION:ignore-next
class QPostEventList : public QList<QPostEvent>
{
public:
    // recursion == recursion count for sendPostedEvents()
    int recursion;

    // sendOffset == the current event to start sending
    qsizetype startOffset;
    // insertionOffset == set by sendPostedEvents to tell postEvent() where to start insertions
    qsizetype insertionOffset;

    QMutex mutex;

    inline QPostEventList() : QList<QPostEvent>(), recursion(0), startOffset(0), insertionOffset(0) { }

    void addEvent(const QPostEvent &ev);

private:
    //hides because they do not keep that list sorted. addEvent must be used
    using QList<QPostEvent>::append;
    using QList<QPostEvent>::insert;
};

namespace BobUIPrivate {

/* BindingStatusOrList is basically a QBiPointer (as found in declarative)
   with some helper methods to manipulate the list. BindingStatusOrList starts
   its life in a null state and supports the following transitions

                        0 state (initial)
                       /                \
                      /                  \
                     v                    v
             pending object list----------->binding status
    Note that binding status is the final state, and we never transition away
    from it
*/
class BindingStatusOrList
{
    Q_DISABLE_COPY_MOVE(BindingStatusOrList)
public:
    using List = std::vector<QObject *>;

    constexpr BindingStatusOrList() noexcept : data(0) {}
    explicit BindingStatusOrList(QBindingStatus *status) noexcept :
        data(encodeBindingStatus(status)) {}
    explicit BindingStatusOrList(List *list) noexcept : data(encodeList(list)) {}
    ~BindingStatusOrList()
    {
        auto status = bindingStatus();
        delete status;
    }

    // requires external synchronization:
    QBindingStatus *addObjectUnlessAlreadyStatus(QObject *object);
    void removeObject(QObject *object);
    void setStatusAndClearList(QBindingStatus *status) noexcept;


    static bool isBindingStatus(quintptr data) noexcept
    {
        return !isNull(data) && !isList(data);
    }
    static bool isList(quintptr data) noexcept { return data & 1; }
    static bool isNull(quintptr data) noexcept { return data == 0; }

    // thread-safe:
    QBindingStatus *bindingStatus() const noexcept
    {
        // synchronizes-with the store-release in setStatusAndClearList():
        const auto d = data.load(std::memory_order_acquire);
        if (isBindingStatus(d))
            return reinterpret_cast<QBindingStatus *>(d);
        else
            return nullptr;
    }

    // requires external synchronization:
    List *list() const noexcept
    {
        return decodeList(data.load(std::memory_order_relaxed));
    }

private:
    static List *decodeList(quintptr ptr) noexcept
    {
        if (isList(ptr))
            return reinterpret_cast<List *>(ptr & ~1);
        else
            return nullptr;
    }

    static quintptr encodeBindingStatus(QBindingStatus *status) noexcept
    {
        return quintptr(status);
    }

    static quintptr encodeList(List *list) noexcept
    {
        return quintptr(list) | 1;
    }

    std::atomic<quintptr> data;
};

} // namespace BobUIPrivate

#if BOBUI_CONFIG(thread)

class Q_CORE_EXPORT QDaemonThread : public BOBUIhread
{
public:
    QDaemonThread(QObject *parent = nullptr);
    ~QDaemonThread();
};

class Q_AUTOTEST_EXPORT BOBUIhreadPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(BOBUIhread)

public:
    BOBUIhreadPrivate(BOBUIhreadData *d = nullptr);
    ~BOBUIhreadPrivate();

    void setPriority(BOBUIhread::Priority prio);
    BobUI::HANDLE threadId() const noexcept;

    mutable QMutex mutex;
    QAtomicInt quitLockRef;

    enum State : quint8 {
        // All state changes are imprecise
        NotStarted = 0,     // before start() or if failed to start
        Running = 1,        // in run()
        Finishing = 2,      // in BOBUIhreadPrivate::finish()
        Finished = 3,       // BOBUIhreadPrivate::finish() or cleanup() is done
                            //    or, if using pthread_join, joining is done
    };

    State threadState = NotStarted;
    bool exited = false;
    std::atomic<bool> interruptionRequested = false;
#ifdef Q_OS_UNIX
    bool terminated = false; // when (the first) terminate has been called
#endif

    int waiters = 0;
    int returnCode = -1;

    uint stackSize = 0;
    std::underlying_type_t<BOBUIhread::Priority> priority = BOBUIhread::InheritPriority;

    bool wait(QMutexLocker<QMutex> &locker, QDeadlineTimer deadline);

    BOBUIhread::QualityOfService serviceLevel = BOBUIhread::QualityOfService::Auto;
    void setQualityOfServiceLevel(BOBUIhread::QualityOfService qosLevel);
#ifdef Q_OS_DARWIN
    qos_class_t nativeQualityOfServiceClass() const;
#endif

#ifdef Q_OS_UNIX
    QWaitCondition thread_done;

    void wakeAll();
    static void *start(void *arg);
    void finish();          // happens early (before thread-local dtors)
    void cleanup();         // happens late (as a thread-local dtor, if possible)
#endif // Q_OS_UNIX

#ifdef Q_OS_WIN
    static unsigned int __stdcall start(void *) noexcept;
    void finish(bool lockAnyway = true) noexcept;

    BobUI::HANDLE handle;
    bool terminationEnabled, terminatePending;
#endif // Q_OS_WIN
#ifdef Q_OS_WASM
    static int idealThreadCount;
#endif
    BOBUIhreadData *data;

    static QAbstractEventDispatcher *createEventDispatcher(BOBUIhreadData *data);

    void ref()
    {
        quitLockRef.ref();
    }

    void deref()
    {
        if (!quitLockRef.deref() && threadState == Running) {
            QCoreApplication::instance()->postEvent(q_ptr, new QEvent(QEvent::Quit));
        }
    }

    QBindingStatus *bindingStatus();

    /* Returns nullptr if the object has been added, or the binding status
       if that one has been set in the meantime
    */
    QBindingStatus *addObjectWithPendingBindingStatusChange(QObject *obj);
    void removeObjectWithPendingBindingStatusChange(QObject *obj);

private:
#ifdef Q_OS_INTEGRITY
    // On INTEGRITY we set the thread name before starting it, so just fake a string
    struct FakeString {
        bool isEmpty() const { return true; }
        const char *toLocal8Bit() const { return nullptr; }
    } objectName;
#else
    // Used in BOBUIhread(Private)::start to avoid racy access to QObject::objectName,
    // unset afterwards.
    QString objectName;
#endif
};

#else // BOBUI_CONFIG(thread)

class BOBUIhreadPrivate : public QObjectPrivate
{
public:
    BOBUIhreadPrivate(BOBUIhreadData *d = nullptr);
    ~BOBUIhreadPrivate();

    mutable QMutex mutex;
    BOBUIhreadData *data;
    QBindingStatus* m_bindingStatus;
    bool running = false;

    QBindingStatus* bindingStatus() { return m_bindingStatus; }
    QBindingStatus *addObjectWithPendingBindingStatusChange(QObject *) { return nullptr; }
    void removeObjectWithPendingBindingStatusChange(QObject *) {}

    static void setCurrentThread(BOBUIhread *) { }
    static QAbstractEventDispatcher *createEventDispatcher(BOBUIhreadData *data);

    void ref() {}
    void deref() {}

    Q_DECLARE_PUBLIC(BOBUIhread)
};

#endif // BOBUI_CONFIG(thread)

class BOBUIhreadData
{
public:
    BOBUIhreadData(int initialRefCount = 1)
        : _ref(initialRefCount)
    {
        // fprintf(stderr, "BOBUIhreadData %p created\n", this);
    }
    ~BOBUIhreadData();

    static BOBUIhreadData *current()
    {
        if (BOBUIhreadData *data = currentThreadData()) Q_LIKELY_BRANCH
            return data;
        return createCurrentThreadData();
    }
    static void clearCurrentThreadData();
    static BOBUIhreadData *get2(BOBUIhread *thread)
    { Q_ASSERT_X(thread != nullptr, "BOBUIhread", "internal error"); return thread->d_func()->data; }

#if BOBUI_CONFIG(thread)
    void ref()
    {
        (void) _ref.ref();
        Q_ASSERT(_ref.loadRelaxed() != 0);
    }
    void deref()
    {
        if (!_ref.deref())
            delete this;
    }
#else
    void ref() {}
    void deref() {}
#endif
    inline bool hasEventDispatcher() const
    { return eventDispatcher.loadRelaxed() != nullptr; }
    QAbstractEventDispatcher *createEventDispatcher();
    QAbstractEventDispatcher *ensureEventDispatcher()
    {
        QAbstractEventDispatcher *ed = eventDispatcher.loadRelaxed();
        if (Q_LIKELY(ed))
            return ed;
        return createEventDispatcher();
    }

    bool canWaitLocked()
    {
        QMutexLocker locker(&postEventList.mutex);
        return canWait;
    }

    void clearEvents();

    void reuseBindingStatusForNewNativeThread()
    {
        auto status = m_statusOrPendingObjects.bindingStatus();
        if (status)
            BobUIPrivate::setBindingStatus(status, {});
    }

    QStack<QEventLoop *> eventLoops;
    QPostEventList postEventList;
    QAtomicPointer<BOBUIhread> thread;
    QAtomicPointer<void> threadId;
    QAtomicPointer<QAbstractEventDispatcher> eventDispatcher;
    QList<void *> tls;
    // manipulating m_statusOrPendingObjects requires BOBUIreadPrivate's mutex to be locked
    BobUIPrivate::BindingStatusOrList m_statusOrPendingObjects = {};

private:
    QAtomicInt _ref;

public:
    int loopLevel = 0;
    int scopeLevel = 0;

    bool quitNow = false;
    bool canWait = true;
    bool isAdopted = false;
    bool requiresCoreApplication = true;

private:
    friend class QAbstractEventDispatcher;
    friend class QBasicTimer;
    Q_DECL_PURE_FUNCTION static Q_AUTOTEST_EXPORT BOBUIhreadData *currentThreadData() noexcept;
    static Q_AUTOTEST_EXPORT BOBUIhreadData *createCurrentThreadData();
};

class QScopedScopeLevelCounter
{
    BOBUIhreadData *threadData;
public:
    inline QScopedScopeLevelCounter(BOBUIhreadData *threadData)
        : threadData(threadData)
    { ++threadData->scopeLevel; }
    inline ~QScopedScopeLevelCounter()
    { --threadData->scopeLevel; }
};

// thread wrapper for the main() thread
class QAdoptedThread : public BOBUIhread
{
    Q_DECLARE_PRIVATE(BOBUIhread)

public:
    QAdoptedThread(BOBUIhreadData *data);
    ~QAdoptedThread();
    void init();

private:
#if BOBUI_CONFIG(thread)
    void run() override;
#endif
};

#if BOBUI_CONFIG(thread)
inline QBindingStatus *BOBUIhreadPrivate::bindingStatus()
{
    return data->m_statusOrPendingObjects.bindingStatus();
}
#endif

BOBUI_END_NAMESPACE

#endif // BOBUIHREAD_P_H
