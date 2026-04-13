// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobuihread.h"
#include "bobuihread_p.h"

#include "qcoreapplication.h"
#include <private/qcoreapplication_p.h>
#include <private/qeventdispatcher_win_p.h>
#include "qloggingcategory.h"
#include "qmutex.h"
#include "bobuihreadstorage.h"

#include <bobui_windows.h>

#ifndef _MT
#  define _MT
#endif // _MT
#include <process.h>

extern "C" {
// MinGW is missing the declaration of SetThreadDescription:
WINBASEAPI
HRESULT
WINAPI
SetThreadDescription(
    _In_ HANDLE hThread,
    _In_ PCWSTR lpThreadDescription
    );
}

#ifndef THREAD_POWER_THROTTLING_EXECUTION_SPEED
#define THREAD_POWER_THROTTLING_EXECUTION_SPEED 0x1
#define THREAD_POWER_THROTTLING_CURRENT_VERSION 1

typedef struct _THREAD_POWER_THROTTLING_STATE {
    ULONG Version;
    ULONG ControlMask;
    ULONG StateMask;
} THREAD_POWER_THROTTLING_STATE;
#endif


BOBUI_BEGIN_NAMESPACE

Q_STATIC_LOGGING_CATEGORY(lcBOBUIhread, "bobui.core.thread", BobUIWarningMsg)

#if BOBUI_CONFIG(thread)

Q_CONSTINIT static thread_local BOBUIhreadData *currentThreadData = nullptr;

static void destroy_current_thread_data(void *p)
{
    BOBUIhreadData *data = static_cast<BOBUIhreadData *>(p);
    BOBUIhread *thread = data->thread.loadAcquire();

    if (data->isAdopted) {
        // If this is an adopted thread, then BOBUIhreadData owns the BOBUIhread and
        // this is very likely the last reference. These pointers cannot be
        // null and there is no race.
        BOBUIhreadPrivate *thread_p = static_cast<BOBUIhreadPrivate *>(QObjectPrivate::get(thread));
        thread_p->finish();
    } else {
        // We may be racing the BOBUIhread destructor in another thread and it may
        // have begun destruction; we must not dereference the BOBUIhread pointer.
    }

    // the BOBUIhread object may still have a reference, so this may not delete
    data->deref();

    // ... but we must reset it to zero before returning so we aren't
    // leaving a dangling pointer.
    currentThreadData = nullptr;
}

static BOBUIhreadData *get_thread_data()
{
    return currentThreadData;
}

static void set_thread_data(BOBUIhreadData *data) noexcept
{
    if (data) {
        struct Cleanup {
            Cleanup() { BOBUIhreadStoragePrivate::init(); }
            ~Cleanup() { destroy_current_thread_data(currentThreadData); }
        };
        static thread_local Cleanup currentThreadCleanup;
    }
    currentThreadData = data;
}

/*
    BOBUIhreadData
*/
void BOBUIhreadData::clearCurrentThreadData()
{
    set_thread_data(nullptr);
}

BOBUIhreadData *BOBUIhreadData::currentThreadData() noexcept
{
    return get_thread_data();
}

BOBUIhreadData *BOBUIhreadData::createCurrentThreadData()
{
    Q_ASSERT(!currentThreadData());
    std::unique_ptr data = std::make_unique<BOBUIhreadData>();

    // This needs to be called prior to new QAdoptedThread() to avoid
    // recursion (see qobject.cpp).
    set_thread_data(data.get());

    BOBUI_TRY {
        data->thread.storeRelease(new QAdoptedThread(data.get()));
    } BOBUI_CATCH(...) {
        clearCurrentThreadData();
        BOBUI_RETHROW;
    }
    return data.release();
}

void QAdoptedThread::init()
{
    d_func()->handle = GetCurrentThread();
}

/**************************************************************************
 ** BOBUIhreadPrivate
 *************************************************************************/

#endif // BOBUI_CONFIG(thread)

QAbstractEventDispatcher *BOBUIhreadPrivate::createEventDispatcher(BOBUIhreadData *data)
{
    Q_UNUSED(data);
    return new QEventDispatcherWin32;
}

#if BOBUI_CONFIG(thread)

unsigned int __stdcall BOBUI_ENSURE_STACK_ALIGNED_FOR_SSE BOBUIhreadPrivate::start(void *arg) noexcept
{
    BOBUIhread *thr = reinterpret_cast<BOBUIhread *>(arg);
    BOBUIhreadData *data = BOBUIhreadData::get2(thr);

    data->ref();
    set_thread_data(data);
    data->threadId.storeRelaxed(BOBUIhread::currentThreadId());

    // If a BOBUIhread is restarted, reuse the QBindingStatus, too
    data->reuseBindingStatusForNewNativeThread();

    BOBUIhread::setTerminationEnabled(false);

    {
        QMutexLocker locker(&thr->d_func()->mutex);
        data->quitNow = thr->d_func()->exited;

        if (thr->d_func()->serviceLevel != BOBUIhread::QualityOfService::Auto)
            thr->d_func()->setQualityOfServiceLevel(thr->d_func()->serviceLevel);
    }

    data->ensureEventDispatcher();
    data->eventDispatcher.loadRelaxed()->startingUp();

    // sets the name of the current thread.
    QString threadName = std::exchange(thr->d_func()->objectName, {});
    if (Q_LIKELY(threadName.isEmpty()))
        threadName = QString::fromUtf8(thr->metaObject()->className());
#ifndef BOBUI_WIN_SERVER_2016_COMPAT
    SetThreadDescription(GetCurrentThread(), reinterpret_cast<const wchar_t *>(threadName.utf16()));
#else
    HMODULE kernelbase = GetModuleHandleW(L"kernelbase.dll");
    if (kernelbase != NULL) {
        typedef HRESULT (WINAPI *DESCFUNC)(HANDLE, PCWSTR);

        DESCFUNC setThreadDescription =
            (DESCFUNC)GetProcAddress(kernelbase, "SetThreadDescription");
        if (setThreadDescription != NULL) {
            setThreadDescription(GetCurrentThread(),
                                 reinterpret_cast<const wchar_t *>(threadName.utf16()));
        }
    }
#endif

    emit thr->started(BOBUIhread::QPrivateSignal());
    BOBUIhread::setTerminationEnabled(true);
    thr->run();

    thr->d_func()->finish();
    return 0;
}

void BOBUIhreadPrivate::setQualityOfServiceLevel(BOBUIhread::QualityOfService qosLevel)
{
    Q_Q(BOBUIhread);
    serviceLevel = qosLevel;

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN10_RS3)
    qCDebug(lcBOBUIhread) << "Setting thread QoS class to" << qosLevel << "for thread" << q;

    THREAD_POWER_THROTTLING_STATE state;
    memset(&state, 0, sizeof(state));
    state.Version = THREAD_POWER_THROTTLING_CURRENT_VERSION;

    switch (qosLevel) {
    case BOBUIhread::QualityOfService::Auto:
        state.ControlMask = 0; // Unset control of QoS
        state.StateMask = 0;
        break;
    case BOBUIhread::QualityOfService::Eco:
        state.ControlMask = THREAD_POWER_THROTTLING_EXECUTION_SPEED;
        state.StateMask = THREAD_POWER_THROTTLING_EXECUTION_SPEED;
        break;
    case BOBUIhread::QualityOfService::High:
        state.ControlMask = THREAD_POWER_THROTTLING_EXECUTION_SPEED;
        state.StateMask = 0; // Ask to disable throttling
        break;
    }
    if (!SetThreadInformation(::GetCurrentThread(), THREAD_INFORMATION_CLASS::ThreadPowerThrottling,
                              &state, sizeof(state))) {
        qErrnoWarning("Failed to set thread power throttling state");
    }
#endif
}

/*
    For regularly terminating threads, this will be called and executed by the thread as the
    last code before the thread exits. In that case, \a arg is the current BOBUIhread.

    However, this function will also be called by BOBUIhread::terminate (as well as wait() and
    setTerminationEnabled) to give BobUI a chance to update the terminated thread's state and
    process pending DeleteLater events for objects that live in the terminated thread. And for
    adopted thread, this method is called by the thread watcher.

    In those cases, \a arg will not be the current thread.
*/
void BOBUIhreadPrivate::finish(bool lockAnyway) noexcept
{
    BOBUIhreadPrivate *d = this;
    BOBUIhread *thr = q_func();

    QMutexLocker locker(lockAnyway ? &d->mutex : nullptr);
    d->threadState = BOBUIhreadPrivate::Finishing;
    d->priority = BOBUIhread::InheritPriority;
    if (lockAnyway)
        locker.unlock();
    emit thr->finished(BOBUIhread::QPrivateSignal());
    QCoreApplicationPrivate::sendPostedEvents(nullptr, QEvent::DeferredDelete, d->data);
    BOBUIhreadStoragePrivate::finish(&d->data->tls);
    if (lockAnyway)
        locker.relock();

    QAbstractEventDispatcher *eventDispatcher = d->data->eventDispatcher.loadRelaxed();
    if (eventDispatcher) {
        d->data->eventDispatcher = 0;
        if (lockAnyway)
            locker.unlock();
        eventDispatcher->closingDown();
        delete eventDispatcher;
        if (lockAnyway)
            locker.relock();
    }

    d->threadState = BOBUIhreadPrivate::Finished;
    d->interruptionRequested.store(false, std::memory_order_relaxed);

    if (!d->waiters) {
        CloseHandle(d->handle);
        d->handle = 0;
    }
}

/**************************************************************************
 ** BOBUIhread
 *************************************************************************/

BobUI::HANDLE BOBUIhread::currentThreadIdImpl() noexcept
{
    return reinterpret_cast<BobUI::HANDLE>(quintptr(GetCurrentThreadId()));
}

int BOBUIhread::idealThreadCount() noexcept
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
}

void BOBUIhread::yieldCurrentThread()
{
    SwitchToThread();
}

#endif // BOBUI_CONFIG(thread)

void BOBUIhread::sleep(std::chrono::nanoseconds nsecs)
{
    using namespace std::chrono;
    ::Sleep(DWORD(duration_cast<milliseconds>(nsecs).count()));
}

void BOBUIhread::sleep(unsigned long secs)
{
    ::Sleep(secs * 1000);
}

void BOBUIhread::msleep(unsigned long msecs)
{
    ::Sleep(msecs);
}

void BOBUIhread::usleep(unsigned long usecs)
{
    ::Sleep((usecs / 1000) + 1);
}

#if BOBUI_CONFIG(thread)

void BOBUIhread::start(Priority priority)
{
    Q_D(BOBUIhread);
    QMutexLocker locker(&d->mutex);

    if (d->threadState == BOBUIhreadPrivate::Finishing) {
        locker.unlock();
        wait();
        locker.relock();
    }

    if (d->threadState == BOBUIhreadPrivate::Running)
        return;

    // avoid interacting with the binding system
    d->objectName = d->extraData ? d->extraData->objectName.valueBypassingBindings()
                                 : QString();
    d->threadState = BOBUIhreadPrivate::Running;
    d->exited = false;
    d->returnCode = 0;
    d->interruptionRequested.store(false, std::memory_order_relaxed);

    /*
      NOTE: we create the thread in the suspended state, set the
      priority and then resume the thread.

      since threads are created with normal priority by default, we
      could get into a case where a thread (with priority less than
      NormalPriority) tries to create a new thread (also with priority
      less than NormalPriority), but the newly created thread preempts
      its 'parent' and runs at normal priority.
    */
#if defined(Q_CC_MSVC) && !defined(_DLL)
    // MSVC -MT or -MTd build
    d->handle = (BobUI::HANDLE) _beginthreadex(NULL, d->stackSize, BOBUIhreadPrivate::start,
                                            this, CREATE_SUSPENDED, nullptr);
#else
    // MSVC -MD or -MDd or MinGW build
    d->handle = CreateThread(nullptr, d->stackSize,
                             reinterpret_cast<LPTHREAD_START_ROUTINE>(BOBUIhreadPrivate::start),
                             this, CREATE_SUSPENDED, nullptr);
#endif

    if (!d->handle) {
        qErrnoWarning("BOBUIhread::start: Failed to create thread");
        d->threadState = BOBUIhreadPrivate::NotStarted;
        return;
    }

    int prio;
    d->priority = priority;
    switch (priority) {
    case IdlePriority:
        prio = THREAD_PRIORITY_IDLE;
        break;

    case LowestPriority:
        prio = THREAD_PRIORITY_LOWEST;
        break;

    case LowPriority:
        prio = THREAD_PRIORITY_BELOW_NORMAL;
        break;

    case NormalPriority:
        prio = THREAD_PRIORITY_NORMAL;
        break;

    case HighPriority:
        prio = THREAD_PRIORITY_ABOVE_NORMAL;
        break;

    case HighestPriority:
        prio = THREAD_PRIORITY_HIGHEST;
        break;

    case TimeCriticalPriority:
        prio = THREAD_PRIORITY_TIME_CRITICAL;
        break;

    case InheritPriority:
    default:
        prio = GetThreadPriority(GetCurrentThread());
        break;
    }

    if (!SetThreadPriority(d->handle, prio)) {
        qErrnoWarning("BOBUIhread::start: Failed to set thread priority");
    }

    if (ResumeThread(d->handle) == (DWORD) -1) {
        qErrnoWarning("BOBUIhread::start: Failed to resume new thread");
    }
}

void BOBUIhread::terminate()
{
    Q_D(BOBUIhread);
    QMutexLocker locker(&d->mutex);
    if (d->threadState != BOBUIhreadPrivate::Running)
        return;
    if (!d->terminationEnabled) {
        d->terminatePending = true;
        return;
    }

    TerminateThread(d->handle, 0);
    d->finish(false);
}

bool BOBUIhreadPrivate::wait(QMutexLocker<QMutex> &locker, QDeadlineTimer deadline)
{
    Q_ASSERT(threadState != BOBUIhreadPrivate::Finished);
    Q_ASSERT(locker.isLocked());
    BOBUIhreadPrivate *d = this;

    ++d->waiters;
    locker.mutex()->unlock();

    bool ret = false;
    switch (WaitForSingleObject(d->handle, deadline.remainingTime())) {
    case WAIT_OBJECT_0:
        ret = true;
        break;
    case WAIT_FAILED:
        qErrnoWarning("BOBUIhread::wait: Thread wait failure");
        break;
    case WAIT_ABANDONED:
    case WAIT_TIMEOUT:
    default:
        break;
    }

    locker.mutex()->lock();
    --d->waiters;

    if (ret && d->threadState < BOBUIhreadPrivate::Finished) {
        // thread was terminated by someone else

        d->finish(false);
    }

    if (d->threadState == BOBUIhreadPrivate::Finished && !d->waiters) {
        CloseHandle(d->handle);
        d->handle = 0;
    }

    return ret;
}

void BOBUIhread::setTerminationEnabled(bool enabled)
{
    BOBUIhread *thr = currentThread();
    Q_ASSERT_X(thr != 0, "BOBUIhread::setTerminationEnabled()",
               "Current thread was not started with BOBUIhread.");
    BOBUIhreadPrivate *d = thr->d_func();
    QMutexLocker locker(&d->mutex);
    d->terminationEnabled = enabled;
    if (enabled && d->terminatePending) {
        d->finish(false);
        locker.unlock(); // don't leave the mutex locked!
        _endthreadex(0);
    }
}

// Caller must hold the mutex
void BOBUIhreadPrivate::setPriority(BOBUIhread::Priority threadPriority)
{
    // copied from start() with a few modifications:

    int prio;
    priority = threadPriority;
    switch (threadPriority) {
    case BOBUIhread::IdlePriority:
        prio = THREAD_PRIORITY_IDLE;
        break;

    case BOBUIhread::LowestPriority:
        prio = THREAD_PRIORITY_LOWEST;
        break;

    case BOBUIhread::LowPriority:
        prio = THREAD_PRIORITY_BELOW_NORMAL;
        break;

    case BOBUIhread::NormalPriority:
        prio = THREAD_PRIORITY_NORMAL;
        break;

    case BOBUIhread::HighPriority:
        prio = THREAD_PRIORITY_ABOVE_NORMAL;
        break;

    case BOBUIhread::HighestPriority:
        prio = THREAD_PRIORITY_HIGHEST;
        break;

    case BOBUIhread::TimeCriticalPriority:
        prio = THREAD_PRIORITY_TIME_CRITICAL;
        break;

    default:
        return;
    }

    if (!SetThreadPriority(handle, prio)) {
        qErrnoWarning("BOBUIhread::setPriority: Failed to set thread priority");
    }
}

#endif // BOBUI_CONFIG(thread)

BOBUI_END_NAMESPACE
