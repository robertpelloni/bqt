// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QFUTUREINTERFACE_P_H
#define QFUTUREINTERFACE_P_H

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

#include <BobUICore/private/qglobal_p.h>
#include <BobUICore/qelapsedtimer.h>
#include <BobUICore/qcoreevent.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qwaitcondition.h>
#include <BobUICore/qrunnable.h>
#include <BobUICore/bobuihreadpool.h>
#include <BobUICore/qfutureinterface.h>
#include <BobUICore/qexception.h>

BOBUI_REQUIRE_CONFIG(future);

BOBUI_BEGIN_NAMESPACE

// Although QFutureCallOutEvent and QFutureCallOutInterface are private,
// for historical reasons they were used externally (in BobUIJambi, see
// https://github.com/OmixVisualization/bobuijambi), so we export them to
// not break the pre-existing code.
class Q_CORE_EXPORT QFutureCallOutEvent : public QEvent
{
    Q_DECL_EVENT_COMMON(QFutureCallOutEvent)
public:
    enum CallOutType {
        Started,
        Finished,
        Canceled,
        Suspending,
        Suspended,
        Resumed,
        Progress,
        ProgressRange,
        ResultsReady
    };

    QFutureCallOutEvent()
        : QEvent(QEvent::FutureCallOut), callOutType(CallOutType(0)), index1(-1), index2(-1)
    { }
    explicit QFutureCallOutEvent(CallOutType callOutType, int index1 = -1)
        : QEvent(QEvent::FutureCallOut), callOutType(callOutType), index1(index1), index2(-1)
    { }
    QFutureCallOutEvent(CallOutType callOutType, int index1, int index2)
        : QEvent(QEvent::FutureCallOut), callOutType(callOutType), index1(index1), index2(index2)
    { }

    QFutureCallOutEvent(CallOutType callOutType, int index1, const QString &text)
        : QEvent(QEvent::FutureCallOut),
          callOutType(callOutType),
          index1(index1),
          index2(-1),
          text(text)
    { }

    CallOutType callOutType;
    int index1;
    int index2;
    QString text;
};

class Q_CORE_EXPORT QFutureCallOutInterface
{
public:
    virtual ~QFutureCallOutInterface();
    virtual void postCallOutEvent(const QFutureCallOutEvent &) = 0;
    virtual void callOutInterfaceDisconnected() = 0;
};

class QFutureInterfaceBasePrivate
{
public:
    QFutureInterfaceBasePrivate(QFutureInterfaceBase::State initialState);
    ~QFutureInterfaceBasePrivate();

    // When the last QFuture<T> reference is removed, we need to make
    // sure that data stored in the ResultStore is cleaned out.
    // Since QFutureInterfaceBasePrivate can be shared between QFuture<T>
    // and QFuture<void> objects, we use a separate ref. counter
    // to keep track of QFuture<T> objects.
    class RefCount
    {
    public:
        inline RefCount(int r = 0, int rt = 0)
            : m_refCount(r), m_refCountT(rt) {}
        // Default ref counter for QFIBP
        inline bool ref() { return m_refCount.ref(); }
        inline bool deref() { return m_refCount.deref(); }
        inline int load() const { return m_refCount.loadRelaxed(); }
        // Ref counter for type T
        inline bool refT() { return m_refCountT.ref(); }
        inline bool derefT() { return m_refCountT.deref(); }
        inline int loadT() const { return m_refCountT.loadRelaxed(); }

    private:
        QAtomicInt m_refCount;
        QAtomicInt m_refCountT;
    };

    // T: accessed from executing thread
    // Q: accessed from the waiting/querying thread
    mutable QMutex m_mutex;
    QBasicMutex continuationMutex;
    QList<QFutureCallOutInterface *> outputConnections;
    QElapsedTimer progressTime;
    QWaitCondition waitCondition;
    QWaitCondition pausedWaitCondition;

    union Data {
        BobUIPrivate::ResultStoreBase m_results;
        BobUIPrivate::ExceptionStore m_exceptionStore;

#ifndef BOBUI_NO_EXCEPTIONS
        void setException(const std::exception_ptr &e)
        {
            m_results.~ResultStoreBase();
            new (&m_exceptionStore) BobUIPrivate::ExceptionStore();
            m_exceptionStore.setException(e);
        }
#endif

        ~Data() { }
    };
    Data data = { BobUIPrivate::ResultStoreBase() };

    QRunnable *runnable = nullptr;
    BOBUIhreadPool *m_pool = nullptr;
    // Wrapper for continuation
    std::function<void(const QFutureInterfaceBase &)> continuation;
    QFutureInterfaceBasePrivate *continuationData = nullptr;
    // will reset back to nullptr when the parent future is done
    // (finished, canceled, failed etc)
    QFutureInterfaceBasePrivate *nonConcludedParent = nullptr;

    RefCount refCount = 1;
    QAtomicInt state; // reads and writes can happen unprotected, both must be atomic

    int m_progressValue = 0; // TQ
    struct ProgressData
    {
        int minimum = 0; // TQ
        int maximum = 0; // TQ
        QString text;
    };
    QScopedPointer<ProgressData> m_progress;

    int m_expectedResultCount = 0;
    bool launchAsync = false;
    bool isValid = false;
    bool hasException = false;

    enum ContinuationState : quint8 { Default, Canceled, Cleaned };
    std::atomic<ContinuationState> continuationState { Default };
    bool continuationExecuted = false;

    QFutureInterfaceBase::ContinuationType continuationType = QFutureInterfaceBase::ContinuationType::Unknown;

    inline BOBUIhreadPool *pool() const
    { return m_pool ? m_pool : BOBUIhreadPool::globalInstance(); }

    // Internal functions that does not change the mutex state.
    // The mutex must be locked when calling these.
    int internal_resultCount() const;
    bool internal_isResultReadyAt(int index) const;
    bool internal_waitForNextResult();
    bool internal_updateProgressValue(int progress);
    bool internal_updateProgress(int progress, const QString &progressText = QString());
    void internal_setThrottled(bool enable);
    void sendCallOut(const QFutureCallOutEvent &callOut);
    void sendCallOuts(const QFutureCallOutEvent &callOut1, const QFutureCallOutEvent &callOut2);
    void connectOutputInterface(QFutureCallOutInterface *iface);
    void disconnectOutputInterface(QFutureCallOutInterface *iface);

    void setState(QFutureInterfaceBase::State state);

    enum class CancelOption : quint32
    {
        None = 0x00,
        CancelContinuations = 0x01,
    };
    Q_DECLARE_FLAGS(CancelOptions, CancelOption)
    void cancelImpl(QFutureInterfaceBase::CancelMode mode, CancelOptions options);
};

BOBUI_END_NAMESPACE

#endif
