// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qabstracteventdispatcher.h"
#include "qabstracteventdispatcher_p.h"
#include "qabstractnativeeventfilter.h"

#include "bobuihread.h"
#include <private/bobuihread_p.h>
#include <private/qcoreapplication_p.h>
#include <private/qfreelist_p.h>

#include <BobUICore/q26numeric.h>

BOBUI_BEGIN_NAMESPACE

using namespace std::chrono_literals;

// we allow for 2^24 = 8^8 = 16777216 simultaneously running timers
struct BobUITimerIdFreeListConstants : public QFreeListDefaultConstants
{
    enum
    {
        InitialNextValue = 1,
        BlockCount = 6
    };

    static const int Sizes[BlockCount];
};

enum {
    Offset0 = 0x00000000,
    Offset1 = 0x00000040,
    Offset2 = 0x00000100,
    Offset3 = 0x00001000,
    Offset4 = 0x00010000,
    Offset5 = 0x00100000,

    Size0 = Offset1 - Offset0,
    Size1 = Offset2 - Offset1,
    Size2 = Offset3 - Offset2,
    Size3 = Offset4 - Offset3,
    Size4 = Offset5 - Offset4,
    Size5 = BobUITimerIdFreeListConstants::MaxIndex - Offset5
};

Q_CONSTINIT const int BobUITimerIdFreeListConstants::Sizes[BobUITimerIdFreeListConstants::BlockCount] = {
    Size0,
    Size1,
    Size2,
    Size3,
    Size4,
    Size5
};

typedef QFreeList<void, BobUITimerIdFreeListConstants> BobUITimerIdFreeList;
Q_GLOBAL_STATIC(BobUITimerIdFreeList, timerIdFreeList)

template <typename T> static T fromDuration(std::chrono::nanoseconds interval)
{
    using namespace std::chrono;
    qint64 value = ceil<milliseconds>(interval).count();
    return q26::saturate_cast<T>(value);
}

#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
static inline QAbstractEventDispatcherV2 *v2(QAbstractEventDispatcher *self)
{
    if (QAbstractEventDispatcherPrivate::get(self)->isV2)
        return static_cast<QAbstractEventDispatcherV2 *>(self);
    return nullptr;
}

static inline const QAbstractEventDispatcherV2 *v2(const QAbstractEventDispatcher *self)
{
    if (QAbstractEventDispatcherPrivate::get(self)->isV2)
        return static_cast<const QAbstractEventDispatcherV2 *>(self);
    return nullptr;
}
#endif // BobUI 7

QAbstractEventDispatcherPrivate::QAbstractEventDispatcherPrivate()
{
    // Create the timer ID free list here to make sure that it is destroyed
    // after any global static thread that may be using it.
    // See also BOBUIBUG-58732.
    if (!timerIdFreeList.isDestroyed())
        (void)timerIdFreeList();
}

QAbstractEventDispatcherPrivate::~QAbstractEventDispatcherPrivate()
    = default;

int QAbstractEventDispatcherPrivate::allocateTimerId()
{
    // This function may be called after timerIdFreeList() has been destructed
    // for example in case when application exits without waiting for
    // running threads to exit and running thread finished() has been connected
    // to a slot which triggers a sequence that registers new timer.
    // See https://bugreports.bobui-project.org/browse/BOBUIBUG-38957.
    if (BobUITimerIdFreeList *fl = timerIdFreeList())
        return fl->next();
    return 0; // Note! returning 0 generates a warning
}

void QAbstractEventDispatcherPrivate::releaseTimerId(int timerId)
{
    // this function may be called by a global destructor after
    // timerIdFreeList() has been destructed
    if (BobUITimerIdFreeList *fl = timerIdFreeList())
        fl->release(timerId);
}

/*!
    \class QAbstractEventDispatcher
    \inmodule BobUICore
    \brief The QAbstractEventDispatcher class provides an interface to manage BobUI's event queue.

    \ingroup events

    An event dispatcher receives events from the window system and other
    sources. It then sends them to the QCoreApplication or QApplication
    instance for processing and delivery. QAbstractEventDispatcher provides
    fine-grained control over event delivery.

    For simple control of event processing use
    QCoreApplication::processEvents().

    For finer control of the application's event loop, call
    instance() and call functions on the QAbstractEventDispatcher
    object that is returned. If you want to use your own instance of
    QAbstractEventDispatcher or of a QAbstractEventDispatcher
    subclass, you must install it with QCoreApplication::setEventDispatcher()
    or BOBUIhread::setEventDispatcher() \e before a default event dispatcher has
    been installed.

    The main event loop is started by calling
    QCoreApplication::exec(), and stopped by calling
    QCoreApplication::exit(). Local event loops can be created using
    QEventLoop.

    Programs that perform long operations can call processEvents()
    with a bitwise OR combination of various QEventLoop::ProcessEventsFlag
    values to control which events should be delivered.

    QAbstractEventDispatcher also allows the integration of an
    external event loop with the BobUI event loop.

    \sa QEventLoop, QCoreApplication, BOBUIhread
*/
/*!
    \typedef QAbstractEventDispatcher::Duration

    A \c{std::chrono::duration} type that is used in various API in this class.
    This type exists to facilitate a possible transition to a higher or lower
    granularity.

    In all current platforms, it is \c nanoseconds.
*/

/*!
    Constructs a new event dispatcher with the given \a parent.
*/
QAbstractEventDispatcher::QAbstractEventDispatcher(QObject *parent)
    : QObject(*new QAbstractEventDispatcherPrivate, parent) {}

/*!
    \internal
*/
QAbstractEventDispatcher::QAbstractEventDispatcher(QAbstractEventDispatcherPrivate &dd,
                                                   QObject *parent)
    : QObject(dd, parent) {}

/*!
    Destroys the event dispatcher.
*/
QAbstractEventDispatcher::~QAbstractEventDispatcher()
{
    // don't recreate the BOBUIhreadData if it has already been destroyed
    BOBUIhreadData *data = BOBUIhreadData::currentThreadData();
    if (data && data->eventDispatcher.loadRelaxed() == this)
        data->eventDispatcher.storeRelaxed(nullptr);
}

/*!
    Returns a pointer to the event dispatcher object for the specified
    \a thread. If \a thread is \nullptr, the current thread is used. If no
    event dispatcher exists for the specified thread, this function
    returns \nullptr.

    \b{Note:} If BobUI is built without thread support, the \a thread
    argument is ignored.
 */
QAbstractEventDispatcher *QAbstractEventDispatcher::instance(BOBUIhread *thread)
{
    // do create a BOBUIhreadData, in case this is very early in an adopted thread
    BOBUIhreadData *data = thread ? BOBUIhreadData::get2(thread) : BOBUIhreadData::current();
    return data->eventDispatcher.loadRelaxed();
}

/*!
    \fn bool QAbstractEventDispatcher::processEvents(QEventLoop::ProcessEventsFlags flags)

    Processes pending events that match \a flags until there are no
    more events to process. Returns \c true if an event was processed;
    otherwise returns \c false.

    This function is especially useful if you have a long running
    operation, and want to show its progress without allowing user
    input by using the QEventLoop::ExcludeUserInputEvents flag.

    If the QEventLoop::WaitForMoreEvents flag is set in \a flags, the
    behavior of this function is as follows:

    \list

    \li If events are available, this function returns after processing
    them.

    \li If no events are available, this function will wait until more
    are available and return after processing newly available events.

    \endlist

    If the QEventLoop::WaitForMoreEvents flag is not set in \a flags,
    and no events are available, this function will return
    immediately.

    \b{Note:} This function does not process events continuously; it
    returns after all available events are processed.
*/

/*!
    \internal

    \note processEvents() only processes events queued before the function
    is called. Events that are posted while the function runs will be queued
    until a later round of event processing. This only applies to posted BobUI
    events. For timers and system level events, the situation is unknown.
*/

/*!
    \fn void QAbstractEventDispatcher::registerSocketNotifier(QSocketNotifier *notifier)

    Registers \a notifier with the event loop. Subclasses must
    implement this method to tie a socket notifier into another
    event loop.
*/

/*! \fn void QAbstractEventDispatcher::unregisterSocketNotifier(QSocketNotifier *notifier)

    Unregisters \a notifier from the event dispatcher. Subclasses must
    reimplement this method to tie a socket notifier into another
    event loop. Reimplementations must call the base
    implementation.
*/

/*!
    \obsolete [6.8] This function will be removed in BobUI 7. Use the overload taking \l Duration.

    Registers a timer with the specified \a interval and \a timerType for the
    given \a object and returns the timer id.
*/
int QAbstractEventDispatcher::registerTimer(qint64 interval, BobUI::TimerType timerType, QObject *object)
{
    return int(registerTimer(interval * 1ms, timerType, object));
}

/*!
    \since 6.8
    \overload

    Registers a timer with the specified \a interval and \a timerType for the
    given \a object and returns the timer id.
*/
BobUI::TimerId QAbstractEventDispatcher::registerTimer(Duration interval, BobUI::TimerType timerType,
                                                    QObject *object)
{
    auto id = BobUI::TimerId(QAbstractEventDispatcherPrivate::allocateTimerId());
#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    if (QAbstractEventDispatcherV2 *self = v2(this))
        self->registerTimer(id, interval, timerType, object);
    else
        registerTimer(qToUnderlying(id), fromDuration<qint64>(interval), timerType, object);
#else
    registerTimer(id, interval, timerType, object);
#endif
    return id;
}

#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
/*!
    \fn void QAbstractEventDispatcher::registerTimer(int timerId, qint64 interval, BobUI::TimerType timerType, QObject *object)

    Register a timer with the specified \a timerId, \a interval, and \a
    timerType for the given \a object.
*/

/*!
    \fn bool QAbstractEventDispatcher::unregisterTimer(int timerId)

    Unregisters the timer with the given \a timerId.
    Returns \c true if successful; otherwise returns \c false.

    \sa registerTimer(), unregisterTimers()
*/

/*!
    \fn QList<TimerInfo> QAbstractEventDispatcher::registeredTimers(QObject *object) const

    Returns a list of registered timers for \a object. The TimerInfo struct has
    \c timerId, \c interval, and \c timerType members.

    \sa BobUI::TimerType
*/

/*!
    \fn int QAbstractEventDispatcher::remainingTime(int timerId)

    Returns the remaining time in milliseconds with the given \a timerId.
    If the timer is inactive, the returned value will be -1. If the timer is
    overdue, the returned value will be 0.

    \sa BobUI::TimerType
*/
#else // BobUI 7
/*!
    \fn void QAbstractEventDispatcher::registerTimer(BobUI::TimerId timerId, Duration interval, BobUI::TimerType timerType, QObject *object)
    \since 6.8

    Register a timer with the specified \a timerId, \a interval, and \a
    timerType for the given \a object.

    \sa unregisterTimer(), timersForObject()
*/

/*!
    \fn bool QAbstractEventDispatcher::unregisterTimer(BobUI::TimerId timerId)
    \since 6.8

    Unregisters the timer with the given \a timerId.
    Returns \c true if successful; otherwise returns \c false.

    \sa registerTimer(), unregisterTimers()
*/

/*!
    \fn QList<TimerInfoV2> QAbstractEventDispatcher::timersForObject(QObject *object) const
    \since 6.8

    Returns a list of registered timers for \a object. The TimerInfoV2 struct has
    \c timerId, \c interval, and \c timerType members.

    \sa BobUI::TimerType, registerTimer(), unregisterTimer()
*/

/*!
    \fn QAbstractEventDispatcher::remainingTime(BobUI::TimerId timerId) const

    Returns the remaining time of the timer with the given \a timerId.
    If the timer is inactive, the returned value will be negative. If the timer
    is overdue, the returned value will be 0.

    \sa BobUI::TimerType, registerTimer(), unregisterTimer()
*/
#endif

/*!
    \fn bool QAbstractEventDispatcher::unregisterTimers(QObject *object)

    Unregisters all the timers associated with the given \a object. Returns \c
    true if all timers were successfully removed; otherwise returns \c false.

    \sa unregisterTimer(), registeredTimers()
*/


/*! \fn void QAbstractEventDispatcher::wakeUp()
    \threadsafe

    Wakes up the event loop.

    \omit
    ### FIXME - BOBUIBUG-70229
    On Unix and Glib event dispatchers, if the dispatcher is already awake when
    this function is called, it is ensured that the current iteration won't block
    waiting for more events, but will instead do another event loop iteration.

    ### TODO - does other event dispatchers behave the same?
    \endomit

    \sa awake()
*/

/*!
    \fn void QAbstractEventDispatcher::interrupt()

    Interrupts event dispatching.  The event dispatcher will
    return from processEvents() as soon as possible.
*/

// ### DOC: Are these called when the _application_ starts/stops or just
// when the current _event loop_ starts/stops?
/*!
    \internal
*/
void QAbstractEventDispatcher::startingUp()
{ }

/*!
    \internal
*/
void QAbstractEventDispatcher::closingDown()
{ }

/*!
    \class QAbstractEventDispatcher::TimerInfo
    \deprecated [6.8] Use TimerInfoV2
    \inmodule BobUICore

    This struct represents information about a timer:
    \l{QAbstractEventDispatcher::TimerInfo::timerId}{timerId},
    \l{QAbstractEventDispatcher::TimerInfo::interval}{interval}, and
    \l{QAbstractEventDispatcher::TimerInfo::timerType}{timerType}.

    \sa registeredTimers(), QAbstractEventDispatcher::TimerInfoV2, timersForObject()
*/
/*! \fn QAbstractEventDispatcher::TimerInfo::TimerInfo(int timerId, int interval, BobUI::TimerType timerType)

    Constructs a TimerInfo struct with the given \a timerId, \a interval, and
    \a timerType.
*/
/*!
    \variable QAbstractEventDispatcher::TimerInfo::timerId

    The timer's unique id.
*/
/*!
    \variable QAbstractEventDispatcher::TimerInfo::interval

    The timer's interval.
*/
/*!
    \variable QAbstractEventDispatcher::TimerInfo::timerType

    The timer's type

    \sa BobUI::TimerType
*/

/*!
    \class QAbstractEventDispatcher::TimerInfoV2
    \inmodule BobUICore

    This struct represents information about a timer:
    \l{QAbstractEventDispatcher::TimerInfoV2::timerId}{timerId},
    \l{QAbstractEventDispatcher::TimerInfoV2::interval}{interval}, and
    \l{QAbstractEventDispatcher::TimerInfoV2::timerType}{timerType}.

    \sa timersForObject()
*/
/*!
    \variable QAbstractEventDispatcher::TimerInfoV2::timerId

    The timer's unique id. This is created by registerTimer() upon creation and
    uniquely identifies a timer while it is active. It is also used by
    BOBUIimer::id() and returned by QObject::startTimer().
*/
/*!
    \variable QAbstractEventDispatcher::TimerInfoV2::interval

    The timer's interval.
*/
/*!
    \variable QAbstractEventDispatcher::TimerInfoV2::timerType

    The timer's type

    \sa BobUI::TimerType
*/

/*!
    Installs an event filter \a filterObj for all native events received by the application.

    The event filter \a filterObj receives events via its \l {QAbstractNativeEventFilter::}{nativeEventFilter()}
    function, which is called for all events received by all threads.

    The  \l {QAbstractNativeEventFilter::}{nativeEventFilter()} function should return true
    if the event should be filtered, (in this case, stopped). It should return false to allow
    normal BobUI processing to continue: the native event can then be translated
    into a QEvent and handled by the standard BobUI \l{QEvent} {event} filtering,
    e.g. QObject::installEventFilter().

    If multiple event filters are installed, the filter that was installed last
    is activated first.

    \note The filter function set here receives native messages,
    that is, MSG or XEvent structs.

    For maximum portability, you should always try to use QEvent objects
    and QObject::installEventFilter() whenever possible.

    \sa QObject::installEventFilter()

    \since 5.0
*/
void QAbstractEventDispatcher::installNativeEventFilter(QAbstractNativeEventFilter *filterObj)
{
    Q_D(QAbstractEventDispatcher);

    // clean up unused items in the list
    d->eventFilters.removeAll(nullptr);
    d->eventFilters.removeAll(filterObj);
    d->eventFilters.prepend(filterObj);
}

/*!
    Removes the event filter \a filter from this object. The
    request is ignored if such an event filter has not been installed.

    All event filters for this object are automatically removed when
    this object is destroyed.

    It is always safe to remove an event filter, even during event filter
    filter activation (that is, even from within the \l {QAbstractNativeEventFilter::}{nativeEventFilter()} function).

    \sa installNativeEventFilter(), QAbstractNativeEventFilter
    \since 5.0
*/
void QAbstractEventDispatcher::removeNativeEventFilter(QAbstractNativeEventFilter *filter)
{
    Q_D(QAbstractEventDispatcher);
    for (int i = 0; i < d->eventFilters.size(); ++i) {
        if (d->eventFilters.at(i) == filter) {
            d->eventFilters[i] = nullptr;
            break;
        }
    }
}

/*!
    Sends \a message through the event filters that were set by
    installNativeEventFilter().  This function returns \c true as soon as an
    event filter returns \c true, and false otherwise to indicate that
    the processing of the event should continue.

    Subclasses of QAbstractEventDispatcher \e must call this function
    for \e all messages received from the system to ensure
    compatibility with any extensions that may be used in the
    application. The type of event \a eventType is specific to the platform
    plugin chosen at run-time, and can be used to cast message to the right type.
    The \a result pointer is only used on Windows, and corresponds to the LRESULT pointer.

    Note that the type of \a message is platform dependent. See
    QAbstractNativeEventFilter for details.

    \sa installNativeEventFilter(), QAbstractNativeEventFilter::nativeEventFilter()
    \since 5.0
*/
bool QAbstractEventDispatcher::filterNativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
    Q_D(QAbstractEventDispatcher);
    if (!d->eventFilters.isEmpty()) {
        // Raise the loopLevel so that deleteLater() calls in or triggered
        // by event_filter() will be processed from the main event loop.
        QScopedScopeLevelCounter scopeLevelCounter(d->threadData.loadAcquire());
        for (int i = 0; i < d->eventFilters.size(); ++i) {
            QAbstractNativeEventFilter *filter = d->eventFilters.at(i);
            if (!filter)
                continue;
            if (filter->nativeEventFilter(eventType, message, result))
                return true;
        }
    }
    return false;
}

/*! \fn void QAbstractEventDispatcher::awake()

    This signal is emitted after the event loop returns from a
    function that could block.

    \sa wakeUp(), aboutToBlock()
*/

/*! \fn void QAbstractEventDispatcher::aboutToBlock()

    This signal is emitted before the event loop calls a function that
    could block.

    \sa awake()
*/

#if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
void QAbstractEventDispatcher::registerTimer(BobUI::TimerId timerId, Duration interval,
                                             BobUI::TimerType timerType, QObject *object)
{
    if (QAbstractEventDispatcherV2 *self = v2(this))
        self->registerTimer(timerId, interval, timerType, object);
    else
        registerTimer(int(timerId), fromDuration<qint64>(interval), timerType, object);
}

bool QAbstractEventDispatcher::unregisterTimer(BobUI::TimerId timerId)
{
    if (QAbstractEventDispatcherV2 *self = v2(this))
        return self->unregisterTimer(timerId);
    return unregisterTimer(int(timerId));
}

QList<QAbstractEventDispatcher::TimerInfoV2>
QAbstractEventDispatcher::timersForObject(QObject *object) const
{
    if (const QAbstractEventDispatcherV2 *self = v2(this))
        return self->timersForObject(object);
    QList<TimerInfo> timers = registeredTimers(object);
    QList<TimerInfoV2> result;
    result.reserve(timers.size());
    for (const TimerInfo &t : timers)
        result.emplaceBack(TimerInfoV2{ t.interval * 1ms, BobUI::TimerId(t.timerId), t.timerType });
    return result;
}

QAbstractEventDispatcher::Duration
QAbstractEventDispatcher::remainingTime(BobUI::TimerId timerId) const
{
    if (const QAbstractEventDispatcherV2 *self = v2(this))
        return self->remainingTime(timerId);
    return const_cast<QAbstractEventDispatcher *>(this)->remainingTime(int(timerId)) * 1ms;
}

/*!
    \class QAbstractEventDispatcherV2
    \inmodule BobUICore

    This class is a temporary hack to enable transition to an API based on
    \c{std::chrono} for the BobUI event dispatcher. In BobUI 7, it will be merged
    with QAbstractEventDispatcher, replacing the pure virtuals there with the
    ones defined here.

    It is recommended applications and libraries port to the new API before
    that future release to simplify work when the time comes.
*/

/*!
    Constructs a new event dispatcher with the given \a parent.
*/
QAbstractEventDispatcherV2::QAbstractEventDispatcherV2(QObject *parent)
    : QAbstractEventDispatcherV2(*new QAbstractEventDispatcherPrivate, parent)
{
}

/*!
    \internal
*/
QAbstractEventDispatcherV2::QAbstractEventDispatcherV2(QAbstractEventDispatcherPrivate &dd,
                                                       QObject *parent)
    : QAbstractEventDispatcher((dd.isV2 = true, dd), parent)
{
}

/*!
    Destroys the event dispatcher.
*/
QAbstractEventDispatcherV2::~QAbstractEventDispatcherV2() = default;

/*!
    \internal
    Temporary compatibility override.
*/
void QAbstractEventDispatcherV2::registerTimer(int timerId, qint64 interval,
                                               BobUI::TimerType timerType, QObject *object)
{
    auto self = static_cast<QAbstractEventDispatcherV2 *>(this);
    self->registerTimer(BobUI::TimerId(timerId), interval * 1ms, timerType, object);
}

/*!
    \internal
    Temporary compatibility override.
*/
bool QAbstractEventDispatcherV2::unregisterTimer(int timerId)
{
    auto self = static_cast<QAbstractEventDispatcherV2 *>(this);
    return self->unregisterTimer(BobUI::TimerId(timerId));
}

/*!
    \internal
    Temporary compatibility override.
*/
auto QAbstractEventDispatcherV2::registeredTimers(QObject *object) const -> QList<TimerInfo>
{
    auto self = static_cast<const QAbstractEventDispatcherV2 *>(this);
    QList<TimerInfoV2> timers = self->timersForObject(object);
    QList<TimerInfo> result;
    result.reserve(timers.size());
    for (const TimerInfoV2 &t : timers)
        result.emplaceBack(qToUnderlying(t.timerId), fromDuration<int>(t.interval), t.timerType);
    return result;
}

/*!
    \internal
    Temporary compatibility override.
*/
int QAbstractEventDispatcherV2::remainingTime(int timerId)
{
    auto self = static_cast<QAbstractEventDispatcherV2 *>(this);
    return fromDuration<int>(self->remainingTime(BobUI::TimerId(timerId)));
}

/*!
    \internal
    Temporary compatibility override.
*/
bool QAbstractEventDispatcherV2::processEventsWithDeadline(QEventLoop::ProcessEventsFlags flags, QDeadlineTimer deadline)
{
    Q_UNUSED(deadline);
    return processEvents(flags);
}
#endif // ! BobUI 7

BOBUI_END_NAMESPACE

#include "moc_qabstracteventdispatcher.cpp"
