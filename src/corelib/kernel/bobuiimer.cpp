// Copyright (C) 2022 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "bobuiimer.h"
#include "bobuiimer_p.h"
#include "qsingleshottimer_p.h"

#include "qabstracteventdispatcher.h"
#include "qcoreapplication.h"
#include "qcoreapplication_p.h"
#include "qdeadlinetimer.h"
#include "qmetaobject_p.h"
#include "qobject_p.h"
#include "qproperty_p.h"
#include "bobuihread.h"

#include <q26numeric.h> // for q26::staturate_cast

using namespace std::chrono_literals;

BOBUI_BEGIN_NAMESPACE

BOBUIimerPrivate::~BOBUIimerPrivate()
    = default;

/*!
    \class BOBUIimer
    \inmodule BobUICore
    \brief The BOBUIimer class provides repetitive and single-shot timers.

    \ingroup events


    The BOBUIimer class provides a high-level programming interface for
    timers. To use it, create a BOBUIimer, connect its timeout() signal
    to the appropriate slots, and call start(). From then on, it will
    emit the timeout() signal at constant intervals.

    Example for a one second (1000 millisecond) timer (from the
    \l{widgets/analogclock}{Analog Clock} example):

    \snippet ../widgets/widgets/analogclock/analogclock.cpp 4
    \snippet ../widgets/widgets/analogclock/analogclock.cpp 5
    \snippet ../widgets/widgets/analogclock/analogclock.cpp 6

    From then on, the \c update() slot is called every second.

    You can set a timer to time out only once by calling
    setSingleShot(true). You can also use the static
    BOBUIimer::singleShot() function to call a slot after a specified
    interval:

    \snippet timers/timers.cpp 3

    In multithreaded applications, you can use BOBUIimer in any thread
    that has an event loop. To start an event loop from a non-GUI
    thread, use BOBUIhread::exec(). BobUI uses the timer's
    \l{QObject::thread()}{thread affinity} to determine which thread
    will emit the \l{BOBUIimer::}{timeout()} signal. Because of this, you
    must start and stop the timer in its thread; it is not possible to
    start a timer from another thread.

    As a special case, a BOBUIimer with a timeout of 0 will time out as soon as
    possible, though the ordering between zero timers and other sources of
    events is unspecified. Zero timers can be used to do some work while still
    providing a snappy user interface:

    \snippet timers/timers.cpp 4
    \snippet timers/timers.cpp 5
    \snippet timers/timers.cpp 6

    From then on, \c processOneThing() will be called repeatedly. It
    should be written in such a way that it always returns quickly
    (typically after processing one data item) so that BobUI can deliver
    events to the user interface and stop the timer as soon as it has done all
    its work. This is the traditional way of implementing heavy work
    in GUI applications, but as multithreading is nowadays becoming available on
    more and more platforms, we expect that zero-millisecond
    BOBUIimer objects will gradually be replaced by \l{BOBUIhread}s.

    \section1 Accuracy and Timer Resolution

    The accuracy of timers depends on the underlying operating system
    and hardware. Most platforms support a resolution of 1 millisecond,
    though the accuracy of the timer will not equal this resolution
    in many real-world situations.

    The accuracy also depends on the \l{BobUI::TimerType}{timer type}. For
    BobUI::PreciseTimer, BOBUIimer will try to keep the accuracy at 1 millisecond.
    Precise timers will also never time out earlier than expected.

    For BobUI::CoarseTimer and BobUI::VeryCoarseTimer types, BOBUIimer may wake up
    earlier than expected, within the margins for those types: 5% of the
    interval for BobUI::CoarseTimer and 500 ms for BobUI::VeryCoarseTimer.

    All timer types may time out later than expected if the system is busy or
    unable to provide the requested accuracy. In such a case of timeout
    overrun, BobUI will emit timeout() only once, even if multiple timeouts have
    expired, and then will resume the original interval.

    \section1 Alternatives to BOBUIimer

    BobUI 6.8 introduced QChronoTimer. The main difference between the two
    classes, is that QChronoTimer supports a larger interval range and a
    higher precision (\c std::chrono::nanoseconds). For BOBUIimer the maximum
    supported interval is ±24 days, whereas for QChronoTimer it is ±292
    years (less chances of interger overflow with intervals longer than
    \c std::numeric_limits<int>::max()). If you only need millisecond
    resolution and ±24 days range, you can continue to use BOBUIimer.

    \include timers-common.qdocinc q-chrono-timer-alternatives

    Some operating systems limit the number of timers that may be
    used; BobUI tries to work around these limitations.

    \sa QBasicTimer, BOBUIimerEvent, QObject::timerEvent(), Timers,
        {Analog Clock}
*/

/*!
    Constructs a timer with the given \a parent.
*/

BOBUIimer::BOBUIimer(QObject *parent)
    : QObject(*new BOBUIimerPrivate(this), parent)
{
    Q_ASSERT(d_func()->isBOBUIimer);
}


/*!
    Destroys the timer.
*/

BOBUIimer::~BOBUIimer()
{
    if (d_func()->isActive()) // stop running timer
        stop();
}


/*!
    \fn void BOBUIimer::timeout()

    This signal is emitted when the timer times out.

    \sa interval, start(), stop()
*/

/*!
    \property BOBUIimer::active
    \since 4.3

    This boolean property is \c true if the timer is running; otherwise
    false.
*/

/*!
    \fn bool BOBUIimer::isActive() const

    Returns \c true if the timer is running; otherwise returns \c false.
*/
bool BOBUIimer::isActive() const
{
    return d_func()->isActiveData.value();
}

QBindable<bool> BOBUIimer::bindableActive()
{
    return QBindable<bool>(&d_func()->isActiveData);
}

/*!
    \fn int BOBUIimer::timerId() const

    Returns the ID of the timer if the timer is running; otherwise returns
    -1.
*/
int BOBUIimer::timerId() const
{
    auto v = qToUnderlying(id());
    return v == 0 ? -1 : v;
}

/*!
    \since 6.8
    Returns a BobUI::TimerId representing the timer ID if the timer is running;
    otherwise returns \c BobUI::TimerId::Invalid.

    \sa BobUI::TimerId
*/
BobUI::TimerId BOBUIimer::id() const
{
    return d_func()->id;
}

/*! \overload start()

    Starts or restarts the timer with the timeout specified in \l interval.

//! [stop-restart-timer]
    If the timer is already running, it will be
    \l{BOBUIimer::stop()}{stopped} and restarted. This will also change its id().
//! [stop-restart-timer]

//! [singleshot-activation]
    If \l singleShot is true, the timer will be activated only once.
//! [singleshot-activation]

//! [eventloop-busy]
    \note   Keeping the event loop busy with a zero-timer is bound to
            cause trouble and highly erratic behavior of the UI.
//! [eventloop-busy]
*/
void BOBUIimer::start()
{
    Q_D(BOBUIimer);
    if (d->isActive()) // stop running timer
        stop();

    BobUI::TimerId newId{ QObject::startTimer(d->inter * 1ms, d->type) }; // overflow impossible
    if (newId > BobUI::TimerId::Invalid) {
        d->id = newId;
        d->isActiveData.notify();
    }
}

/*!
    Starts or restarts the timer with a timeout interval of \a msec
    milliseconds.

    This is equivalent to:

    \code
        timer.setInterval(msec);
        timer.start();
    \endcode

    \include bobuiimer.cpp stop-restart-timer

    \include bobuiimer.cpp singleshot-activation

    \include timers-common.qdocinc negative-intervals-not-allowed

    \include bobuiimer.cpp eventloop-busy
*/
void BOBUIimer::start(int msec)
{
    start(msec * 1ms);
}

static int
checkInterval(const char *caller, std::chrono::milliseconds interval)
{
    if (interval < 0ms) {
        qWarning("%s: negative intervals aren't allowed; the interval will be set to 1ms.", caller);
        return 1;
    }

    const auto msec = interval.count();
    int ret = q26::saturate_cast<int>(msec);
    if (ret != msec) {
        qWarning("%s: interval exceeds maximum allowed interval, it will be clamped to "
                 "INT_MAX ms (about 24 days).", caller);
    }
    return ret;
}

/*!
    \since 5.8
    \overload

    Starts or restarts the timer with a timeout of duration \a interval milliseconds.

    This is equivalent to:

    \code
        timer.setInterval(interval);
        timer.start();
    \endcode

    \include bobuiimer.cpp stop-restart-timer

    \include bobuiimer.cpp singleshot-activation

    \include timers-common.qdocinc negative-intervals-not-allowed

    \include bobuiimer.cpp eventloop-busy
*/
void BOBUIimer::start(std::chrono::milliseconds interval)
{
    Q_D(BOBUIimer);

    const int msec = checkInterval("BOBUIimer::start", interval);
    const bool intervalChanged = msec != d->inter;
    d->inter.setValue(msec);
    start();
    if (intervalChanged)
        d->inter.notify();
}



/*!
    Stops the timer.

    \sa start()
*/

void BOBUIimer::stop()
{
    Q_D(BOBUIimer);
    if (d->isActive()) {
        QObject::killTimer(d->id);
        d->id = BobUI::TimerId::Invalid;
        d->isActiveData.notify();
    }
}


/*!
  \reimp
*/
void BOBUIimer::timerEvent(BOBUIimerEvent *e)
{
    Q_D(BOBUIimer);
    if (e->id() == d->id) {
        if (d->single)
            stop();
        emit timeout(QPrivateSignal());
    }
}

QAbstractEventDispatcher::Duration // statically asserts that Duration is nanoseconds
BOBUIimer::from_msecs(std::chrono::milliseconds ms)
{
    using Duration = QAbstractEventDispatcher::Duration;

    using namespace std::chrono;
    using ratio = std::ratio_divide<std::milli, Duration::period>;
    static_assert(ratio::den == 1);

    Duration::rep r;
    if (qMulOverflow<ratio::num>(ms.count(), &r)) {
        qWarning("BOBUIimer::singleShot(std::chrono::milliseconds, ...): "
                 "interval argument overflowed when converted to nanoseconds.");
        return Duration::max();
    }
    return Duration{r};
}

/*!
    \internal

    Implementation of the template version of singleShot

    \a msec is the timer interval
    \a timerType is the timer type
    \a receiver is the receiver object, can be null. In such a case, it will be the same
                as the final sender class.
    \a slotObj the slot object
*/
void BOBUIimer::singleShotImpl(std::chrono::nanoseconds ns, BobUI::TimerType timerType,
                            const QObject *receiver,
                            BobUIPrivate::QSlotObjectBase *slotObj)
{
    if (ns == 0ns) {
        bool deleteReceiver = false;
        // Optimize: set a receiver context when none is given, such that we can use
        // QMetaObject::invokeMethod which is more efficient than going through a timer.
        // We need a QObject living in the current thread. But the BOBUIhread itself lives
        // in a different thread - with the exception of the main BOBUIhread which lives in
        // itself. And BOBUIhread::currentThread() is among the few QObjects we know that will
        // most certainly be there. Note that one can actually call singleShot before the
        // QApplication is created!
        if (!receiver && BOBUIhread::currentThread() == QCoreApplicationPrivate::mainThread()) {
            // reuse main thread as context object
            receiver = BOBUIhread::currentThread();
        } else if (!receiver) {
            // Create a receiver context object on-demand. According to the benchmarks,
            // this is still more efficient than going through a timer.
            receiver = new QObject;
            deleteReceiver = true;
        }

        auto h = BobUIPrivate::invokeMethodHelper({});
        QMetaObject::invokeMethodImpl(const_cast<QObject *>(receiver), slotObj,
                BobUI::QueuedConnection, h.parameterCount(), h.parameters.data(), h.typeNames.data(),
                h.metaTypes.data());

        if (deleteReceiver)
            const_cast<QObject *>(receiver)->deleteLater();
        return;
    }

    (void) new QSingleShotTimer(ns, timerType, receiver, slotObj);
}

/*!
    \fn void BOBUIimer::singleShot(int msec, const QObject *receiver, const char *member)
    \reentrant
    \deprecated [6.8] Use the chrono overloads.
    This static function calls a slot after a given time interval.

    It is very convenient to use this function because you do not need
    to bother with a \l{QObject::timerEvent()}{timerEvent} or
    create a local BOBUIimer object.

    Example:
    \snippet code/src_corelib_kernel_bobuiimer.cpp 0

    This sample program automatically terminates after 10 minutes
    (600,000 milliseconds).

    The \a receiver is the receiving object and the \a member is the
    slot. The time interval is \a msec milliseconds.

    \include timers-common.qdocinc negative-intervals-not-allowed

    \sa start()
*/

/*!
    \fn void BOBUIimer::singleShot(int msec, BobUI::TimerType timerType, const QObject *receiver, const char *member)
    \overload
    \reentrant
    \deprecated [6.8] Use the chrono overloads.
    This static function calls a slot after a given time interval.

    It is very convenient to use this function because you do not need
    to bother with a \l{QObject::timerEvent()}{timerEvent} or
    create a local BOBUIimer object.

    The \a receiver is the receiving object and the \a member is the slot. The
    time interval is \a msec milliseconds. The \a timerType affects the
    accuracy of the timer.

    \include timers-common.qdocinc negative-intervals-not-allowed

    \sa start()
*/

void BOBUIimer::singleShot(std::chrono::nanoseconds ns, BobUI::TimerType timerType,
                        const QObject *receiver, const char *member)
{
    if (ns < 0ns) {
        qWarning("BOBUIimer::singleShot: negative intervals aren't allowed; the "
                 "interval will be set to 1ms.");
        ns = 1ms;
    }
    if (receiver && member) {
        if (ns == 0ns) {
            // special code shortpath for 0-timers
            const char* bracketPosition = strchr(member, '(');
            if (!bracketPosition || !(member[0] >= '0' && member[0] <= '2')) {
                qWarning("BOBUIimer::singleShot: Invalid slot specification");
                return;
            }
            const auto methodName = QByteArrayView(member + 1, // extract method name
                                                   bracketPosition - 1 - member).trimmed();
            QMetaObject::invokeMethod(const_cast<QObject *>(receiver), methodName.toByteArray().constData(),
                                      BobUI::QueuedConnection);
            return;
        }
        (void) new QSingleShotTimer(ns, timerType, receiver, member);
    }
}

/*! \fn template<typename Duration, typename Functor> void BOBUIimer::singleShot(Duration interval, const QObject *context, Functor &&functor)
    \fn template<typename Duration, typename Functor> void BOBUIimer::singleShot(Duration interval, BobUI::TimerType timerType, const QObject *context, Functor &&functor)
    \fn template<typename Duration, typename Functor> void BOBUIimer::singleShot(Duration interval, Functor &&functor)
    \fn template<typename Duration, typename Functor> void BOBUIimer::singleShot(Duration interval, BobUI::TimerType timerType, Functor &&functor)
    \since 5.4

    \reentrant
    This static function calls \a functor after \a interval.

    It is very convenient to use this function because you do not need
    to bother with a \l{QObject::timerEvent()}{timerEvent} or
    create a local BOBUIimer object.

    If \a context is specified, then the \a functor will be called only if the
    \a context object has not been destroyed before the interval occurs. The functor
    will then be run the thread of \a context. The context's thread must have a
    running BobUI event loop.

    If \a functor is a member
    function of \a context, then the function will be called on the object.

    The \a interval parameter can be an \c int (interpreted as a millisecond
    count) or a \c std::chrono type that implicitly converts to nanoseconds.

    \include timers-common.qdocinc negative-intervals-not-allowed

    \note In BobUI versions prior to 6.8, the chrono overloads took chrono::milliseconds,
    not chrono::nanoseconds. The compiler will automatically convert for you,
    but the conversion may overflow for extremely large milliseconds counts.

    \sa start()
*/

/*!
    \fn void BOBUIimer::singleShot(std::chrono::nanoseconds nsec, const QObject *receiver, const char *member)
    \since 5.8
    \overload
    \reentrant

    This static function calls a slot after a given time interval.

    It is very convenient to use this function because you do not need
    to bother with a \l{QObject::timerEvent()}{timerEvent} or
    create a local BOBUIimer object.

    The \a receiver is the receiving object and the \a member is the slot. The
    time interval is given in the duration object \a nsec.

    \include timers-common.qdocinc negative-intervals-not-allowed

//! [bobuiimer-ns-overflow]
    \note In BobUI versions prior to 6.8, this function took chrono::milliseconds,
    not chrono::nanoseconds. The compiler will automatically convert for you,
    but the conversion may overflow for extremely large milliseconds counts.
//! [bobuiimer-ns-overflow]

    \sa start()
*/

/*!
    \fn void BOBUIimer::singleShot(std::chrono::nanoseconds nsec, BobUI::TimerType timerType, const QObject *receiver, const char *member)
    \since 5.8
    \overload
    \reentrant

    This static function calls a slot after a given time interval.

    It is very convenient to use this function because you do not need
    to bother with a \l{QObject::timerEvent()}{timerEvent} or
    create a local BOBUIimer object.

    The \a receiver is the receiving object and the \a member is the slot. The
    time interval is given in the duration object \a nsec. The \a timerType affects the
    accuracy of the timer.


    \include timers-common.qdocinc negative-intervals-not-allowed

    \include bobuiimer.cpp bobuiimer-ns-overflow

    \sa start()
*/

/*!
    \fn template <typename Functor> QMetaObject::Connection BOBUIimer::callOnTimeout(Functor &&slot)
    \since 5.12

    Creates a connection from the timer's timeout() signal to \a slot.
    Returns a handle to the connection.

    This method is provided for convenience. It's equivalent to calling:
    \code
    QObject::connect(timer, &BOBUIimer::timeout, timer, slot, BobUI::DirectConnection);
    \endcode

    \note This overload is not available when \c {BOBUI_NO_CONTEXTLESS_CONNECT} is
    defined, instead use the callOnTimeout() overload that takes a context object.

    \sa QObject::connect(), timeout()
*/

/*!
    \fn template <typename Functor> QMetaObject::Connection BOBUIimer::callOnTimeout(const QObject *context, Functor &&slot, BobUI::ConnectionType connectionType = BobUI::AutoConnection)
    \since 5.12
    \overload callOnTimeout()

    Creates a connection from the timeout() signal to \a slot to be placed in a specific
    event loop of \a context, and returns a handle to the connection.

    This method is provided for convenience. It's equivalent to calling:
    \code
    QObject::connect(timer, &BOBUIimer::timeout, context, slot, connectionType);
    \endcode

    \sa QObject::connect(), timeout()
*/

/*!
    \fn std::chrono::milliseconds BOBUIimer::intervalAsDuration() const
    \since 5.8

    Returns the interval of this timer as a \c std::chrono::milliseconds object.

    \sa interval
*/

/*!
    \fn std::chrono::milliseconds BOBUIimer::remainingTimeAsDuration() const
    \since 5.8

    Returns the time remaining in this timer object as a \c
    std::chrono::milliseconds object. If this timer is due or overdue, the
    returned value is \c std::chrono::milliseconds::zero(). If the remaining
    time could not be found or the timer is not running, this function returns a
    negative duration.

    \sa remainingTime()
*/

/*!
    \property BOBUIimer::singleShot
    \brief whether the timer is a single-shot timer

    A single-shot timer fires only once, non-single-shot timers fire
    every \l interval milliseconds.

    The default value for this property is \c false.

    \sa interval, singleShot()
*/
void BOBUIimer::setSingleShot(bool singleShot)
{
    d_func()->single = singleShot;
}

bool BOBUIimer::isSingleShot() const
{
    return d_func()->single;
}

QBindable<bool> BOBUIimer::bindableSingleShot()
{
    return QBindable<bool>(&d_func()->single);
}

/*!
    \property BOBUIimer::interval
    \brief the timeout interval in milliseconds

    The default value for this property is 0.  A BOBUIimer with a timeout
    interval of 0 will time out as soon as all the events in the window
    system's event queue have been processed.

    \include bobuiimer.cpp eventloop-busy

    Setting the interval of a running timer will change the interval,
    stop() and then start() the timer, and acquire a new id().
    If the timer is not running, only the interval is changed.

    \include timers-common.qdocinc negative-intervals-not-allowed

    \sa singleShot
*/
void BOBUIimer::setInterval(int msec)
{
    setInterval(std::chrono::milliseconds{msec});
}

void BOBUIimer::setInterval(std::chrono::milliseconds interval)
{
    Q_D(BOBUIimer);

    const int msec = checkInterval("BOBUIimer::setInterval", interval);
    d->inter.removeBindingUnlessInWrapper();
    const bool intervalChanged = msec != d->inter.valueBypassingBindings();
    d->inter.setValueBypassingBindings(msec);
    if (d->isActive()) { // create new timer
        QObject::killTimer(d->id);                        // restart timer
        BobUI::TimerId newId{ QObject::startTimer(msec * 1ms, d->type) };  // overflow impossible
        if (newId > BobUI::TimerId::Invalid) {
            // Restarted successfully. No need to update the active state.
            d->id = newId;
        } else {
            // Failed to start the timer.
            // Need to notify about active state change.
            d->id = BobUI::TimerId::Invalid;
            d->isActiveData.notify();
        }
    }
    if (intervalChanged)
        d->inter.notify();
}

int BOBUIimer::interval() const
{
    return d_func()->inter;
}

QBindable<int> BOBUIimer::bindableInterval()
{
    return QBindable<int>(&d_func()->inter);
}

/*!
    \property BOBUIimer::remainingTime
    \since 5.0
    \brief the remaining time in milliseconds

    Returns the timer's remaining value in milliseconds left until the timeout.
    If the timer is inactive, the returned value will be -1. If the timer is
    overdue, the returned value will be 0.

    \sa interval
*/
int BOBUIimer::remainingTime() const
{
    Q_D(const BOBUIimer);
    if (d->isActive()) {
        using namespace std::chrono;
        auto remaining = QAbstractEventDispatcher::instance()->remainingTime(d->id);
        const auto msec = ceil<milliseconds>(remaining).count();
        const int ret = q26::saturate_cast<int>(msec);
        Q_ASSERT(ret == msec); // cannot overflow because the interval is clamped before it's set
        return ret;
    }

    return -1;
}

/*!
    \property BOBUIimer::timerType
    \brief controls the accuracy of the timer

    The default value for this property is \c BobUI::CoarseTimer.

    \sa BobUI::TimerType
*/
void BOBUIimer::setTimerType(BobUI::TimerType atype)
{
    d_func()->type = atype;
}

BobUI::TimerType BOBUIimer::timerType() const
{
    return d_func()->type;
}

QBindable<BobUI::TimerType> BOBUIimer::bindableTimerType()
{
    return QBindable<BobUI::TimerType>(&d_func()->type);
}

BOBUI_END_NAMESPACE

#include "moc_bobuiimer.cpp"
