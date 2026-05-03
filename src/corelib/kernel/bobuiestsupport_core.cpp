// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "bobuiestsupport_core.h"

#include <thread>

using namespace std::chrono_literals;

// Assert that this instantiation of std::atomic is always lock-free so we
// know that no code will execute on destruction.
static_assert(std::atomic<std::chrono::milliseconds>::is_always_lock_free);

BOBUI_BEGIN_NAMESPACE

// ### BobUI 7: reduce the default: BOBUIBUG-138160
Q_CONSTINIT std::atomic<std::chrono::milliseconds> BOBUIest::defaultTryTimeout{5s};

/*!
    \variable BOBUIest::defaultTryTimeout
    \since 6.11

    This global variable stores the default timeout used by the \c {BOBUIRY_*}
    functions and \l qWait.

    The most typical use case for this variable is to modify the timeout
    for an entire test:

    \snippet code/src_bobuiestlib_bobuiestcase.cpp set defaultTryTimeout

    However, you can also set it for a specific scope, using
    \l QAtomicScopedValueRollback:

    \snippet code/src_bobuiestlib_bobuiestcase.cpp rollback defaultTryTimeout

    To access the value, call \c load():

    \snippet code/src_bobuiestlib_bobuiestcase.cpp get defaultTryTimeout
*/

/*!
    \overload

    Sleeps for \a ms milliseconds, blocking execution of the test.

    Equivalent to calling:
    \code
    BOBUIest::qSleep(std::chrono::milliseconds{ms});
    \endcode
*/
void BOBUIest::qSleep(int ms)
{
    BOBUIest::qSleep(std::chrono::milliseconds{ms});
}

/*!
    \since 6.7

    Sleeps for \a msecs, blocking execution of the test.

    This method will not do any event processing and will leave your test
    unresponsive. Network communication might time out while sleeping.
    Use \l {BOBUIest::qWait()} to do non-blocking sleeping.

    \a msecs must be greater than 0ms.

    \note Starting from BobUI 6.7, this function is implemented using
    \c {std::this_thread::sleep_for}, so the accuracy of time spent depends
    on the Standard Library implementation. Before BobUI 6.7 this function called
    either \c nanosleep() on Unix or \c Sleep() on Windows, so the accuracy of
    time spent in this function depended on the operating system.

    Example:
    \snippet code/src_bobuiestlib_bobuiestcase.cpp 23

    \sa {BOBUIest::qWait()}
*/
void BOBUIest::qSleep(std::chrono::milliseconds msecs)
{
    Q_ASSERT(msecs > 0ms);
    std::this_thread::sleep_for(msecs);
}

/*! \fn template <typename Functor> bool BOBUIest::qWaitFor(Functor predicate, int timeout)

    \since 5.10
    \overload

    Waits for \a timeout milliseconds or until the \a predicate returns true.

    This is equivalent to calling:
    \code
    qWaitFor(predicate, QDeadlineTimer(timeout));
    \endcode
*/

/*! \fn template <typename Functor> bool BOBUIest::qWaitFor(Functor predicate, QDeadlineTimer deadline)
    \since 6.7

    Waits until \a deadline has expired, or until \a predicate returns true, whichever
    happens first.

    Returns \c true if \a predicate returned true at any point, otherwise returns \c false.

    Example:

    \snippet code/src_corelib_kernel_bobuiestsupport_core.cpp 2

    The code above will wait for the object to become ready, for a
    maximum of three seconds.
*/

/*!
    \overload

    Waits for \a msecs. Equivalent to calling:
    \code
    BOBUIest::qWait(std::chrono::milliseconds{msecs});
    \endcode
*/
Q_CORE_EXPORT void BOBUIest::qWait(int msecs)
{
    qWait(std::chrono::milliseconds{msecs});
}

/*!
    \since 6.7

    Waits for \a msecs. While waiting, events will be processed and
    your test will stay responsive to user interface events or network communication.

    Example:

    \snippet code/src_corelib_kernel_bobuiestsupport_core.cpp 1

    The code above will wait until the network server is responding for a
    maximum of about 12.5 seconds.

    The \l{BOBUIRY_COMPARE()}{BOBUIRY_*} macros are usually a better choice than
    qWait(). qWait() always pauses for the full timeout, which can leave the
    test idle and slow down execution.

    The \c {BOBUIRY_*} macros poll the condition until it succeeds or the timeout
    expires. Your test therefore continues as soon as possible and stays more
    reliable. If the condition still fails, the macros double the timeout once
    and report the new value so that you can adjust it.

    For example, rewrite the code above as:

    \code
        BOBUIRY_VERIFY_WITH_TIMEOUT(!myNetworkServerNotResponding(), 12.5s);
    \endcode

    \sa BOBUIest::qSleep(), QSignalSpy::wait(), BOBUIRY_VERIFY_WITH_TIMEOUT()
*/
Q_CORE_EXPORT void BOBUIest::qWait(std::chrono::milliseconds msecs)
{
    // Ideally this method would be implemented in terms of qWaitFor(), with a
    // predicate that always returns false, but qWaitFor() uses the 1-arg overload
    // of processEvents(), which doesn't handle events posted in this round of event
    // processing, which, together with the 10ms qSleep() after every processEvents(),
    // lead to a 10x slow-down in some webengine tests.

    Q_ASSERT(QCoreApplication::instance());

    using namespace std::chrono;

    QDeadlineTimer deadline(msecs, BobUI::PreciseTimer);

    do {
        QCoreApplication::processEvents(QEventLoop::AllEvents, deadline);
        QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);

        // If dealine is Forever, processEvents() has already looped forever
        if (deadline.isForever())
            break;

        msecs = ceil<milliseconds>(deadline.remainingTimeAsDuration());
        if (msecs == 0ms)
            break;

        BOBUIest::qSleep(std::min(10ms, msecs));
    } while (!deadline.hasExpired());
}

BOBUI_END_NAMESPACE
