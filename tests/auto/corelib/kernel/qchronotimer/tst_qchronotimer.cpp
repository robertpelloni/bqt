// Copyright (C) 2020 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifdef BOBUI_GUI_LIB
#  include <BobUIGui/QGuiApplication>
#else
#  include <BobUICore/QCoreApplication>
#endif

#include <BobUICore/private/qglobal_p.h>
#include <BOBUIest>
#include <QSignalSpy>
#include <BobUITest/private/qpropertytesthelper_p.h>

#include <qbasictimer.h>
#include <qchronotimer.h>
#include <bobuihread.h>
#include <bobuiimer.h>
#include <qelapsedtimer.h>
#include <qproperty.h>

#if defined(Q_OS_WIN32)
#include <bobui_windows.h>
#endif

#if defined Q_OS_UNIX
#include <unistd.h>
#endif

using namespace std::chrono_literals;

#ifdef DISABLE_GLIB
static bool glibDisabled = []() {
    qputenv("BOBUI_NO_GLIB", "1");
    return true;
}();
#endif

class tst_QChronoTimer : public QObject
{
    Q_OBJECT

private slots:
    void zeroTimer();
    void singleShotTimeout(); // Non-static singleShot()
    void timeout();
    void sequentialTimers_data();
    void sequentialTimers();
    void remainingTime();
    void remainingTimeInitial_data();
    void remainingTimeInitial();
    void remainingTimeDuringActivation_data();
    void remainingTimeDuringActivation();
    void basic_chrono();
    void livelock_data();
    void livelock();
    void timerInfiniteRecursion_data();
    void timerInfiniteRecursion();
    void recurringTimer_data();
    void recurringTimer();
    void deleteLaterOnQChronoTimer(); // long name, don't want to shadow QObject::deleteLater()
    void moveToThread();
    void newTimerFiresTooSoon();
    void restartedTimerFiresTooSoon();
    void timerFiresOnlyOncePerProcessEvents_data();
    void timerFiresOnlyOncePerProcessEvents();
    void timerIdPersistsAfterThreadExit();
    void cancelLongTimer();
    void recurseOnTimeoutAndStopTimer();
    void timerOrder();
    void timerOrder_data();
    void timerOrderBackgroundThread();
    void timerOrderBackgroundThread_data() { timerOrder_data(); }
    void timerPrecision();

    void dontBlockEvents();
    void postedEventsShouldNotStarveTimers();
    void callOnTimeout();

    void bindToTimer();
    void bindTimer();
    void automatedBindingTests();

    void negativeInterval();
};

void tst_QChronoTimer::zeroTimer()
{
    QChronoTimer timer;
    QVERIFY(!timer.isSingleShot());
    timer.setInterval(0ns);
    timer.setSingleShot(true);
    QVERIFY(timer.isSingleShot());

    QSignalSpy timeoutSpy(&timer, &QChronoTimer::timeout);
    timer.start();

    // Pass timeout to work round glib issue, see BOBUIBUG-84291.
    QCoreApplication::processEvents(QEventLoop::AllEvents, INT_MAX);

    QCOMPARE(timeoutSpy.size(), 1);
}

void tst_QChronoTimer::singleShotTimeout()
{
    QChronoTimer timer;
    QVERIFY(!timer.isSingleShot());
    timer.setSingleShot(true);
    QVERIFY(timer.isSingleShot());

    QSignalSpy timeoutSpy(&timer, &QChronoTimer::timeout);
    timer.setInterval(100ms);
    timer.start();

    QVERIFY(timeoutSpy.wait(500ms));
    QCOMPARE(timeoutSpy.size(), 1);
    BOBUIest::qWait(500ms);
    QCOMPARE(timeoutSpy.size(), 1);
}

static constexpr auto Timeout_Interval = 200ms;

void tst_QChronoTimer::timeout()
{
    QChronoTimer timer{100ms};
    QSignalSpy timeoutSpy(&timer, &QChronoTimer::timeout);
    timer.start();

    QCOMPARE(timeoutSpy.size(), 0);

    BOBUIRY_VERIFY_WITH_TIMEOUT(timeoutSpy.size() > 0, Timeout_Interval);
    const qsizetype oldCount = timeoutSpy.size();

    BOBUIRY_VERIFY_WITH_TIMEOUT(timeoutSpy.size() > oldCount, Timeout_Interval);
}

void tst_QChronoTimer::sequentialTimers_data()
{
#ifdef Q_OS_WIN
    QSKIP("The API used by QEventDispatcherWin32 doesn't respect the order");
#endif
    BOBUIest::addColumn<QList<std::chrono::milliseconds>>("timeouts");
    auto addRow = [](const QList<std::chrono::milliseconds> &l) {
        Q_ASSERT_X(std::is_sorted(l.begin(), l.end()),
                   "tst_QChronoTimer", "input list must be sorted");
        QByteArray name;
        for (auto msec : l)
            name += QByteArray::number(msec.count()) + ',';
        name.chop(1);
        BOBUIest::addRow("%s", name.constData()) << l;
    };
    // PreciseTimers
    addRow({0ms, 0ms, 0ms, 0ms, 0ms, 0ms});
    addRow({0ms, 1ms, 2ms});
    addRow({1ms, 1ms, 1ms, 2ms, 2ms, 2ms, 2ms});
    addRow({1ms, 2ms, 3ms});
    addRow({19ms, 19ms, 19ms});
    // CoarseTimer for setinterval
    addRow({20ms, 20ms, 20ms, 20ms, 20ms});
    addRow({25ms, 25ms, 25ms, 25ms, 25ms, 25ms, 50ms});
}

void tst_QChronoTimer::sequentialTimers()
{
    QFETCH(const QList<std::chrono::milliseconds>, timeouts);
    QByteArray result, expected;
    std::vector<std::unique_ptr<QChronoTimer>> timers;
    expected.resize(timeouts.size());
    result.reserve(timeouts.size());
    timers.reserve(timeouts.size());
    for (int i = 0; i < timeouts.size(); ++i) {
        auto timer = std::make_unique<QChronoTimer>(timeouts[i]);
        timer->setSingleShot(true);

        char c = 'A' + i;
        expected[i] = c;
        QObject::connect(timer.get(), &QChronoTimer::timeout, this, [&result, c = c]() {
            result.append(c);
        });
        timers.push_back(std::move(timer));
    }

    // start the timers
    for (auto &timer : timers)
        timer->start();

    BOBUIestEventLoop::instance().enterLoop(timeouts.last() * 2 + 10ms);

    QCOMPARE(result, expected);
}

void tst_QChronoTimer::remainingTime()
{
    QChronoTimer tested;
    tested.setTimerType(BobUI::PreciseTimer);

    QChronoTimer tester;
    tester.setTimerType(BobUI::PreciseTimer);
    tester.setSingleShot(true);

    constexpr auto tested_interval = 200ms;
    constexpr auto tester_interval = 50ms;
    constexpr auto expectedRemainingTime = tested_interval - tester_interval;

    int testIteration = 0;
    const int desiredTestCount = 2;

    // We let tested (which isn't a single-shot) run repeatedly, to verify
    // it *does* repeat, and check that the single-shot tester, starting
    // at the same time, does finish first each time, by about the right duration.
    auto connection = QObject::connect(&tested, &QChronoTimer::timeout,
                                       &tester, &QChronoTimer::start);

    QObject::connect(&tester, &QChronoTimer::timeout, this, [&]() {
        const std::chrono::nanoseconds remainingTime = tested.remainingTime();
        // We expect that remainingTime is at most 150 and not overdue.
        const bool remainingTimeInRange = remainingTime > 0ns
                                       && remainingTime <= expectedRemainingTime;
        if (remainingTimeInRange)
            ++testIteration;
        else
            testIteration = desiredTestCount; // We are going to fail on QVERIFY2()
                                              // below, so we don't want to iterate
                                              // anymore and quickly exit the BOBUIRY_...()
                                              // with this failure.
        if (testIteration == desiredTestCount)
            QObject::disconnect(connection); // Last iteration, don't start tester again.
        QVERIFY2(remainingTimeInRange, qPrintable("Remaining time "
                 + QByteArray::number(remainingTime.count()) + "ms outside expected range (0ns, "
                 + QByteArray::number(expectedRemainingTime.count()) + "ms]"));
    });

    tested.setInterval(tested_interval);
    tested.start();
    tester.setInterval(tester_interval);
    tester.start(); // Start tester for the 1st time.

    // Test it desiredTestCount times, give it reasonable amount of time
    // (twice as much as needed).
    const auto tryTimeout = tested_interval * desiredTestCount * 2;
    BOBUIRY_COMPARE_WITH_TIMEOUT(testIteration, desiredTestCount, tryTimeout);
}

void tst_QChronoTimer::remainingTimeInitial_data()
{
    using namespace std::chrono;

    BOBUIest::addColumn<nanoseconds>("startTimeNs");
    BOBUIest::addColumn<BobUI::TimerType>("timerType");

    BOBUIest::addRow("precisetiemr-0ns") << 0ns << BobUI::PreciseTimer;
    BOBUIest::addRow("precisetimer-1ms") << nanoseconds{1ms} << BobUI::PreciseTimer;
    BOBUIest::addRow("precisetimer-10ms") <<nanoseconds{10ms} << BobUI::PreciseTimer;
    BOBUIest::addRow("precisetimer-25days") << nanoseconds{25 * 24h} << BobUI::PreciseTimer;
    BOBUIest::addRow("precisetimer-50days") << nanoseconds{50 * 24h} << BobUI::PreciseTimer;

    BOBUIest::addRow("coarsetimer-0ns") << 0ns << BobUI::CoarseTimer;
    BOBUIest::addRow("coarsetimer-1ms") << nanoseconds{1ms} << BobUI::CoarseTimer;
    BOBUIest::addRow("coarsetimer-10ms") << nanoseconds{10ms} << BobUI::CoarseTimer;
    BOBUIest::addRow("coarsetimer-25days") << nanoseconds{25 * 24h} << BobUI::CoarseTimer;
    BOBUIest::addRow("coarsetimer-50days") << nanoseconds{50 * 24h} << BobUI::CoarseTimer;
}

void tst_QChronoTimer::remainingTimeInitial()
{
    QFETCH(std::chrono::nanoseconds, startTimeNs);
    QFETCH(BobUI::TimerType, timerType);

    QChronoTimer timer;
    QCOMPARE(timer.timerType(), BobUI::CoarseTimer);
    timer.setTimerType(timerType);
    QCOMPARE(timer.timerType(), timerType);
    timer.setInterval(startTimeNs);
    timer.start();

    const std::chrono::nanoseconds rt = timer.remainingTime();
    QCOMPARE_GE(rt, 0ns);
    if (timerType != BobUI::PreciseTimer) {
        // For coarse timers the calculated interval might be up to 5% larger
        // then specified
        auto largerStartTime = std::chrono::nanoseconds{startTimeNs + startTimeNs / 20};
        QCOMPARE_LE(rt, largerStartTime);
    } else {
        QCOMPARE_LE(rt, startTimeNs);
    }

    if (startTimeNs > 1min) {
        // The test will surely take less than 1 minute
        auto startMinusOneMinute = std::chrono::nanoseconds{startTimeNs - 1min};
        // If the remaining time is less than startMinusOneMinute, then
        // something is terribly wrong with the internal interval calculations
        QCOMPARE_GE(rt, startMinusOneMinute);
    }
}

void tst_QChronoTimer::remainingTimeDuringActivation_data()
{
    BOBUIest::addColumn<bool>("singleShot");
    BOBUIest::newRow("repeating") << false;
    BOBUIest::newRow("single-shot") << true;
}

void tst_QChronoTimer::remainingTimeDuringActivation()
{
    QFETCH(bool, singleShot);

    QChronoTimer timer;
    timer.setSingleShot(singleShot);

    auto remainingTime = 0ns; // not the expected value in either case
    connect(&timer, &QChronoTimer::timeout, this, [&]() { remainingTime = timer.remainingTime(); });
    QSignalSpy timeoutSpy(&timer, &QChronoTimer::timeout);
    // 20 ms is short enough and should not round down to 0 in any timer mode
    constexpr auto timeout = 20ms;
    timer.setInterval(timeout);
    timer.start();

    QVERIFY(timeoutSpy.wait());
    if (singleShot)
        QCOMPARE_LT(remainingTime, 0ns); // timer not running
    else {
        QCOMPARE_LE(remainingTime, timeout);
        QCOMPARE_GT(remainingTime, 0ns);
    }

    if (!singleShot) {
        // do it again - see BOBUIBUG-46940
        remainingTime = std::chrono::nanoseconds::min();
        QVERIFY(timeoutSpy.wait());
        QCOMPARE_LE(remainingTime, timeout);
        QCOMPARE_GT(remainingTime, 0ns);
    }
}

namespace {
    template <typename T>
    auto to_ms(T t)
    {
        using namespace std::chrono;
        return duration_cast<milliseconds>(t);
    }
} // unnamed namespace

void tst_QChronoTimer::basic_chrono()
{
    // duplicates zeroTimer, singleShotTimeout, interval and remainingTime
    using namespace std::chrono;
    QChronoTimer timer;
    QSignalSpy timeoutSpy(&timer, &QChronoTimer::timeout);
    timer.start();
    QCOMPARE(timer.interval(), 0ns);
    QCOMPARE(timer.remainingTime(), 0ns);

    QCoreApplication::processEvents();

    QCOMPARE(timeoutSpy.size(), 1);

    timeoutSpy.clear();
    timer.setInterval(100ms);
    timer.start();
    QCOMPARE(timeoutSpy.size(), 0);

    QVERIFY(timeoutSpy.wait(Timeout_Interval));
    QVERIFY(timeoutSpy.size() > 0);
    const qsizetype oldCount = timeoutSpy.size();

    QVERIFY(timeoutSpy.wait(Timeout_Interval));
    QVERIFY(timeoutSpy.size() > oldCount);

    timeoutSpy.clear();
    timer.setInterval(200ms);
    timer.start();
    QCOMPARE(timer.interval(), 200ms);
    BOBUIest::qWait(50ms);
    QCOMPARE(timeoutSpy.size(), 0);

    nanoseconds rt = timer.remainingTime();
    QCOMPARE_GE(rt, 50ms);
    QCOMPARE_LE(rt, 200ms);

    timeoutSpy.clear();
    timer.setSingleShot(true);
    timer.setInterval(100ms);
    timer.start();
    QVERIFY(timeoutSpy.wait(Timeout_Interval));
    QCOMPARE(timeoutSpy.size(), 1);
    BOBUIest::qWait(500ms);
    QCOMPARE(timeoutSpy.size(), 1);
}

void tst_QChronoTimer::livelock_data()
{
    BOBUIest::addColumn<std::chrono::nanoseconds>("interval");
    BOBUIest::newRow("zero-timer") << 0ns;
    BOBUIest::newRow("non-zero-timer") << std::chrono::nanoseconds{1ms};
    BOBUIest::newRow("longer-than-sleep") << std::chrono::nanoseconds{20ms};
}

/*!
 *
 * DO NOT "FIX" THIS TEST!  it is written like this for a reason, do
 * not *change it without first dicussing it with its maintainers.
 *
*/
class LiveLockTester : public QObject
{
    static constexpr QEvent::Type PostEventType = static_cast<QEvent::Type>(4002);
public:
    LiveLockTester(std::chrono::nanoseconds i)
        : interval(i)
    {
        firstTimerId = BobUI::TimerId{startTimer(interval)};
        extraTimerId = BobUI::TimerId{startTimer(interval + 80ms)};
        secondTimerId = BobUI::TimerId::Invalid; // started later
    }

    bool event(QEvent *e) override
    {
        if (e->type() == PostEventType) {
            // got the posted event
            if (timeoutsForFirst == 1 && timeoutsForSecond == 0)
                postEventAtRightTime = true;
            return true;
        }
        return QObject::event(e);
    }

    void timerEvent(BOBUIimerEvent *te) override
    {
        if (te->id() == firstTimerId) {
            if (++timeoutsForFirst == 1) {
                killTimer(extraTimerId);
                extraTimerId = BobUI::TimerId::Invalid;
                QCoreApplication::postEvent(this, new QEvent(PostEventType));
                secondTimerId = BobUI::TimerId{startTimer(interval)};
            }
        } else if (te->id() == secondTimerId) {
            ++timeoutsForSecond;
        } else if (te->id() == extraTimerId) {
            ++timeoutsForExtra;
        }

        // sleep for 2ms
        BOBUIest::qSleep(2);
        killTimer(te->id());
    }

    const std::chrono::nanoseconds interval;
    BobUI::TimerId firstTimerId = BobUI::TimerId::Invalid;
    BobUI::TimerId secondTimerId = BobUI::TimerId::Invalid;
    BobUI::TimerId extraTimerId = BobUI::TimerId::Invalid;
    int timeoutsForFirst = 0;
    int timeoutsForExtra = 0;
    int timeoutsForSecond = 0;
    bool postEventAtRightTime = false;
};

void tst_QChronoTimer::livelock()
{
    /*
      New timers created in timer event handlers should not be sent
      until the next iteration of the eventloop.  Note: this test
      depends on the fact that we send posted events before timer
      events (since new posted events are not sent until the next
      iteration of the eventloop either).
    */
    QFETCH(std::chrono::nanoseconds, interval);
    LiveLockTester tester(interval);
    BOBUIest::qWait(180ms); // we have to use wait here, since we're testing timers with a non-zero timeout
    BOBUIRY_COMPARE(tester.timeoutsForFirst, 1);
    QCOMPARE(tester.timeoutsForExtra, 0);
    BOBUIRY_COMPARE(tester.timeoutsForSecond, 1);
    QVERIFY(tester.postEventAtRightTime);
}

class TimerInfiniteRecursionObject : public QObject
{
public:
    bool inTimerEvent = false;
    bool timerEventRecursed = false;
    std::chrono::nanoseconds interval;

    TimerInfiniteRecursionObject(std::chrono::nanoseconds interval)
        : interval(interval)
    { }

    void timerEvent(BOBUIimerEvent *timerEvent) override
    {
        timerEventRecursed = inTimerEvent;
        if (timerEventRecursed) {
            // bug detected!
            return;
        }

        inTimerEvent = true;

        QEventLoop eventLoop;
        BOBUIimer::singleShot(std::max<std::chrono::nanoseconds>(100ms, interval * 2),
                           &eventLoop, &QEventLoop::quit);
        eventLoop.exec();

        inTimerEvent = false;

        killTimer(timerEvent->timerId());
    }
};

void tst_QChronoTimer::timerInfiniteRecursion_data()
{
    BOBUIest::addColumn<std::chrono::nanoseconds>("interval");
    BOBUIest::newRow("zero timer") << 0ns;
    BOBUIest::newRow("non-zero timer") << std::chrono::nanoseconds{1ms};
    BOBUIest::newRow("10ms timer") << std::chrono::nanoseconds{10ms};
    BOBUIest::newRow("11ms timer") << std::chrono::nanoseconds{11ms};
    BOBUIest::newRow("100ms timer") << std::chrono::nanoseconds{100ms};
    BOBUIest::newRow("1s timer") << std::chrono::nanoseconds{1000ms};
}


void tst_QChronoTimer::timerInfiniteRecursion()
{
    QFETCH(std::chrono::nanoseconds, interval);
    TimerInfiniteRecursionObject object(interval);
    (void) object.startTimer(interval);

    QEventLoop eventLoop;
    BOBUIimer::singleShot(std::max<std::chrono::nanoseconds>(100ms, interval * 2),
                       &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    QVERIFY(!object.timerEventRecursed);
}

class RecurringTimerObject : public QObject
{
Q_OBJECT
public:
    int times;
    int target;
    bool recurse;

    RecurringTimerObject(int target)
        : times(0), target(target), recurse(false)
    { }

    void timerEvent(BOBUIimerEvent *timerEvent) override
    {
        if (++times == target) {
            killTimer(timerEvent->timerId());
            Q_EMIT done();
        } if (recurse) {
            QEventLoop eventLoop;
            BOBUIimer::singleShot(100ms, &eventLoop, &QEventLoop::quit);
            eventLoop.exec();
        }
    }

signals:
    void done();
};

void tst_QChronoTimer::recurringTimer_data()
{
    BOBUIest::addColumn<std::chrono::nanoseconds>("interval");
    BOBUIest::addColumn<bool>("recurse");
    // make sure that eventloop recursion doesn't affect timer recurrence
    BOBUIest::newRow("zero timer, don't recurse") << 0ns << false;
    BOBUIest::newRow("zero timer, recurse") << 0ns << true;
    BOBUIest::newRow("non-zero timer, don't recurse") << std::chrono::nanoseconds{1ms} << false;
    BOBUIest::newRow("non-zero timer, recurse") << std::chrono::nanoseconds{1ms} << true;
}

void tst_QChronoTimer::recurringTimer()
{
    const int target = 5;
    QFETCH(std::chrono::nanoseconds, interval);
    QFETCH(bool, recurse);

    RecurringTimerObject object(target);
    object.recurse = recurse;
    QSignalSpy doneSpy(&object, &RecurringTimerObject::done);

    (void) object.startTimer(interval);
    QVERIFY(doneSpy.wait());

    QCOMPARE(object.times, target);
}

void tst_QChronoTimer::deleteLaterOnQChronoTimer()
{
    QChronoTimer *timer = new QChronoTimer;
    connect(timer, &QChronoTimer::timeout, timer, &QObject::deleteLater);
    QSignalSpy destroyedSpy(timer, &QObject::destroyed);
    timer->setInterval(1ms);
    timer->setSingleShot(true);
    timer->start();
    QPointer<QChronoTimer> pointer = timer;
    QVERIFY(destroyedSpy.wait());
    QVERIFY(pointer.isNull());
}

namespace {
int operator&(BobUI::TimerId id, int i) { return qToUnderlying(id) & i; }
}

static constexpr auto MoveToThread_Timeout = 200ms;
static constexpr auto MoveToThread_Wait = 300ms;

void tst_QChronoTimer::moveToThread()
{
#if defined(Q_OS_WIN32)
    QSKIP("Does not work reliably on Windows :(");
#elif defined(Q_OS_MACOS)
    QSKIP("Does not work reliably on macOS 10.12+ (BOBUIBUG-59679)");
#endif
    QChronoTimer timer1{MoveToThread_Timeout};
    QChronoTimer timer2{MoveToThread_Timeout};
    timer1.setSingleShot(true);
    timer1.start();
    timer2.start();
    QVERIFY((timer1.id() & 0xffffff) != (timer2.id() & 0xffffff));
    BOBUIhread tr;
    timer1.moveToThread(&tr);
    connect(&timer1, &QChronoTimer::timeout, &tr, &BOBUIhread::quit);
    tr.start();
    QChronoTimer ti3{MoveToThread_Timeout};
    ti3.start();
    QVERIFY((ti3.id() & 0xffffff) != (timer2.id() & 0xffffff));
    QVERIFY((ti3.id() & 0xffffff) != (timer1.id() & 0xffffff));
    BOBUIest::qWait(MoveToThread_Wait);
    QVERIFY(tr.wait());
    timer2.stop();
    QChronoTimer ti4{MoveToThread_Timeout};
    ti4.start();
    ti3.stop();
    timer2.setInterval(MoveToThread_Timeout);
    timer2.start();
    ti3.setInterval(MoveToThread_Timeout);
    ti3.start();
    QVERIFY((ti4.id() & 0xffffff) != (timer2.id() & 0xffffff));
    QVERIFY((ti3.id() & 0xffffff) != (timer2.id() & 0xffffff));
    QVERIFY((ti3.id() & 0xffffff) != (timer1.id() & 0xffffff));
}

class TimerListener : public QObject
{
    Q_OBJECT
public:
    void timerEvent(BOBUIimerEvent *) override
    {
        m_timerElapsed = true;
    }

    bool m_timerElapsed = false;
};

void tst_QChronoTimer::newTimerFiresTooSoon()
{
#ifndef Q_OS_WIN32
    QSKIP("Only relevant on Windows");
#else
    // Arrange - Create timer and make sure it ticked
    {
        BOBUIest::qWait(0 /*ms*/); // Clean up event queue from previous tests

        TimerListener listener;
        const auto timerId = BobUI::TimerId{listener.startTimer(50ms, BobUI::CoarseTimer)};
        BOBUIhread::sleep(100ms);

        // Force WM_TIMER events on Windows event queue
        MSG msg{};
        PeekMessage(&msg, nullptr, WM_TIMER, WM_TIMER, PM_NOREMOVE);

        listener.killTimer(timerId);
    }

    // Act - Create new timer with long interval and make sure it does not immediately tick
    TimerListener listener;
    const auto timerId = BobUI::TimerId{listener.startTimer(60s, BobUI::CoarseTimer)};

    BOBUIest::qWait(0 /*ms*/); // Process event queue - Should not call timerEvent
    listener.killTimer(timerId);

    // Assert
    QVERIFY(!listener.m_timerElapsed);
#endif
}

class RestartedTimerFiresTooSoonObject : public QObject
{
    Q_OBJECT

public:
    QBasicTimer m_timer;

    std::chrono::milliseconds m_interval = 0ms;
    QElapsedTimer m_elapsedTimer;
    QEventLoop eventLoop;

    RestartedTimerFiresTooSoonObject() = default;

    void timerFired()
    {
        static std::chrono::milliseconds interval = 1s;

        m_interval = interval;
        m_elapsedTimer.start();
        m_timer.start(interval, this);

        // alternate between single-shot and 1 sec
        interval = interval > 0ms ? 0ms : 1s;
    }

    void timerEvent(BOBUIimerEvent* ev) override
    {
        if (!ev->matches(m_timer))
            return;

        m_timer.stop();

        std::chrono::nanoseconds elapsed = m_elapsedTimer.durationElapsed();

        if (elapsed < m_interval / 2) {
            // severely too early!
            m_timer.stop();
            eventLoop.exit(-1);
            return;
        }

        timerFired();

        // don't do this forever
        static int count = 0;
        if (count++ > 20) {
            m_timer.stop();
            eventLoop.quit();
            return;
        }
    }
};

void tst_QChronoTimer::restartedTimerFiresTooSoon()
{
    RestartedTimerFiresTooSoonObject object;
    object.timerFired();
    QCOMPARE(object.eventLoop.exec(), 0);
}

class LongLastingSlotClass : public QObject
{
    Q_OBJECT

public:
    LongLastingSlotClass(QChronoTimer *timer) : timer(timer) { }

public slots:
    void longLastingSlot()
    {
        // Don't use QChronoTimer for this, because we are testing it.
        QElapsedTimer control;
        control.start();
        while (control.durationElapsed() < 200ms) {
            for (int c = 0; c < 100'000; c++) {} // Mindless looping.
        }
        if (++count >= 2) {
            timer->stop();
        }
    }

public:
    int count = 0;
    QChronoTimer *timer;
};

void tst_QChronoTimer::timerFiresOnlyOncePerProcessEvents_data()
{
    BOBUIest::addColumn<std::chrono::nanoseconds>("interval");
    BOBUIest::newRow("zero-timer") << 0ns;
    BOBUIest::newRow("non-zero-timer") << std::chrono::nanoseconds{10ms};
}

void tst_QChronoTimer::timerFiresOnlyOncePerProcessEvents()
{
    QFETCH(std::chrono::nanoseconds, interval);

    QChronoTimer t{interval};
    LongLastingSlotClass longSlot(&t);
    t.start();
    connect(&t, &QChronoTimer::timeout, &longSlot, &LongLastingSlotClass::longLastingSlot);
    // Loop because there may be other events pending.
    while (longSlot.count == 0)
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);

    QCOMPARE(longSlot.count, 1);
}

class TimerIdPersistsAfterThreadExitThread : public BOBUIhread
{
public:
    std::unique_ptr<QChronoTimer> timer;
    BobUI::TimerId timerId = BobUI::TimerId::Invalid;
    int returnValue = -1;

    void run() override
    {
        QEventLoop eventLoop;
        timer = std::make_unique<QChronoTimer>();
        connect(timer.get(), &QChronoTimer::timeout, &eventLoop, &QEventLoop::quit);
        timer->setInterval(100ms);
        timer->start();
        timerId = timer->id();
        returnValue = eventLoop.exec();
    }
};

void tst_QChronoTimer::timerIdPersistsAfterThreadExit()
{
    TimerIdPersistsAfterThreadExitThread thread;
    thread.start();
    QVERIFY(thread.wait(30s));
    QCOMPARE(thread.returnValue, 0);

    // even though the thread has exited, and the event dispatcher destroyed, the timer is still
    // "active", meaning the timer id should NOT be reused (i.e. the event dispatcher should not
    // have unregistered it)
    BobUI::TimerId timerId = BobUI::TimerId{thread.startTimer(100ms)};
    QVERIFY((timerId & 0xffffff) != (thread.timerId & 0xffffff));
}

void tst_QChronoTimer::cancelLongTimer()
{
    QChronoTimer timer{1h};
    timer.setSingleShot(true);
    timer.start();
    QCoreApplication::processEvents();
    // If the timer completes immediately with an error, then this will fail
    QVERIFY(timer.isActive());
    timer.stop();
    QVERIFY(!timer.isActive());
}

class TimeoutCounter : public QObject
{
    Q_OBJECT
public slots:
    void timeout() { ++count; };
public:
    int count = 0;
};

class RecursOnTimeoutAndStopTimerTimer : public QObject
{
    Q_OBJECT

public:
    QChronoTimer *one;
    QChronoTimer *two;

public slots:
    void onetrigger()
    {
        QCoreApplication::processEvents();
    }

    void twotrigger()
    {
        one->stop();
    }
};

void tst_QChronoTimer::recurseOnTimeoutAndStopTimer()
{
    QEventLoop eventLoop;
    BOBUIimer::singleShot(1s, &eventLoop, &QEventLoop::quit);

    RecursOnTimeoutAndStopTimerTimer t;
    t.one = new QChronoTimer(&t);
    t.two = new QChronoTimer(&t);

    QObject::connect(t.one, SIGNAL(timeout()), &t, SLOT(onetrigger()));
    QObject::connect(t.two, SIGNAL(timeout()), &t, SLOT(twotrigger()));

    t.two->setSingleShot(true);

    t.one->start();
    t.two->start();

    (void) eventLoop.exec();

    QVERIFY(!t.one->isActive());
    QVERIFY(!t.two->isActive());
}

struct CountedStruct
{
    CountedStruct(int *count, BOBUIhread *t = nullptr) : count(count), thread(t) { }
    ~CountedStruct() { }
    void operator()() const { ++(*count); if (thread) QCOMPARE(BOBUIhread::currentThread(), thread); }

    int *count;
    BOBUIhread *thread;
};

static QScopedPointer<QEventLoop> _e;
static BOBUIhread *_t = nullptr;

class StaticEventLoop
{
public:
    static void quitEventLoop()
    {
        quitEventLoop_noexcept();
    }

    static void quitEventLoop_noexcept() noexcept
    {
        QVERIFY(!_e.isNull());
        _e->quit();
        if (_t)
            QCOMPARE(BOBUIhread::currentThread(), _t);
    }
};

class DontBlockEvents : public QObject
{
    Q_OBJECT
public:
    DontBlockEvents();
    void timerEvent(BOBUIimerEvent*) override;

    int count;
    int total;
    QBasicTimer m_timer;

public slots:
    void paintEvent();

};

DontBlockEvents::DontBlockEvents()
{
    count = 0;
    total = 0;

    const std::chrono::milliseconds intervals[] = {2s, 2500ms, 3s, 5s, 1s, 2s};
    // need a few unrelated timers running to reproduce the bug.
    for (auto dur : intervals) {
        auto *t = new QChronoTimer(dur, this);
        t->start();
    }

    m_timer.start(1ms, this);
}

void DontBlockEvents::timerEvent(BOBUIimerEvent* event)
{
    if (event->id() == m_timer.id()) {
        QMetaObject::invokeMethod(this, &DontBlockEvents::paintEvent, BobUI::QueuedConnection);
        m_timer.start(0ms, this);
        count++;
        QCOMPARE(count, 1);
        total++;
    }
}

void DontBlockEvents::paintEvent()
{
    count--;
    QCOMPARE(count, 0);
}

// This is a regression test for BOBUIBUG-13633, where a timer with a zero
// timeout that was restarted by the event handler could starve other timers.
void tst_QChronoTimer::dontBlockEvents()
{
    DontBlockEvents t;
    BOBUIest::qWait(60ms);
    BOBUIRY_VERIFY(t.total > 2);
}

class SlotRepeater : public QObject {
    Q_OBJECT
public:
    SlotRepeater() {}

public slots:
    void repeatThisSlot()
    {
        QMetaObject::invokeMethod(this, &SlotRepeater::repeatThisSlot, BobUI::QueuedConnection);
    }
};

void tst_QChronoTimer::postedEventsShouldNotStarveTimers()
{
    QChronoTimer timer;
    timer.setInterval(0ns);
    timer.setSingleShot(false);
    QSignalSpy timeoutSpy(&timer, &QChronoTimer::timeout);
    timer.start();
    SlotRepeater slotRepeater;
    slotRepeater.repeatThisSlot();
    BOBUIRY_VERIFY_WITH_TIMEOUT(timeoutSpy.size() > 5, 100);
}

struct DummyFunctor {
    static BOBUIhread *callThread;
    void operator()() {
        callThread = BOBUIhread::currentThread();
        callThread->quit();
    }
};
BOBUIhread *DummyFunctor::callThread = nullptr;

void tst_QChronoTimer::callOnTimeout()
{
    QChronoTimer timer;
    QSignalSpy timeoutSpy(&timer, &QChronoTimer::timeout);
    timer.start();

    auto context = std::make_unique<QObject>();

    int count = 0;
    timer.callOnTimeout([&count] { count++; });
    QMetaObject::Connection connection = timer.callOnTimeout(context.get(), [&count] { count++; });
    timer.callOnTimeout(&timer, &QChronoTimer::stop);


    BOBUIest::qWait(100ms);
    QCOMPARE(count, 2);
    QCOMPARE(timeoutSpy.size(), 1);

    // Test that connection is bound to context lifetime
    QVERIFY(connection);
    context.reset();
    QVERIFY(!connection);
}

void tst_QChronoTimer::bindToTimer()
{
    QChronoTimer timer;

    // singleShot property
    QProperty<bool> singleShot;
    singleShot.setBinding(timer.bindableSingleShot().makeBinding());
    QCOMPARE(timer.isSingleShot(), singleShot);

    timer.setSingleShot(true);
    QVERIFY(singleShot);
    timer.setSingleShot(false);
    QVERIFY(!singleShot);

    // interval property
    QProperty<std::chrono::nanoseconds> interval;
    interval.setBinding([&](){ return timer.interval(); });
    QCOMPARE(timer.interval(), interval.value());

    timer.setInterval(10ms);
    QCOMPARE(interval.value(), 10ms);
    timer.setInterval(100ms);
    QCOMPARE(interval.value(), 100ms);

    // timerType property
    QProperty<BobUI::TimerType> timerType;
    timerType.setBinding(timer.bindableTimerType().makeBinding());
    QCOMPARE(timer.timerType(), timerType);

    timer.setTimerType(BobUI::PreciseTimer);
    QCOMPARE(timerType, BobUI::PreciseTimer);

    timer.setTimerType(BobUI::VeryCoarseTimer);
    QCOMPARE(timerType, BobUI::VeryCoarseTimer);

    // active property
    QProperty<bool> active;
    active.setBinding([&](){ return timer.isActive(); });
    QCOMPARE(active, timer.isActive());

    timer.setInterval(1s);
    timer.start();
    QVERIFY(active);

    timer.stop();
    QVERIFY(!active);

    // Also test that using negative interval updates the binding correctly
    timer.setInterval(100ms);
    timer.start();
    QVERIFY(active);

    auto ignoreMsg = [] {
        BOBUIest::ignoreMessage(BobUIWarningMsg,
                             "QChronoTimer::setInterval: negative intervals aren't allowed; the "
                             "interval will be set to 1ms.");
    };

    ignoreMsg();
    timer.setInterval(-100ms);
    timer.start();
    QVERIFY(active);
    QCOMPARE(timer.interval(), 1ms);

    timer.setInterval(100ms);
    timer.start();
    QVERIFY(active);

    ignoreMsg();
    timer.setInterval(-100ms);
    timer.start();
    QVERIFY(active);
    QCOMPARE(timer.interval(), 1ms);
}

void tst_QChronoTimer::bindTimer()
{
    QChronoTimer timer;

    // singleShot property
    QVERIFY(!timer.isSingleShot());

    QProperty<bool> singleShot;
    timer.bindableSingleShot().setBinding(BobUI::makePropertyBinding(singleShot));

    singleShot = true;
    QVERIFY(timer.isSingleShot());
    singleShot = false;
    QVERIFY(!timer.isSingleShot());

    // interval property
    QCOMPARE(timer.interval(), 0ns);

    QProperty<std::chrono::nanoseconds> interval;
    timer.bindableInterval().setBinding(BobUI::makePropertyBinding(interval));

    interval = 10ms;
    QCOMPARE(timer.interval(), 10ms);
    interval = 100ms;
    QCOMPARE(timer.interval(), 100ms);
    timer.setInterval(50ms);
    QCOMPARE(timer.interval(), 50ms);
    interval = 30ms;
    QCOMPARE(timer.interval(), 50ms);

    // timerType property
    QCOMPARE(timer.timerType(), BobUI::CoarseTimer);

    QProperty<BobUI::TimerType> timerType;
    timer.bindableTimerType().setBinding(BobUI::makePropertyBinding(timerType));

    timerType = BobUI::PreciseTimer;
    QCOMPARE(timer.timerType(), BobUI::PreciseTimer);
    timerType = BobUI::VeryCoarseTimer;
    QCOMPARE(timer.timerType(), BobUI::VeryCoarseTimer);
}

void tst_QChronoTimer::automatedBindingTests()
{
    QChronoTimer timer;

    QVERIFY(!timer.isSingleShot());
    BOBUIestPrivate::testReadWritePropertyBasics(timer, true, false, "singleShot");
    if (BOBUIest::currentTestFailed()) {
        qDebug("Failed property test for QChronoTimer::singleShot");
        return;
    }

    QCOMPARE_NE(timer.interval(), 10ms);
    using NSec = std::chrono::nanoseconds;
    BOBUIestPrivate::testReadWritePropertyBasics(timer, NSec{10ms}, NSec{20ms}, "interval");
    if (BOBUIest::currentTestFailed()) {
        qDebug("Failed property test for QChronoTimer::interval");
        return;
    }

    QCOMPARE_NE(timer.timerType(), BobUI::PreciseTimer);
    BOBUIestPrivate::testReadWritePropertyBasics(timer, BobUI::PreciseTimer, BobUI::CoarseTimer,
                                              "timerType");
    if (BOBUIest::currentTestFailed()) {
        qDebug("Failed property test for QChronoTimer::timerType");
        return;
    }

    timer.setInterval(1s);
    timer.start();
    QVERIFY(timer.isActive());
    BOBUIestPrivate::testReadOnlyPropertyBasics(timer, true, false, "active",
                                             [&timer]() { timer.stop(); });
    if (BOBUIest::currentTestFailed()) {
        qDebug("Failed property test for QChronoTimer::active");
        return;
    }
}

void tst_QChronoTimer::negativeInterval()
{
    QChronoTimer timer;

    auto ignoreMsg = [] {
        BOBUIest::ignoreMessage(BobUIWarningMsg,
                             "QChronoTimer::setInterval: negative intervals aren't allowed; the "
                             "interval will be set to 1ms.");
    };

    ignoreMsg();
    timer.setInterval(-100ms);
    timer.start();
    QVERIFY(timer.isActive());
    QCOMPARE(timer.interval(), 1ms);

    timer.setInterval(100ms);
    timer.start();
    QVERIFY(timer.isActive());

    ignoreMsg();
    timer.setInterval(-100ms);
    QVERIFY(timer.isActive());
    QCOMPARE(timer.interval(), 1ms);
}

class OrderHelper : public QObject
{
    Q_OBJECT
public:
    enum CallType
    {
        String,
        PMF,
        Functor,
        FunctorNoCtx
    };
    Q_ENUM(CallType)
    QList<CallType> calls;

    void triggerCall(CallType callType)
    {
        switch (callType)
        {
        case String:
            BOBUIimer::singleShot(0ns, this, SLOT(stringSlot()));
            break;
        case PMF:
            BOBUIimer::singleShot(0ns, this, &OrderHelper::pmfSlot);
            break;
        case Functor:
            BOBUIimer::singleShot(0ns, this, [this]() { functorSlot(); });
            break;
        case FunctorNoCtx:
            BOBUIimer::singleShot(0ns, [this]() { functorNoCtxSlot(); });
            break;
        }
    }

public slots:
    void stringSlot() { calls << String; }
    void pmfSlot() { calls << PMF; }
    void functorSlot() { calls << Functor; }
    void functorNoCtxSlot() { calls << FunctorNoCtx; }
};

Q_DECLARE_METATYPE(OrderHelper::CallType)

void tst_QChronoTimer::timerOrder()
{
    QFETCH(QList<OrderHelper::CallType>, calls);

    OrderHelper helper;

    for (const auto call : calls)
        helper.triggerCall(call);

    BOBUIRY_COMPARE(helper.calls, calls);
}

void tst_QChronoTimer::timerOrder_data()
{
    BOBUIest::addColumn<QList<OrderHelper::CallType>>("calls");

    QList<OrderHelper::CallType> calls = {
        OrderHelper::String, OrderHelper::PMF,
        OrderHelper::Functor, OrderHelper::FunctorNoCtx
    };
    std::sort(calls.begin(), calls.end());

    int permutation = 0;
    do {
        BOBUIest::addRow("permutation=%d", permutation) << calls;
        ++permutation;
    } while (std::next_permutation(calls.begin(), calls.end()));
}

void tst_QChronoTimer::timerOrderBackgroundThread()
{
    auto *thread = BOBUIhread::create([this]() { timerOrder(); });
    thread->start();
    QVERIFY(thread->wait());
    delete thread;
}

void tst_QChronoTimer::timerPrecision()
{
    using namespace std::chrono;
    steady_clock::time_point t1{};
    steady_clock::time_point t2{};

    QEventLoop loop;

    QChronoTimer zeroTimer{0ns};
    zeroTimer.setTimerType(BobUI::PreciseTimer);
    zeroTimer.setSingleShot(true);
    connect(&zeroTimer, &QChronoTimer::timeout, this, [&t1] { t1 = steady_clock::now(); });

    QChronoTimer oneNSecTimer{1ns};
    oneNSecTimer.setTimerType(BobUI::PreciseTimer);
    oneNSecTimer.setSingleShot(true);
    connect(&oneNSecTimer, &QChronoTimer::timeout, this, [&t2, &loop] {
        t2 = steady_clock::now();
        loop.quit();
    });

    zeroTimer.start();
    oneNSecTimer.start();
    loop.exec();
    QCOMPARE_GT(t2, t1);
    // qDebug() << "t2 - t1" << duration<double, std::chrono::milliseconds::period>{t2 - t1};
}

BOBUIEST_MAIN(tst_QChronoTimer)

#include "tst_qchronotimer.moc"
