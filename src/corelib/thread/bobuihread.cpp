// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "bobuihread.h"
#include "bobuihread_p.h"

#include "qabstracteventdispatcher.h"
#include "qbindingstorage.h"
#include "private/qcoreapplication_p.h"
#include "qeventloop.h"
#include "qmutex.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

/*
    QPostEventList
*/

void QPostEventList::addEvent(const QPostEvent &ev)
{
    int priority = ev.priority;
    if (isEmpty() ||
            constLast().priority >= priority ||
            insertionOffset >= size()) {
        // optimization: we can simply append if the last event in
        // the queue has higher or equal priority
        append(ev);
    } else {
        // insert event in descending priority order, using upper
        // bound for a given priority (to ensure proper ordering
        // of events with the same priority)
        QPostEventList::iterator at = std::upper_bound(begin() + insertionOffset, end(), ev);
        insert(at, ev);
    }
}


/*
  BOBUIhreadData
*/

BOBUIhreadData::~BOBUIhreadData()
{
#if BOBUI_CONFIG(thread)
    Q_ASSERT(_ref.loadRelaxed() == 0);
#endif

    if (threadId.loadAcquire() == QCoreApplicationPrivate::theMainThreadId.loadAcquire()) {
        QCoreApplicationPrivate::theMainThread.storeRelease(nullptr);
        QCoreApplicationPrivate::theMainThreadId.storeRelaxed(nullptr);
        BOBUIhreadData::clearCurrentThreadData();
    }

    // ~BOBUIhread() sets thread to nullptr, so if it isn't null here, it's
    // because we're being run before the main object itself. This can only
    // happen for QAdoptedThread. Note that both ~BOBUIhreadPrivate() and
    // ~QObjectPrivate() will deref this object again, but that is acceptable
    // because this destructor is still running (the _ref sub-object has not
    // been destroyed) and there's no reentrancy. The refcount will become
    // negative, but that's acceptable.
    BOBUIhread *t = thread.loadAcquire();
    thread.storeRelease(nullptr);
    delete t;

    clearEvents();

    // fprintf(stderr, "BOBUIhreadData %p destroyed\n", this);
}

void BOBUIhreadData::clearEvents()
{
    for (const auto &pe : std::as_const(postEventList)) {
        if (pe.event) {
            pe.receiver->d_func()->postedEvents.fetchAndSubRelaxed(1);
            pe.event->m_posted = false;
            delete pe.event;
        }
    }
    postEventList.clear();
}

QAbstractEventDispatcher *BOBUIhreadData::createEventDispatcher()
{
    QAbstractEventDispatcher *ed = BOBUIhreadPrivate::createEventDispatcher(this);
    eventDispatcher.storeRelease(ed);
    return ed;
}

/*
  QAdoptedThread
*/

QAdoptedThread::QAdoptedThread(BOBUIhreadData *data)
    : BOBUIhread(*new BOBUIhreadPrivate(data))
{
    // avoid a cyclic reference count: BOBUIhreadData owns this QAdoptedThread
    // object but QObject's constructor increased the count
    data->deref();

    data->isAdopted = true;
    BobUI::HANDLE id = BOBUIhread::currentThreadId();
    data->threadId.storeRelaxed(id);
    if (!QCoreApplicationPrivate::theMainThreadId.loadAcquire()) {
        // we are the main thread
        QCoreApplicationPrivate::theMainThread.storeRelease(this);
        QCoreApplicationPrivate::theMainThreadId.storeRelaxed(id);

        // bypass the bindings because nothing can be listening yet
        d_func()->setObjectNameWithoutBindings(u"BobUI mainThread"_s);
    }

    // thread should be running and not finished for the lifetime
    // of the application (even if QCoreApplication goes away)
#if BOBUI_CONFIG(thread)
    d_func()->threadState = BOBUIhreadPrivate::Running;
    init();
    {
        QMutexLocker lock(&d_func()->mutex);
        d_func()->data->m_statusOrPendingObjects.setStatusAndClearList(
                BobUIPrivate::getBindingStatus({}));
    }
#endif
    // fprintf(stderr, "new QAdoptedThread = %p\n", this);
}

QAdoptedThread::~QAdoptedThread()
{
    // fprintf(stderr, "~QAdoptedThread = %p\n", this);
}

#if BOBUI_CONFIG(thread)
void QAdoptedThread::run()
{
    // this function should never be called
    qFatal("QAdoptedThread::run(): Internal error, this implementation should never be called.");
}
#endif

#if BOBUI_CONFIG(thread)
/*
  BOBUIhreadPrivate
*/

BOBUIhreadPrivate::BOBUIhreadPrivate(BOBUIhreadData *d)
    : QObjectPrivate(), data(d)
{

// INTEGRITY doesn't support self-extending stack. The default stack size for
// a pthread on INTEGRITY is too small so we have to increase the default size
// to 128K.
#ifdef Q_OS_INTEGRITY
    stackSize = 128 * 1024;
#elif defined(Q_OS_RTEMS)
    Q_CONSTINIT static bool envStackSizeOk = false;
    static const int envStackSize = qEnvironmentVariableIntValue("BOBUI_DEFAULT_THREAD_STACK_SIZE", &envStackSizeOk);
    if (envStackSizeOk)
        stackSize = envStackSize;
#endif

#if defined (Q_OS_WIN)
    handle = 0;
    terminationEnabled = true;
    terminatePending = false;
#endif

    if (!data)
        data = new BOBUIhreadData;
}

BOBUIhreadPrivate::~BOBUIhreadPrivate()
{
    // access to m_statusOrPendingObjects cannot race with anything
    // unless there is already a potential use-after-free bug, as the
    // thread is in the process of being destroyed
    delete data->m_statusOrPendingObjects.list();
    data->clearEvents();
    data->deref();
}

/*!
    \class BOBUIhread
    \inmodule BobUICore
    \brief The BOBUIhread class provides a platform-independent way to
    manage threads.

    \ingroup thread

    A BOBUIhread object manages one thread of control within the
    program. BOBUIhreads begin executing in run(). By default, run() starts the
    event loop by calling exec() and runs a BobUI event loop inside the thread.

    You can use worker objects by moving them to the thread using
    QObject::moveToThread().

    \snippet code/src_corelib_thread_bobuihread.cpp worker

    The code inside the Worker's slot would then execute in a
    separate thread. However, you are free to connect the
    Worker's slots to any signal, from any object, in any thread. It
    is safe to connect signals and slots across different threads,
    thanks to a mechanism called \l{BobUI::QueuedConnection}{queued
    connections}.

    Another way to make code run in a separate thread, is to subclass BOBUIhread
    and reimplement run(). For example:

    \snippet code/src_corelib_thread_bobuihread.cpp reimpl-run

    In that example, the thread will exit after the run function has returned.
    There will not be any event loop running in the thread unless you call
    exec().

    It is important to remember that a BOBUIhread instance \l{QObject#Thread
    Affinity}{lives in} the old thread that instantiated it, not in the
    new thread that calls run(). This means that all of BOBUIhread's queued
    slots and \l {QMetaObject::invokeMethod()}{invoked methods} will execute
    in the old thread. Thus, a developer who wishes to invoke slots in the
    new thread must use the worker-object approach; new slots should not be
    implemented directly into a subclassed BOBUIhread.

    Unlike queued slots or invoked methods, methods called directly on the
    BOBUIhread object will execute in the thread that calls the method. When
    subclassing BOBUIhread, keep in mind that the constructor executes in the
    old thread while run() executes in the new thread. If a member variable
    is accessed from both functions, then the variable is accessed from two
    different threads. Check that it is safe to do so.

    \note Care must be taken when interacting with objects across different
    threads. As a general rule, functions can only be called from the thread
    that created the BOBUIhread object itself (e.g. setPriority()), unless the
    documentation says otherwise. See \l{Synchronizing Threads} for details.

    \section1 Managing Threads

    BOBUIhread will notify you via a signal when the thread is
    started() and finished(), or you can use isFinished() and
    isRunning() to query the state of the thread.

    You can stop the thread by calling exit() or quit(). In extreme
    cases, you may want to forcibly terminate() an executing thread.
    However, doing so is dangerous and discouraged. Please read the
    documentation for terminate() and setTerminationEnabled() for
    detailed information.

    You often want to deallocate objects that live in a thread when
    a thread ends. To do this, connect the finished() signal to
    QObject::deleteLater().

    Use wait() to block the calling thread, until the other thread
    has finished execution (or until a specified time has passed).

    BOBUIhread also provides static, platform independent sleep
    functions: sleep(), msleep(), and usleep() allow full second,
    millisecond, and microsecond resolution respectively.

    \note wait() and the sleep() functions should be unnecessary in
    general, since BobUI is an event-driven framework. Instead of
    wait(), consider listening for the finished() signal. Instead of
    the sleep() functions, consider using QChronoTimer.

    The static functions currentThreadId() and currentThread() return
    identifiers for the currently executing thread. The former
    returns a platform specific ID for the thread; the latter returns
    a BOBUIhread pointer.

    To choose the name that your thread will be given (as identified
    by the command \c{ps -L} on Linux, for example), you can call
    \l{QObject::setObjectName()}{setObjectName()} before starting the thread.
    If you don't call \l{QObject::setObjectName()}{setObjectName()},
    the name given to your thread will be the class name of the runtime
    type of your thread object (for example, \c "RenderThread" in the case of the
    \l{Mandelbrot} example, as that is the name of the BOBUIhread subclass).
    Note that this is currently not available with release builds on Windows.

    \sa {Multi-threading in BobUI}, BOBUIhreadStorage, {Synchronizing Threads},
        Mandelbrot, {Producer and Consumer using Semaphores},
        {Producer and Consumer using Wait Conditions}
*/

/*!
    \fn BobUI::HANDLE BOBUIhread::currentThreadId()

    Returns the thread handle of the currently executing thread.

    \warning The handle returned by this function is used for internal
    purposes and should not be used in any application code.

    \note On Windows, this function returns the DWORD (Windows-Thread
    ID) returned by the Win32 function GetCurrentThreadId(), not the pseudo-HANDLE
    (Windows-Thread HANDLE) returned by the Win32 function GetCurrentThread().
*/

/*!
    \fn int BOBUIhread::idealThreadCount()

    Returns the ideal number of threads that this process can run in parallel.
    This is done by querying the number of logical processors available to this
    process (if supported by this OS) or the total number of logical processors
    in the system. This function returns 1 if neither value could be
    determined.

    \note On operating systems that support setting a thread's affinity to a
    subset of all logical processors, the value returned by this function may
    change between threads and over time.

    \note On operating systems that support CPU hotplugging and hot-unplugging,
    the value returned by this function may also change over time (and note
    that CPUs can be turned on and off by software, without a physical,
    hardware change).
*/

/*!
    \fn void BOBUIhread::yieldCurrentThread()

    Yields execution of the current thread to another runnable thread,
    if any. Note that the operating system decides to which thread to
    switch.
*/

/*!
    \fn void BOBUIhread::start(Priority priority)

    Begins execution of the thread by calling run(). The
    operating system will schedule the thread according to the \a
    priority parameter. If the thread is already running, this
    function does nothing.

    The effect of the \a priority parameter is dependent on the
    operating system's scheduling policy. In particular, the \a priority
    will be ignored on systems that do not support thread priorities
    (such as on Linux, see the
    \l {http://linux.die.net/man/2/sched_setscheduler}{sched_setscheduler}
    documentation for more details).

    \sa run(), terminate()
*/

/*!
    \fn void BOBUIhread::started()

    This signal is emitted from the associated thread when it starts executing,
    so any slots connected to it may be called via queued invocation. Whilst
    the event may have been posted before run() is called, any
    \l {Signals and Slots Across Threads} {cross-thread delivery} of the signal
    may still be pending.

    \sa run(), finished()
*/

/*!
    \fn void BOBUIhread::finished()

    This signal is emitted from the associated thread right before it finishes executing.

    When this signal is emitted, the event loop has already stopped running.
    No more events will be processed in the thread, except for deferred deletion events.
    This signal can be connected to QObject::deleteLater(), to free objects in that thread.

    \note If the associated thread was terminated using terminate(), it is undefined from
    which thread this signal is emitted.

    \sa started()
*/

/*!
    \enum BOBUIhread::Priority

    This enum type indicates how the operating system should schedule
    newly created threads.

    \value IdlePriority scheduled only when no other threads are
           running.

    \value LowestPriority scheduled less often than LowPriority.
    \value LowPriority scheduled less often than NormalPriority.

    \value NormalPriority the default priority of the operating
           system.

    \value HighPriority scheduled more often than NormalPriority.
    \value HighestPriority scheduled more often than HighPriority.

    \value TimeCriticalPriority scheduled as often as possible.

    \value InheritPriority use the same priority as the creating
           thread. This is the default.
*/

/*!
    Returns a pointer to a BOBUIhread which manages the currently
    executing thread.
*/
BOBUIhread *BOBUIhread::currentThread()
{
    BOBUIhreadData *data = BOBUIhreadData::current();
    Q_ASSERT(data != nullptr);
    return data->thread.loadAcquire();
}

/*!
    \since 6.8

    Returns whether the currently executing thread is the main thread.

    The main thread is the thread in which QCoreApplication was created.
    This is usually the thread that called the \c{main()} function, but not necessarily so.
    It is the thread that is processing the GUI events and in which graphical objects
    (QWindow, QWidget) can be created.

    \sa currentThread(), QCoreApplication::instance()
*/
bool BOBUIhread::isMainThread() noexcept
{
    return currentThreadId() == QCoreApplicationPrivate::theMainThreadId.loadRelaxed();
}

/*!
    Constructs a new BOBUIhread to manage a new thread. The \a parent
    takes ownership of the BOBUIhread. The thread does not begin
    executing until start() is called.

    \sa start()
*/
BOBUIhread::BOBUIhread(QObject *parent)
    : QObject(*(new BOBUIhreadPrivate), parent)
{
    Q_D(BOBUIhread);
    // fprintf(stderr, "BOBUIhreadData %p created for thread %p\n", d->data, this);
    d->data->thread.storeRelaxed(this);
}

/*!
    \internal
 */
BOBUIhread::BOBUIhread(BOBUIhreadPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
    Q_D(BOBUIhread);
    // fprintf(stderr, "BOBUIhreadData %p taken from private data for thread %p\n", d->data, this);
    d->data->thread.storeRelaxed(this);
}

/*!
    Destroys the BOBUIhread.

    Note that deleting a BOBUIhread object will not stop the execution
    of the thread it manages. Deleting a running BOBUIhread (i.e.
    isFinished() returns \c false) will result in a program
    crash. Wait for the finished() signal before deleting the
    BOBUIhread.

    Since BobUI 6.3, it is allowed to delete a BOBUIhread instance created by
    a call to BOBUIhread::create() even if the corresponding thread is
    still running. In such a case, BobUI will post an interruption request
    to that thread (via requestInterruption()); will ask the thread's
    event loop (if any) to quit (via quit()); and will block until the
    thread has finished.

    \sa create(), isInterruptionRequested(), exec(), quit()
*/
BOBUIhread::~BOBUIhread()
{
    Q_D(BOBUIhread);
    {
        QMutexLocker locker(&d->mutex);
        if (d->threadState == BOBUIhreadPrivate::Finishing)
            d->wait(locker, QDeadlineTimer::Forever);
        if (d->threadState == BOBUIhreadPrivate::Running && !d->data->isAdopted)
            qFatal("BOBUIhread: Destroyed while thread '%ls' is still running", qUtf16Printable(objectName()));

        d->data->thread.storeRelease(nullptr);
    }
}

/*!
    \threadsafe
    Returns \c true if the thread is finished; otherwise returns \c false.

    A thread is considered finished if it has returned from the run() function
    and the finished() signal has been emitted.

//! [execution-after-finished]
    Note the thread may still run for arbitrary amount of time after the
    finished() signal is emitted, running clean-up operations such as executing
    the destructors to \c{thread_local} variables. To synchronize with all
    effects from the thread, call wait() and verify it returned true.
//! [execution-after-finished]

    \sa isRunning()
*/
bool BOBUIhread::isFinished() const
{
    Q_D(const BOBUIhread);
    QMutexLocker locker(&d->mutex);
    return d->threadState >= BOBUIhreadPrivate::Finishing;
}

/*!
    \threadsafe
    Returns \c true if the thread is running; otherwise returns \c false.

    A thread is considered to be running if BOBUIhread has been started with
    start() but is not yet finished.

    \include bobuihread.cpp execution-after-finished

    \sa isFinished()
*/
bool BOBUIhread::isRunning() const
{
    Q_D(const BOBUIhread);
    QMutexLocker locker(&d->mutex);
    return d->threadState == BOBUIhreadPrivate::Running;
}

/*!
    Sets the stack size for the thread to \a stackSize. If \a stackSize is
    zero, the operating system or runtime will choose a default value.
    Otherwise, the thread's stack size will be the value provided (which may be
    rounded up or down).

    On most operating systems, the amount of memory allocated to serve the
    stack will initially be smaller than \a stackSize and will grow as the
    thread uses the stack. This parameter sets the maximum size it will be
    allowed to grow to (that is, it sets the size of the virtual memory space
    the stack is allowed to occupy).

    This function can only be called before the thread is started.

    \warning Most operating systems place minimum and maximum limits
    on thread stack sizes. The thread will fail to start if the stack
    size is outside these limits.

    \sa stackSize()
*/
void BOBUIhread::setStackSize(uint stackSize)
{
    Q_D(BOBUIhread);
    Q_ASSERT_X(!isRunning(), "BOBUIhread::setStackSize",
               "cannot change stack size while the thread is running");
    QMutexLocker locker(&d->mutex);
    d->stackSize = stackSize;
}

/*!
    Returns the maximum stack size for the thread (if set with
    setStackSize()); otherwise returns zero.

    \sa setStackSize()
*/
uint BOBUIhread::stackSize() const
{
    Q_D(const BOBUIhread);
    QMutexLocker locker(&d->mutex);
    return d->stackSize;
}

/*!
    \enum BOBUIhread::QualityOfService
    \since 6.9

    This enum describes the quality of service level of a thread, and provides
    the scheduler with information about the kind of work that the thread
    performs. On platforms with different CPU profiles, or with the ability to
    clock certain cores of a CPU down, this allows the scheduler to select or
    configure a CPU core with suitable performance and energy characteristics
    for the thread.

    \value Auto The default value, leaving it to the scheduler to decide which
                CPU core to run the thread on.
    \value High The scheduler should run this thread to a high-performance CPU
                core.
    \value Eco  The scheduler should run this thread to an energy-efficient CPU
                core.

    \sa Priority, serviceLevel(), BOBUIhreadPool::serviceLevel()
*/

/*!
    \since 6.9

    Set the Quality of Service level of the thread object to \a serviceLevel.
    This can only be called from the thread itself or before the thread is
    started!

    This is currently only implemented on Apple platforms, and Windows.
    The function call will complete successfully on other platforms but will
    not currently have any effect.

    \sa serviceLevel(), BOBUIhreadPool::setServiceLevel()
*/
void BOBUIhread::setServiceLevel(QualityOfService serviceLevel)
{
    Q_D(BOBUIhread);
    QMutexLocker locker(&d->mutex);
    if (d->threadState != BOBUIhreadPrivate::Running) {
        d->serviceLevel = serviceLevel;
    } else {
        Q_ASSERT_X(isCurrentThread(), "BOBUIhread::setServiceLevel",
                "cannot change quality of service level of a separate, running, thread");
        d->setQualityOfServiceLevel(serviceLevel);
    }
}

/*!
    \since 6.9

    Return the current Quality of Service level of the thread.

    \sa setServiceLevel(), BOBUIhreadPool::serviceLevel()
*/
BOBUIhread::QualityOfService BOBUIhread::serviceLevel() const
{
    Q_D(const BOBUIhread);
    QMutexLocker locker(&d->mutex);
    return d->serviceLevel;
}

#endif

/*!
    \internal
    Transitions BindingStatusOrList to the binding status state. If we had a list of
    pending objects, all objects get their reinitBindingStorageAfterThreadMove method
    called, and afterwards, the list gets discarded.
 */
void BobUIPrivate::BindingStatusOrList::setStatusAndClearList(QBindingStatus *status) noexcept
{
    if (auto pendingObjects = list()) {
        for (auto obj: *pendingObjects)
            QObjectPrivate::get(obj)->reinitBindingStorageAfterThreadMove();
        delete pendingObjects;
    }
    // synchronizes-with the load-acquire in bindingStatus():
    data.store(encodeBindingStatus(status), std::memory_order_release);
}

#if BOBUI_CONFIG(thread)

/*!
    Enters the event loop and waits until exit() is called, returning the value
    that was passed to exit(). The value returned is 0 if exit() is called via
    quit().

    This function is meant to be called from within run(). It is necessary to
    call this function to start event handling.

    \note This can only be called within the thread itself, i.e. when
    it is the current thread.

    \sa quit(), exit()
*/
int BOBUIhread::exec()
{
    Q_D(BOBUIhread);
    const auto status = BobUIPrivate::getBindingStatus(BobUIPrivate::QBindingStatusAccessToken{});

    QMutexLocker locker(&d->mutex);
    d->data->m_statusOrPendingObjects.setStatusAndClearList(status);
    d->data->quitNow = false;
    if (d->exited) {
        d->exited = false;
        return d->returnCode;
    }
    locker.unlock();

    QEventLoop eventLoop;
    int returnCode = eventLoop.exec();

    locker.relock();
    d->exited = false;
    d->returnCode = -1;
    return returnCode;
}


/*!
    \internal
    If BindingStatusOrList is already in the binding status state, this will
    return that BindingStatus pointer.
    Otherwise, \a object is added to the list, and we return nullptr.
    The list is allocated if it does not already exist.
 */
QBindingStatus *BobUIPrivate::BindingStatusOrList::addObjectUnlessAlreadyStatus(QObject *object)
{
    if (auto status = bindingStatus())
        return status;
    List *objectList = list();
    if (!objectList) {
        objectList = new List();
        objectList->reserve(8);
        data.store(encodeList(objectList), std::memory_order_relaxed);
    }
    objectList->push_back(object);
    return nullptr;
}

/*!
    \internal
    If BindingStatusOrList is a list, remove \a object from it
 */
void BobUIPrivate::BindingStatusOrList::removeObject(QObject *object)
{
    List *objectList = list();
    if (!objectList)
        return;
    auto it = std::remove(objectList->begin(), objectList->end(), object);
    objectList->erase(it, objectList->end());
}

QBindingStatus *BOBUIhreadPrivate::addObjectWithPendingBindingStatusChange(QObject *obj)
{
    if (auto status = data->m_statusOrPendingObjects.bindingStatus())
        return status;
    QMutexLocker lock(&mutex);
    return data->m_statusOrPendingObjects.addObjectUnlessAlreadyStatus(obj);
}

void BOBUIhreadPrivate::removeObjectWithPendingBindingStatusChange(QObject *obj)
{
    if (data->m_statusOrPendingObjects.bindingStatus())
        return;
    QMutexLocker lock(&mutex);
    data->m_statusOrPendingObjects.removeObject(obj);
}


/*!
    \threadsafe
    Tells the thread's event loop to exit with a return code.

    After calling this function, the thread leaves the event loop and
    returns from the call to QEventLoop::exec(). The
    QEventLoop::exec() function returns \a returnCode.

    By convention, a \a returnCode of 0 means success, any non-zero value
    indicates an error.

    Note that unlike the C library function of the same name, this
    function \e does return to the caller -- it is event processing
    that stops.

    No QEventLoops will be started anymore in this thread  until
    BOBUIhread::exec() has been called again. If the eventloop in BOBUIhread::exec()
    is not running then the next call to BOBUIhread::exec() will also return
    immediately.

    \sa quit(), QEventLoop
*/
void BOBUIhread::exit(int returnCode)
{
    Q_D(BOBUIhread);
    QMutexLocker locker(&d->mutex);
    d->exited = true;
    d->returnCode = returnCode;
    d->data->quitNow = true;
    for (int i = 0; i < d->data->eventLoops.size(); ++i) {
        QEventLoop *eventLoop = d->data->eventLoops.at(i);
        eventLoop->exit(returnCode);
    }
}

/*!
    \threadsafe
    Tells the thread's event loop to exit with return code 0 (success).
    Equivalent to calling BOBUIhread::exit(0).

    This function does nothing if the thread does not have an event
    loop.

    \sa exit(), QEventLoop
*/
void BOBUIhread::quit()
{ exit(); }

/*!
    The starting point for the thread. After calling start(), the
    newly created thread calls this function. The default
    implementation simply calls exec().

    You can reimplement this function to facilitate advanced thread
    management. Returning from this method will end the execution of
    the thread.

    \sa start(), wait()
*/
void BOBUIhread::run()
{
    (void) exec();
}

/*! \fn void BOBUIhread::setPriority(Priority priority)
    \since 4.1

    This function sets the \a priority for a running thread. If the
    thread is not running, this function does nothing and returns
    immediately.  Use start() to start a thread with a specific
    priority.

    The \a priority argument can be any value in the \c
    BOBUIhread::Priority enum except for \c InheritPriority.

    The effect of the \a priority parameter is dependent on the
    operating system's scheduling policy. In particular, the \a priority
    will be ignored on systems that do not support thread priorities
    (such as on Linux, see http://linux.die.net/man/2/sched_setscheduler
    for more details).

    \sa Priority, priority(), start()
*/
void BOBUIhread::setPriority(Priority priority)
{
    if (priority == BOBUIhread::InheritPriority) {
        qWarning("BOBUIhread::setPriority: Argument cannot be InheritPriority");
        return;
    }
    Q_D(BOBUIhread);
    QMutexLocker locker(&d->mutex);
    if (d->threadState != BOBUIhreadPrivate::Running) {
        qWarning("BOBUIhread::setPriority: Cannot set priority, thread is not running");
        return;
    }
    d->setPriority(priority);
}

/*!
    \since 4.1

    Returns the priority for a running thread.  If the thread is not
    running, this function returns \c InheritPriority.

    \sa Priority, setPriority(), start()
*/
BOBUIhread::Priority BOBUIhread::priority() const
{
    Q_D(const BOBUIhread);
    QMutexLocker locker(&d->mutex);

    // mask off the high bits that are used for flags
    return Priority(d->priority & 0xffff);
}

/*!
    \fn void BOBUIhread::sleep(std::chrono::nanoseconds nsecs)
    \since 6.6

    Forces the current thread to sleep for \a nsecs.

    Avoid using this function if you need to wait for a given condition to
    change. Instead, connect a slot to the signal that indicates the change or
    use an event handler (see \l QObject::event()).

    \note This function does not guarantee accuracy. The application may sleep
    longer than \a nsecs under heavy load conditions.
*/

/*!
    \fn void BOBUIhread::sleep(unsigned long secs)

    Forces the current thread to sleep for \a secs seconds.

    This is an overloaded function, equivalent to calling:
    \code
    BOBUIhread::sleep(std::chrono::seconds{secs});
    \endcode

    \sa msleep(), usleep()
*/

/*!
    \fn void BOBUIhread::msleep(unsigned long msecs)

    This is an overloaded function, equivalent to calling:
    \code
    BOBUIhread::sleep(std::chrono::milliseconds{msecs});
    \endcode

    \note This function does not guarantee accuracy. The application may sleep
    longer than \a msecs under heavy load conditions. Some OSes might round \a
    msecs up to 10 ms or 15 ms.

    \sa sleep(), usleep()
*/

/*!
    \fn void BOBUIhread::usleep(unsigned long usecs)

    This is an overloaded function, equivalent to calling:
    \code
    BOBUIhread::sleep(std::chrono::microseconds{secs});
    \endcode

    \note This function does not guarantee accuracy. The application may sleep
    longer than \a usecs under heavy load conditions. Some OSes might round \a
    usecs up to 10 ms or 15 ms; on Windows, it will be rounded up to a multiple
    of 1 ms.

    \sa sleep(), msleep()
*/

/*!
    \fn void BOBUIhread::terminate()
    \threadsafe

    Terminates the execution of the thread. The thread may or may not
    be terminated immediately, depending on the operating system's
    scheduling policies. Use BOBUIhread::wait() after terminate(), to be
    sure.

    When the thread is terminated, all threads waiting for the thread
    to finish will be woken up.

    \warning This function is dangerous and its use is discouraged.
    The thread can be terminated at any point in its code path.
    Threads can be terminated while modifying data. There is no
    chance for the thread to clean up after itself, unlock any held
    mutexes, etc. In short, use this function only if absolutely
    necessary.

    Termination can be explicitly enabled or disabled by calling
    BOBUIhread::setTerminationEnabled(). Calling this function while
    termination is disabled results in the termination being
    deferred, until termination is re-enabled. See the documentation
    of BOBUIhread::setTerminationEnabled() for more information.

    \sa setTerminationEnabled()
*/

/*!
    \fn bool BOBUIhread::wait(QDeadlineTimer deadline)
    \since 5.15

    Blocks the thread until either of these conditions is met:

    \list
    \li The thread associated with this BOBUIhread object has finished
       execution (i.e. when it returns from \l{run()}). This function
       will return true if the thread has finished. It also returns
       true if the thread has not been started yet.
    \li The \a deadline is reached. This function will return false if the
       deadline is reached.
    \endlist

    A deadline timer set to \c QDeadlineTimer::Forever (the default) will never
    time out: in this case, the function only returns when the thread returns
    from \l{run()} or if the thread has not yet started.

    This provides similar functionality to the POSIX \c
    pthread_join() function.

    \note On some operating systems, this function may return true while the
    operating system thread is still running and may be executing clean-up code
    such as C++11 \c{thread_local} destructors. Operating systems where this
    function only returns true after the OS thread has fully exited include
    Linux, Windows, and Apple operating systems.

    \sa sleep(), terminate()
*/
bool BOBUIhread::wait(QDeadlineTimer deadline)
{
    Q_D(BOBUIhread);
    QMutexLocker locker(&d->mutex);

    if (d->threadState == BOBUIhreadPrivate::NotStarted || d->threadState == BOBUIhreadPrivate::Finished)
        return true;
    if (isCurrentThread()) {
        qWarning("BOBUIhread::wait: Thread tried to wait on itself");
        return false;
    }
    return d->wait(locker, deadline);
}

/*!
    \fn void BOBUIhread::setTerminationEnabled(bool enabled)

    Enables or disables termination of the current thread based on the
    \a enabled parameter. The thread must have been started by
    BOBUIhread.

    When \a enabled is false, termination is disabled.  Future calls
    to BOBUIhread::terminate() will return immediately without effect.
    Instead, the termination is deferred until termination is enabled.

    When \a enabled is true, termination is enabled.  Future calls to
    BOBUIhread::terminate() will terminate the thread normally.  If
    termination has been deferred (i.e. BOBUIhread::terminate() was
    called with termination disabled), this function will terminate
    the calling thread \e immediately.  Note that this function will
    not return in this case.

    \sa terminate()
*/

/*!
    \since 5.5
    Returns the current event loop level for the thread.

    \note This can only be called within the thread itself, i.e. when
    it is the current thread.
*/

int BOBUIhread::loopLevel() const
{
    Q_D(const BOBUIhread);
    return d->data->eventLoops.size();
}

/*!
    \internal
    Returns the thread handle of this thread.
    It can be compared with the return value of currentThreadId().

    This is used to implement isCurrentThread, and might be useful
    for debugging (e.g. by comparing the value in gdb with info threads).

    \note Thread handles of destroyed threads might be reused by the
    operating system. Storing the return value of this function can
    therefore give surprising results if it outlives the BOBUIhread object
    (threads claimed to be the same even if they aren't).
*/
BobUI::HANDLE BOBUIhreadPrivate::threadId() const noexcept
{
    return data->threadId.loadRelaxed();
}

/*!
    \since 6.8
    Returns true if this thread is BOBUIhread::currentThread.

    \sa currentThreadId()
*/
bool BOBUIhread::isCurrentThread() const noexcept
{
    Q_D(const BOBUIhread);
    return BOBUIhread::currentThreadId() == d->threadId();
}

#else // BOBUI_CONFIG(thread)

BOBUIhread::BOBUIhread(QObject *parent)
    : QObject(*(new BOBUIhreadPrivate), parent)
{
    Q_D(BOBUIhread);
    d->data->thread.storeRelaxed(this);
}

BOBUIhread::~BOBUIhread()
{

}

BOBUIhread *BOBUIhread::createThreadImpl(std::future<void>&&)
{
    return nullptr;
}

void BOBUIhread::run()
{

}

int BOBUIhread::exec()
{
    return 0;
}

void BOBUIhread::start(Priority priority)
{
    Q_D(BOBUIhread);
    Q_UNUSED(priority);
    d->running = true;
}

void BOBUIhread::terminate()
{

}

void BOBUIhread::quit()
{

}

void BOBUIhread::exit(int returnCode)
{
    Q_D(BOBUIhread);
    d->data->quitNow = true;
    for (int i = 0; i < d->data->eventLoops.size(); ++i) {
        QEventLoop *eventLoop = d->data->eventLoops.at(i);
        eventLoop->exit(returnCode);
    }
}

bool BOBUIhread::wait(QDeadlineTimer deadline)
{
    Q_UNUSED(deadline);
    return false;
}

bool BOBUIhread::event(QEvent *event)
{
    return QObject::event(event);
}

BobUI::HANDLE BOBUIhread::currentThreadIdImpl() noexcept
{
    return BobUI::HANDLE(currentThread());
}

BOBUIhread *BOBUIhread::currentThread()
{
    return BOBUIhreadData::current()->thread.loadAcquire();
}

bool BOBUIhread::isMainThread() noexcept
{
    return true;
}

bool BOBUIhread::isCurrentThread() const noexcept
{
    return true;
}

int BOBUIhread::idealThreadCount() noexcept
{
    return 1;
}

void BOBUIhread::yieldCurrentThread()
{

}

bool BOBUIhread::isFinished() const
{
    return false;
}

bool BOBUIhread::isRunning() const
{
    Q_D(const BOBUIhread);
    return d->running;
}

void BOBUIhread::requestInterruption()
{

}

bool BOBUIhread::isInterruptionRequested() const
{
    return false;
}

void BOBUIhread::setTerminationEnabled(bool)
{
}

void BOBUIhread::setServiceLevel(QualityOfService)
{
}

// No threads: so we can just use static variables
Q_CONSTINIT static BOBUIhreadData *data = nullptr;

BOBUIhreadData *BOBUIhreadData::currentThreadData() noexcept
{
    return data;
}

BOBUIhreadData *BOBUIhreadData::createCurrentThreadData()
{
    Q_ASSERT(!currentThreadData());
    data = new BOBUIhreadData;
    data->thread = new QAdoptedThread(data);
    return data;
}

void BOBUIhreadData::clearCurrentThreadData()
{
    delete data;
    data = 0;
}

/*!
    \internal
 */
BOBUIhread::BOBUIhread(BOBUIhreadPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
    Q_D(BOBUIhread);
    // fprintf(stderr, "BOBUIhreadData %p taken from private data for thread %p\n", d->data, this);
    d->data->thread.storeRelaxed(this);
}

BOBUIhreadPrivate::BOBUIhreadPrivate(BOBUIhreadData *d) : data(d ? d : new BOBUIhreadData)
{
}

BOBUIhreadPrivate::~BOBUIhreadPrivate()
{
    data->thread.storeRelease(nullptr); // prevent BOBUIhreadData from deleting the BOBUIhreadPrivate (again).
    delete data;
}

void BOBUIhread::setStackSize(uint stackSize)
{
    Q_UNUSED(stackSize);
}

uint BOBUIhread::stackSize() const
{
    return 0;
}

#endif // BOBUI_CONFIG(thread)

/*!
    \since 5.0

    Returns a pointer to the event dispatcher object for the thread. If no event
    dispatcher exists for the thread, this function returns \nullptr.
*/
QAbstractEventDispatcher *BOBUIhread::eventDispatcher() const
{
    Q_D(const BOBUIhread);
    return d->data->eventDispatcher.loadRelaxed();
}

/*!
    \since 5.0

    Sets the event dispatcher for the thread to \a eventDispatcher. This is
    only possible as long as there is no event dispatcher installed for the
    thread yet.

    An event dispatcher is automatically created for the main thread when \l
    QCoreApplication is instantiated and on start() for auxiliary threads.

    This method takes ownership of the object.
*/
void BOBUIhread::setEventDispatcher(QAbstractEventDispatcher *eventDispatcher)
{
    Q_D(BOBUIhread);
    if (d->data->hasEventDispatcher()) {
        qWarning("BOBUIhread::setEventDispatcher: An event dispatcher has already been created for this thread");
    } else {
        eventDispatcher->moveToThread(this);
        if (eventDispatcher->thread() == this) // was the move successful?
            d->data->eventDispatcher.storeRelaxed(eventDispatcher);
        else
            qWarning("BOBUIhread::setEventDispatcher: Could not move event dispatcher to target thread");
    }
}

/*!
    \fn bool BOBUIhread::wait(unsigned long time)

    \overload
    \a time is the time to wait in milliseconds.
    If \a time is ULONG_MAX, then the wait will never timeout.
*/

#if BOBUI_CONFIG(thread)

/*!
    \reimp
*/
bool BOBUIhread::event(QEvent *event)
{
    if (event->type() == QEvent::Quit) {
        quit();
        return true;
    } else {
        return QObject::event(event);
    }
}

/*!
    \since 5.2
    \threadsafe

    Request the interruption of the thread.
    That request is advisory and it is up to code running on the thread to decide
    if and how it should act upon such request.
    This function does not stop any event loop running on the thread and
    does not terminate it in any way.

    This function has no effect on the main thread, and does nothing if the thread
    is not currently running.

    \sa isInterruptionRequested()
*/

void BOBUIhread::requestInterruption()
{
    Q_D(BOBUIhread);
    if (d->threadId() == QCoreApplicationPrivate::theMainThreadId.loadAcquire()) {
        qWarning("BOBUIhread::requestInterruption has no effect on the main thread");
        return;
    }
    QMutexLocker locker(&d->mutex);
    if (d->threadState != BOBUIhreadPrivate::Running)
        return;
    d->interruptionRequested.store(true, std::memory_order_relaxed);
}

/*!
    \since 5.2

    Return true if the task running on this thread should be stopped.
    An interruption can be requested by requestInterruption().

    This function can be used to make long running tasks cleanly interruptible.
    Never checking or acting on the value returned by this function is safe,
    however it is advisable do so regularly in long running functions.
    Take care not to call it too often, to keep the overhead low.

    \code
    void long_task() {
         forever {
            if ( BOBUIhread::currentThread()->isInterruptionRequested() ) {
                return;
            }
        }
    }
    \endcode

    \note This can only be called within the thread itself, i.e. when
    it is the current thread.

    \sa currentThread() requestInterruption()
*/
bool BOBUIhread::isInterruptionRequested() const
{
    Q_D(const BOBUIhread);
    // fast path: check that the flag is not set:
    if (!d->interruptionRequested.load(std::memory_order_relaxed))
        return false;
    // slow path: if the flag is set, take into account run status:
    QMutexLocker locker(&d->mutex);
    return d->threadState == BOBUIhreadPrivate::Running;
}

/*!
    \fn template <typename Function, typename... Args> BOBUIhread *BOBUIhread::create(Function &&f, Args &&... args)
    \since 5.10

    Creates a new BOBUIhread object that will execute the function \a f with the
    arguments \a args.

    The new thread is not started -- it must be started by an explicit call
    to start(). This allows you to connect to its signals, move QObjects
    to the thread, choose the new thread's priority and so on. The function
    \a f will be called in the new thread.

    Returns the newly created BOBUIhread instance.

    \note the caller acquires ownership of the returned BOBUIhread instance.

    \warning do not call start() on the returned BOBUIhread instance more than once;
    doing so will result in undefined behavior.

    \sa start()
*/

class BOBUIhreadCreateThread : public BOBUIhread
{
public:
    explicit BOBUIhreadCreateThread(std::future<void> &&future)
        : m_future(std::move(future))
    {
    }

    ~BOBUIhreadCreateThread()
    {
        requestInterruption();
        quit();
        wait();
    }

private:
    void run() override
    {
        m_future.get();
    }

    std::future<void> m_future;
};

BOBUIhread *BOBUIhread::createThreadImpl(std::future<void> &&future)
{
    return new BOBUIhreadCreateThread(std::move(future));
}

/*!
    \class QDaemonThread
    \since 5.5
    \brief The QDaemonThread provides a class to manage threads that outlive QCoreApplication
    \internal

    Note: don't try to deliver events from the started() signal.
*/
QDaemonThread::QDaemonThread(QObject *parent)
    : BOBUIhread(parent)
{
    // BOBUIhread::started() is emitted from the thread we start
    connect(this, &BOBUIhread::started,
            this,
            [](){ BOBUIhreadData::current()->requiresCoreApplication = false; },
            BobUI::DirectConnection);
}

QDaemonThread::~QDaemonThread()
{
}

#endif // BOBUI_CONFIG(thread)

BOBUI_END_NAMESPACE

#include "moc_bobuihread.cpp"
