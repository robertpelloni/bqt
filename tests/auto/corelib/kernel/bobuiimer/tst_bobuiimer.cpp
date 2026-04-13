// Copyright (C) 2020 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

/* WARNING: this source-code is reused by another test.

   As BobUI built with GUI support may use a different backend for its event loops
   and other timer-related matters, it is important to test it in that form, as
   well as in its GUI-less form. So this source file is reused by a build config
   in the GUI module. Similarly, testing with and without glib is supported,
   where relevant (see DISABLE_GLIB below).
*/
#ifdef BOBUI_GUI_LIB
// When compiled as tests/auto/gui/kernel/qguitimer/'s source-code:
#  include <BobUIGui/QGuiApplication>
#else
// When compiled as tests/auto/corelib/kernel/bobuiimer/'s source-code:
#  include <BobUICore/QCoreApplication>
#endif

#include <BobUICore/private/qglobal_p.h>
#include <BOBUIest>
#include <QSignalSpy>
#include <BobUITest/private/qpropertytesthelper_p.h>

#include <BobUICore/private/qsingleshottimer_p.h>
#include <bobuiimer.h>
#include <bobuihread.h>
#include <qelapsedtimer.h>
#include <qproperty.h>

#if defined Q_OS_UNIX
#include <unistd.h>
#endif

#ifdef DISABLE_GLIB
static bool glibDisabled = []() {
    qputenv("BOBUI_NO_GLIB", "1");
    return true;
}();
#endif

using namespace std::chrono_literals;

class tst_BOBUIimer : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void zeroTimer();
    void singleShotTimeout();
    void timeout();
    void singleShotNormalizes_data();
    void singleShotNormalizes();
    void sequentialTimers_data();
    void sequentialTimers();
    void singleShotSequentialTimers_data();
    void singleShotSequentialTimers();
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
    void deleteLaterOnBOBUIimer(); // long name, don't want to shadow QObject::deleteLater()
    void moveToThread();
    void restartedTimerFiresTooSoon();
    void timerFiresOnlyOncePerProcessEvents_data();
    void timerFiresOnlyOncePerProcessEvents();
    void timerIdPersistsAfterThreadExit();
    void cancelLongTimer();
    void singleShotStaticFunctionZeroTimeout();
    void recurseOnTimeoutAndStopTimer();
    void singleShotToFunctors();
    void singleShot_chrono();
    void singleShot_static();
    void singleShotDestructionBeforeEventDispatcher();
    void crossThreadSingleShotToFunctor_data();
    void crossThreadSingleShotToFunctor();
#ifdef BOBUI_BUILD_INTERNAL
    void crossThreadSingleShotDestruction_data();
    void crossThreadSingleShotDestruction();
#endif
    void timerOrder();
    void timerOrder_data();
    void timerOrderBackgroundThread();
    void timerOrderBackgroundThread_data() { timerOrder_data(); }

    void dontBlockEvents();
    void postedEventsShouldNotStarveTimers();
    void callOnTimeout();

    void bindToTimer();
    void bindTimer();
    void automatedBindingTests();

    void negativeInterval();
    void testTimerId();

    void intervalOverflow();
};

void tst_BOBUIimer::zeroTimer()
{
    BOBUIimer timer;
    QVERIFY(!timer.isSingleShot());
    timer.setInterval(0);
    timer.setSingleShot(true);
    QVERIFY(timer.isSingleShot());

    QSignalSpy timeoutSpy(&timer, &BOBUIimer::timeout);
    timer.start();

    // Pass timeout to work round glib issue, see BOBUIBUG-84291.
    QCoreApplication::processEvents(QEventLoop::AllEvents, INT_MAX);

    QCOMPARE(timeoutSpy.size(), 1);
}

void tst_BOBUIimer::singleShotTimeout()
{
    BOBUIimer timer;
    QVERIFY(!timer.isSingleShot());
    timer.setSingleShot(true);
    QVERIFY(timer.isSingleShot());

    QSignalSpy timeoutSpy(&timer, &BOBUIimer::timeout);
    timer.start(100);

    QVERIFY(timeoutSpy.wait(500));
    QCOMPARE(timeoutSpy.size(), 1);
    BOBUIest::qWait(500);
    QCOMPARE(timeoutSpy.size(), 1);
}

#define TIMEOUT_TIMEOUT 200

void tst_BOBUIimer::timeout()
{
    BOBUIimer timer;
    QSignalSpy timeoutSpy(&timer, &BOBUIimer::timeout);
    timer.start(100);

    QCOMPARE(timeoutSpy.size(), 0);

    BOBUIRY_VERIFY_WITH_TIMEOUT(timeoutSpy.size() > 0, TIMEOUT_TIMEOUT);
    int oldCount = timeoutSpy.size();

    BOBUIRY_VERIFY_WITH_TIMEOUT(timeoutSpy.size() > oldCount, TIMEOUT_TIMEOUT);
}

void tst_BOBUIimer::singleShotNormalizes_data()
{
    BOBUIest::addColumn<QByteArray>("slotName");

    BOBUIest::newRow("normalized") << QByteArray(SLOT(exitLoop()));

    BOBUIest::newRow("space-before") << QByteArray(SLOT( exitLoop()));
    BOBUIest::newRow("space-after") << QByteArray(SLOT(exitLoop ()));
    BOBUIest::newRow("space-around") << QByteArray(SLOT( exitLoop ()));
    BOBUIest::newRow("spaces-before") << QByteArray(SLOT(  exitLoop()));
    BOBUIest::newRow("spaces-after") << QByteArray(SLOT(exitLoop  ()));
    BOBUIest::newRow("spaces-around") << QByteArray(SLOT(  exitLoop  ()));

    BOBUIest::newRow("space-in-parens") << QByteArray(SLOT(exitLoop( )));
    BOBUIest::newRow("spaces-in-parens") << QByteArray(SLOT(exitLoop(  )));
    BOBUIest::newRow("space-after-parens") << QByteArray(SLOT(exitLoop() ));
    BOBUIest::newRow("spaces-after-parens") << QByteArray(SLOT(exitLoop()  ));
}

void tst_BOBUIimer::singleShotNormalizes()
{
    static constexpr auto TestTimeout = 250ms;
    QFETCH(QByteArray, slotName);
    QEventLoop loop;

    // control test: regular connection
    {
        BOBUIimer timer;
        QVERIFY(QObject::connect(&timer, SIGNAL(timeout()), &BOBUIestEventLoop::instance(), slotName));
        timer.setSingleShot(true);
        timer.start(1);
        BOBUIestEventLoop::instance().enterLoop(TestTimeout);
        QVERIFY(!BOBUIestEventLoop::instance().timeout());
    }

    // non-zero time
    BOBUIimer::singleShot(1, &BOBUIestEventLoop::instance(), slotName);
    BOBUIestEventLoop::instance().enterLoop(TestTimeout);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());

    BOBUIimer::singleShot(1ms, &BOBUIestEventLoop::instance(), slotName);
    BOBUIestEventLoop::instance().enterLoop(TestTimeout);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());

    // zero time
    BOBUIimer::singleShot(0, &BOBUIestEventLoop::instance(), slotName);
    BOBUIestEventLoop::instance().enterLoop(TestTimeout);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());

    BOBUIimer::singleShot(0ms, &BOBUIestEventLoop::instance(), slotName);
    BOBUIestEventLoop::instance().enterLoop(TestTimeout);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());
}

void tst_BOBUIimer::sequentialTimers_data()
{
#ifdef Q_OS_WIN
    QSKIP("The API used by QEventDispatcherWin32 doesn't respect the order");
#endif
    BOBUIest::addColumn<QList<int>>("timeouts");
    auto addRow = [](const QList<int> &l) {
        QByteArray name;
        int last = -1;
        for (int i = 0; i < l.size(); ++i) {
            Q_ASSERT_X(l[i] >= last, "tst_BOBUIimer", "input list must be sorted");
            name += QByteArray::number(l[i]) + ',';
        }
        name.chop(1);
        BOBUIest::addRow("%s", name.constData()) << l;
    };
    // PreciseTimers
    addRow({0, 0, 0, 0, 0, 0});
    addRow({0, 1, 2});
    addRow({1, 1, 1, 2, 2, 2, 2});
    addRow({1, 2, 3});
    addRow({19, 19, 19});
    // CoarseTimer for setInterval
    addRow({20, 20, 20, 20, 20});
    addRow({25, 25, 25, 25, 25, 25, 50});
}

void tst_BOBUIimer::sequentialTimers()
{
    QFETCH(const QList<int>, timeouts);
    QByteArray result, expected;
    std::vector<std::unique_ptr<BOBUIimer>> timers;
    expected.resize(timeouts.size());
    result.reserve(timeouts.size());
    timers.reserve(timeouts.size());
    for (int i = 0; i < timeouts.size(); ++i) {
        auto timer = std::make_unique<BOBUIimer>();
        timer->setSingleShot(true);
        timer->setInterval(timeouts[i]);

        char c = 'A' + i;
        expected[i] = c;
        QObject::connect(timer.get(), &BOBUIimer::timeout, this, [&result, c = c]() {
            result.append(c);
        });
        timers.push_back(std::move(timer));
    }

    // start the timers
    for (auto &timer : timers)
        timer->start();

    BOBUIestEventLoop::instance().enterLoopMSecs(timeouts.last() * 2 + 10);

    QCOMPARE(result, expected);
}

void tst_BOBUIimer::singleShotSequentialTimers_data()
{
    sequentialTimers_data();
}

void tst_BOBUIimer::singleShotSequentialTimers()
{
    QFETCH(const QList<int>, timeouts);
    QByteArray result, expected;
    expected.resize(timeouts.size());
    result.reserve(timeouts.size());
    for (int i = 0; i < timeouts.size(); ++i) {
        char c = 'A' + i;
        expected[i] = c;
        BOBUIimer::singleShot(timeouts[i], this, [&result, c = c]() {
            result.append(c);
        });
    }

    BOBUIestEventLoop::instance().enterLoopMSecs(timeouts.last() * 2 + 10);

    QCOMPARE(result, expected);
}

void tst_BOBUIimer::remainingTime()
{
    BOBUIimer tested;
    tested.setTimerType(BobUI::PreciseTimer);

    BOBUIimer tester;
    tester.setTimerType(BobUI::PreciseTimer);
    tester.setSingleShot(true);

    const int testedInterval = 200;
    const int testerInterval = 50;
    const int expectedRemainingTime = testedInterval - testerInterval;

    int testIteration = 0;
    const int desiredTestCount = 2;

    auto connection = QObject::connect(&tested, &BOBUIimer::timeout, [&tester]() {
        // We let tested (which isn't a single-shot) run repeatedly, to verify
        // it *does* repeat, and check that the single-shot tester, starting
        // at the same time, does finish first each time, by about the right duration.
        tester.start(); // Start tester again.
    });

    QObject::connect(&tester, &BOBUIimer::timeout, [&]() {
        const int remainingTime = tested.remainingTime();
        // We expect that remainingTime is at most 150 and not overdue.
        const bool remainingTimeInRange = remainingTime > 0
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
                 + QByteArray::number(remainingTime) + "ms outside expected range (0ms, "
                 + QByteArray::number(expectedRemainingTime) + "ms]"));
    });

    tested.start(testedInterval);
    tester.start(testerInterval); // Start tester for the 1st time.

    // Test it desiredTestCount times, give it reasonable amount of time
    // (twice as much as needed).
    BOBUIRY_COMPARE_WITH_TIMEOUT(testIteration, desiredTestCount,
                              testedInterval * desiredTestCount * 2);
}

void tst_BOBUIimer::remainingTimeInitial_data()
{
    BOBUIest::addColumn<int>("startTimeMs");
    BOBUIest::addColumn<BobUI::TimerType>("timerType");

    BOBUIest::addRow("precise time 0ms") << 0 << BobUI::PreciseTimer;
    BOBUIest::addRow("precise time 1ms") << 1 << BobUI::PreciseTimer;
    BOBUIest::addRow("precise time 10ms") << 10 << BobUI::PreciseTimer;

    BOBUIest::addRow("coarse time 0ms") << 0 << BobUI::CoarseTimer;
    BOBUIest::addRow("coarse time 1ms") << 1 << BobUI::CoarseTimer;
    BOBUIest::addRow("coarse time 10ms") << 10 << BobUI::CoarseTimer;
}

void tst_BOBUIimer::remainingTimeInitial()
{
    QFETCH(int, startTimeMs);
    QFETCH(BobUI::TimerType, timerType);

    BOBUIimer timer;
    QCOMPARE(timer.timerType(), BobUI::CoarseTimer);
    timer.setTimerType(timerType);
    QCOMPARE(timer.timerType(), timerType);
    timer.start(startTimeMs);

    const int rt = timer.remainingTime();
    QVERIFY2(rt >= 0 && rt <= startTimeMs, qPrintable(QString::number(rt)));
}

void tst_BOBUIimer::remainingTimeDuringActivation_data()
{
    BOBUIest::addColumn<bool>("singleShot");
    BOBUIest::newRow("repeating") << false;
    BOBUIest::newRow("single-shot") << true;
}

void tst_BOBUIimer::remainingTimeDuringActivation()
{
    QFETCH(bool, singleShot);

    BOBUIimer timer;
    timer.setSingleShot(singleShot);

    int remainingTime = 0; // not the expected value in either case
    connect(&timer, &BOBUIimer::timeout,
            [&]() {
                remainingTime = timer.remainingTime();
            });
    QSignalSpy timeoutSpy(&timer, &BOBUIimer::timeout);
    const int timeout = 20; // 20 ms is short enough and should not round down to 0 in any timer mode
    timer.start(timeout);

    QVERIFY(timeoutSpy.wait());
    if (singleShot)
        QCOMPARE(remainingTime, -1);     // timer not running
    else
        QVERIFY2(remainingTime <= timeout && remainingTime > 0,
                 qPrintable(QString::number(remainingTime)));

    if (!singleShot) {
        // do it again - see BOBUIBUG-46940
        remainingTime = -1;
        QVERIFY(timeoutSpy.wait());
        QVERIFY2(remainingTime <= timeout && remainingTime > 0,
                 qPrintable(QString::number(remainingTime)));
    }
}

namespace {

    template <typename T>
    std::chrono::milliseconds to_ms(T t)
    { return std::chrono::duration_cast<std::chrono::milliseconds>(t); }

} // unnamed namespace

void tst_BOBUIimer::basic_chrono()
{
    // duplicates zeroTimer, singleShotTimeout, interval and remainingTime
    using namespace std::chrono;
    BOBUIimer timer;
    QSignalSpy timeoutSpy(&timer, &BOBUIimer::timeout);
    timer.setInterval(to_ms(nanoseconds(0)));
    timer.start();
    QCOMPARE(timer.intervalAsDuration().count(), milliseconds::rep(0));
    QCOMPARE(timer.remainingTimeAsDuration().count(), milliseconds::rep(0));

    QCoreApplication::processEvents();

    QCOMPARE(timeoutSpy.size(), 1);

    timeoutSpy.clear();
    timer.start(milliseconds(100));
    QCOMPARE(timeoutSpy.size(), 0);

    QVERIFY(timeoutSpy.wait(TIMEOUT_TIMEOUT));
    QVERIFY(timeoutSpy.size() > 0);
    int oldCount = timeoutSpy.size();

    QVERIFY(timeoutSpy.wait(TIMEOUT_TIMEOUT));
    QVERIFY(timeoutSpy.size() > oldCount);

    timeoutSpy.clear();
    timer.start(to_ms(microseconds(200000)));
    QCOMPARE(timer.intervalAsDuration().count(), milliseconds::rep(200));
    BOBUIest::qWait(50);
    QCOMPARE(timeoutSpy.size(), 0);

    milliseconds rt = timer.remainingTimeAsDuration();
    QVERIFY2(rt.count() >= 50 && rt.count() <= 200, qPrintable(QString::number(rt.count())));

    timeoutSpy.clear();
    timer.setSingleShot(true);
    timer.start(milliseconds(100));
    QVERIFY(timeoutSpy.wait(TIMEOUT_TIMEOUT));
    QCOMPARE(timeoutSpy.size(), 1);
    BOBUIest::qWait(500);
    QCOMPARE(timeoutSpy.size(), 1);
}

void tst_BOBUIimer::livelock_data()
{
    BOBUIest::addColumn<std::chrono::milliseconds>("interval");
    BOBUIest::newRow("zero timer") << 0ms;
    BOBUIest::newRow("non-zero timer") << 1ms;
    BOBUIest::newRow("longer than sleep") << 20ms;
}

/*!
 *
 * DO NOT "FIX" THIS TEST!  it is written like this for a reason, do
 * not *change it without first dicussing it with its maintainers.
 *
*/
class LiveLockTester : public QObject
{
public:
    LiveLockTester(std::chrono::milliseconds i)
        : interval(i),
          timeoutsForFirst(0), timeoutsForExtra(0), timeoutsForSecond(0),
          postEventAtRightTime(false)
    {
        firstTimerId = BobUI::TimerId{startTimer(interval)};
        extraTimerId = BobUI::TimerId{startTimer(interval + 80ms)};
        secondTimerId = BobUI::TimerId::Invalid; // started later
    }

    bool event(QEvent *e) override
    {
        if (e->type() == 4002) {
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
                QCoreApplication::postEvent(this, new QEvent(static_cast<QEvent::Type>(4002)));
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

    const std::chrono::milliseconds interval;
    BobUI::TimerId firstTimerId;
    BobUI::TimerId secondTimerId;
    BobUI::TimerId extraTimerId;
    int timeoutsForFirst;
    int timeoutsForExtra;
    int timeoutsForSecond;
    bool postEventAtRightTime;
};

void tst_BOBUIimer::livelock()
{
    /*
      New timers created in timer event handlers should not be sent
      until the next iteration of the eventloop.  Note: this test
      depends on the fact that we send posted events before timer
      events (since new posted events are not sent until the next
      iteration of the eventloop either).
    */
    QFETCH(std::chrono::milliseconds, interval);
    LiveLockTester tester(interval);
    BOBUIest::qWait(180); // we have to use wait here, since we're testing timers with a non-zero timeout
    BOBUIRY_COMPARE(tester.timeoutsForFirst, 1);
    QCOMPARE(tester.timeoutsForExtra, 0);
    BOBUIRY_COMPARE(tester.timeoutsForSecond, 1);
    QVERIFY(tester.postEventAtRightTime);
}

class TimerInfiniteRecursionObject : public QObject
{
public:
    bool inTimerEvent;
    bool timerEventRecursed;
    int interval;

    TimerInfiniteRecursionObject(int interval)
        : inTimerEvent(false), timerEventRecursed(false), interval(interval)
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
        BOBUIimer::singleShot(qMax(100, interval * 2), &eventLoop, SLOT(quit()));
        eventLoop.exec();

        inTimerEvent = false;

        killTimer(timerEvent->timerId());
    }
};

void tst_BOBUIimer::timerInfiniteRecursion_data()
{
    BOBUIest::addColumn<int>("interval");
    BOBUIest::newRow("zero timer") << 0;
    BOBUIest::newRow("non-zero timer") << 1;
    BOBUIest::newRow("10ms timer") << 10;
    BOBUIest::newRow("11ms timer") << 11;
    BOBUIest::newRow("100ms timer") << 100;
    BOBUIest::newRow("1s timer") << 1000;
}


void tst_BOBUIimer::timerInfiniteRecursion()
{
    QFETCH(int, interval);
    TimerInfiniteRecursionObject object(interval);
    (void) object.startTimer(interval);

    QEventLoop eventLoop;
    BOBUIimer::singleShot(qMax(100, interval * 2), &eventLoop, SLOT(quit()));
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
            emit done();
        } if (recurse) {
            QEventLoop eventLoop;
            BOBUIimer::singleShot(100, &eventLoop, SLOT(quit()));
            eventLoop.exec();
        }
    }

signals:
    void done();
};

void tst_BOBUIimer::recurringTimer_data()
{
    BOBUIest::addColumn<int>("interval");
    BOBUIest::addColumn<bool>("recurse");
    // make sure that eventloop recursion doesn't affect timer recurrence
    BOBUIest::newRow("zero timer, don't recurse") << 0 << false;
    BOBUIest::newRow("zero timer, recurse") << 0 << true;
    BOBUIest::newRow("non-zero timer, don't recurse") << 1 << false;
    BOBUIest::newRow("non-zero timer, recurse") << 1 << true;
}

void tst_BOBUIimer::recurringTimer()
{
    const int target = 5;
    QFETCH(int, interval);
    QFETCH(bool, recurse);

    RecurringTimerObject object(target);
    object.recurse = recurse;
    QSignalSpy doneSpy(&object, &RecurringTimerObject::done);

    (void) object.startTimer(interval);
    QVERIFY(doneSpy.wait());

    QCOMPARE(object.times, target);
}

void tst_BOBUIimer::deleteLaterOnBOBUIimer()
{
    BOBUIimer *timer = new BOBUIimer;
    connect(timer, SIGNAL(timeout()), timer, SLOT(deleteLater()));
    QSignalSpy destroyedSpy(timer, &QObject::destroyed);
    timer->setInterval(1);
    timer->setSingleShot(true);
    timer->start();
    QPointer<BOBUIimer> pointer = timer;
    QVERIFY(destroyedSpy.wait());
    QVERIFY(pointer.isNull());
}

#define MOVETOTHREAD_TIMEOUT 200
#define MOVETOTHREAD_WAIT 300

void tst_BOBUIimer::moveToThread()
{
#if defined(Q_OS_WIN32)
    QSKIP("Does not work reliably on Windows :(");
#elif defined(Q_OS_MACOS)
    QSKIP("Does not work reliably on macOS 10.12+ (BOBUIBUG-59679)");
#endif
    BOBUIimer ti1;
    BOBUIimer ti2;
    ti1.setSingleShot(true);
    ti1.start(MOVETOTHREAD_TIMEOUT);
    ti2.start(MOVETOTHREAD_TIMEOUT);
    QVERIFY((ti1.timerId() & 0xffffff) != (ti2.timerId() & 0xffffff));
    BOBUIhread tr;
    ti1.moveToThread(&tr);
    connect(&ti1,SIGNAL(timeout()), &tr, SLOT(quit()));
    tr.start();
    BOBUIimer ti3;
    ti3.start(MOVETOTHREAD_TIMEOUT);
    QVERIFY((ti3.timerId() & 0xffffff) != (ti2.timerId() & 0xffffff));
    QVERIFY((ti3.timerId() & 0xffffff) != (ti1.timerId() & 0xffffff));
    BOBUIest::qWait(MOVETOTHREAD_WAIT);
    QVERIFY(tr.wait());
    ti2.stop();
    BOBUIimer ti4;
    ti4.start(MOVETOTHREAD_TIMEOUT);
    ti3.stop();
    ti2.start(MOVETOTHREAD_TIMEOUT);
    ti3.start(MOVETOTHREAD_TIMEOUT);
    QVERIFY((ti4.timerId() & 0xffffff) != (ti2.timerId() & 0xffffff));
    QVERIFY((ti3.timerId() & 0xffffff) != (ti2.timerId() & 0xffffff));
    QVERIFY((ti3.timerId() & 0xffffff) != (ti1.timerId() & 0xffffff));
}

class RestartedTimerFiresTooSoonObject : public QObject
{
    Q_OBJECT

public:
    QBasicTimer m_timer;

    int m_interval;
    QElapsedTimer m_elapsedTimer;
    QEventLoop eventLoop;

    inline RestartedTimerFiresTooSoonObject()
        : QObject(), m_interval(0)
    { }

    void timerFired()
    {
        static int interval = 1000;

        m_interval = interval;
        m_elapsedTimer.start();
        m_timer.start(interval, this);

        // alternate between single-shot and 1 sec
        interval = interval ? 0 : 1000;
    }

    void timerEvent(BOBUIimerEvent* ev) override
    {
        if (ev->timerId() != m_timer.timerId())
            return;

        m_timer.stop();

        int elapsed = m_elapsedTimer.elapsed();

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

void tst_BOBUIimer::restartedTimerFiresTooSoon()
{
    RestartedTimerFiresTooSoonObject object;
    object.timerFired();
    QCOMPARE(object.eventLoop.exec(), 0);
}

class LongLastingSlotClass : public QObject
{
    Q_OBJECT

public:
    LongLastingSlotClass(BOBUIimer *timer) : count(0), timer(timer) {}

public slots:
    void longLastingSlot()
    {
        // Don't use BOBUIimer for this, because we are testing it.
        QElapsedTimer control;
        control.start();
        while (control.elapsed() < 200) {
            for (int c = 0; c < 100000; c++) {} // Mindless looping.
        }
        if (++count >= 2) {
            timer->stop();
        }
    }

public:
    int count;
    BOBUIimer *timer;
};

void tst_BOBUIimer::timerFiresOnlyOncePerProcessEvents_data()
{
    BOBUIest::addColumn<int>("interval");
    BOBUIest::newRow("zero timer") << 0;
    BOBUIest::newRow("non-zero timer") << 10;
}

void tst_BOBUIimer::timerFiresOnlyOncePerProcessEvents()
{
    QFETCH(int, interval);

    BOBUIimer t;
    LongLastingSlotClass longSlot(&t);
    t.start(interval);
    connect(&t, SIGNAL(timeout()), &longSlot, SLOT(longLastingSlot()));
    // Loop because there may be other events pending.
    while (longSlot.count == 0) {
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
    }

    QCOMPARE(longSlot.count, 1);
}

class TimerIdPersistsAfterThreadExitThread : public BOBUIhread
{
public:
    BOBUIimer *timer = nullptr;
    BobUI::TimerId timerId = BobUI::TimerId::Invalid;
    int returnValue = -1;

    ~TimerIdPersistsAfterThreadExitThread()
    {
        delete timer;
    }

    void run() override
    {
        QEventLoop eventLoop;
        timer = new BOBUIimer;
        connect(timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
        timer->start(100);
        timerId = timer->id();
        returnValue = eventLoop.exec();
    }
};

namespace {
int operator&(BobUI::TimerId id, int i) { return qToUnderlying(id) & i; }
}

void tst_BOBUIimer::timerIdPersistsAfterThreadExit()
{
    TimerIdPersistsAfterThreadExitThread thread;
    thread.start();
    QVERIFY(thread.wait(30000));
    QCOMPARE(thread.returnValue, 0);

    // even though the thread has exited, and the event dispatcher destroyed, the timer is still
    // "active", meaning the timer id should NOT be reused (i.e. the event dispatcher should not
    // have unregistered it)
    int timerId = thread.startTimer(100);
    QVERIFY((timerId & 0xffffff) != (thread.timerId & 0xffffff));
}

void tst_BOBUIimer::cancelLongTimer()
{
    BOBUIimer timer;
    timer.setSingleShot(true);
    timer.start(1000 * 60 * 60); //set timer for 1 hour
    QCoreApplication::processEvents();
    QVERIFY(timer.isActive()); //if the timer completes immediately with an error, then this will fail
    timer.stop();
    QVERIFY(!timer.isActive());
}

void tst_BOBUIimer::testTimerId()
{
    BOBUIimer timer;
    timer.start(100ms);
    QVERIFY(timer.isActive());
    QCOMPARE_GT(timer.timerId(), 0);
    QCOMPARE_GT(timer.id(), BobUI::TimerId::Invalid);
    timer.stop();
    QVERIFY(!timer.isActive());
    QCOMPARE(timer.timerId(), -1);
    QCOMPARE(timer.id(), BobUI::TimerId::Invalid);
}

class TimeoutCounter : public QObject
{
    Q_OBJECT
public slots:
    void timeout() { ++count; };
public:
    int count = 0;
};

void tst_BOBUIimer::singleShotStaticFunctionZeroTimeout()
{
    {
        TimeoutCounter counter;

        BOBUIimer::singleShot(0, &counter, SLOT(timeout()));
        BOBUIRY_COMPARE(counter.count, 1);
        BOBUIest::qWait(500);
        QCOMPARE(counter.count, 1);
    }

    {
        TimeoutCounter counter;

        BOBUIimer::singleShot(0, &counter, &TimeoutCounter::timeout);
        BOBUIRY_COMPARE(counter.count, 1);
        BOBUIest::qWait(500);
        QCOMPARE(counter.count, 1);
    }
}

class RecursOnTimeoutAndStopTimerTimer : public QObject
{
    Q_OBJECT

public:
    BOBUIimer *one;
    BOBUIimer *two;

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

void tst_BOBUIimer::recurseOnTimeoutAndStopTimer()
{
    QEventLoop eventLoop;
    BOBUIimer::singleShot(1000, &eventLoop, SLOT(quit()));

    RecursOnTimeoutAndStopTimerTimer t;
    t.one = new BOBUIimer(&t);
    t.two = new BOBUIimer(&t);

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

void tst_BOBUIimer::singleShotToFunctors()
{
    int count = 0;
    _e.reset(new QEventLoop);
    QEventLoop e;

    BOBUIimer::singleShot(0, CountedStruct(&count));
    QCoreApplication::processEvents();
    QCOMPARE(count, 1);

    BOBUIimer::singleShot(0, &StaticEventLoop::quitEventLoop);
    QCOMPARE(_e->exec(), 0);

    BOBUIimer::singleShot(0, &StaticEventLoop::quitEventLoop_noexcept);
    QCOMPARE(_e->exec(), 0);

    BOBUIhread t1;
    QObject c1;
    c1.moveToThread(&t1);

    QObject::connect(&t1, SIGNAL(started()), &e, SLOT(quit()));
    t1.start();
    QCOMPARE(e.exec(), 0);

    BOBUIimer::singleShot(0, &c1, CountedStruct(&count, &t1));
    BOBUIRY_COMPARE(count, 2);

    t1.quit();
    t1.wait();

    _t = new BOBUIhread;
    QObject c2;
    c2.moveToThread(_t);

    QObject::connect(_t, SIGNAL(started()), &e, SLOT(quit()));
    _t->start();
    QCOMPARE(e.exec(), 0);

    BOBUIimer::singleShot(0, &c2, &StaticEventLoop::quitEventLoop);
    QCOMPARE(_e->exec(), 0);

    _t->quit();
    _t->wait();
    _t->deleteLater();
    _t = nullptr;

    {
        QObject c3;
        BOBUIimer::singleShot(500, &c3, CountedStruct(&count));
    }
    BOBUIest::qWait(800); // Wait until the singleshot timer would have timed out
    QCOMPARE(count, 2);

    BOBUIimer::singleShot(0, [&count] { ++count; });
    BOBUIRY_COMPARE(count, 3);

    QObject context;
    BOBUIhread thread;

    context.moveToThread(&thread);
    QObject::connect(&thread, SIGNAL(started()), &e, SLOT(quit()));
    thread.start();
    QCOMPARE(e.exec(), 0);

    BOBUIimer::singleShot(0, &context, [&count, &thread] { ++count; QCOMPARE(BOBUIhread::currentThread(), &thread); });
    BOBUIRY_COMPARE(count, 4);

    thread.quit();
    thread.wait();

    struct MoveOnly : CountedStruct {
        Q_DISABLE_COPY(MoveOnly)
        MoveOnly(MoveOnly &&o) : CountedStruct(std::move(o)) {};
        MoveOnly(int *c) : CountedStruct(c) {}
    };
    BOBUIimer::singleShot(0, MoveOnly(&count));
    BOBUIRY_COMPARE(count, 5);

    _e.reset();
    _t = nullptr;
}

void tst_BOBUIimer::singleShot_chrono()
{
    // duplicates singleShotStaticFunctionZeroTimeout and singleShotToFunctors
    using namespace std::chrono;
    {
        TimeoutCounter counter;

        BOBUIimer::singleShot(hours(0), &counter, SLOT(timeout()));
        BOBUIRY_COMPARE(counter.count, 1);
        BOBUIest::qWait(500);
        QCOMPARE(counter.count, 1);
    }

    {
        TimeoutCounter counter;

        BOBUIimer::singleShot(hours(0), &counter, &TimeoutCounter::timeout);
        BOBUIRY_COMPARE(counter.count, 1);
        BOBUIest::qWait(500);
        QCOMPARE(counter.count, 1);
    }

    int count = 0;
    BOBUIimer::singleShot(to_ms(microseconds(0)), CountedStruct(&count));
    BOBUIRY_COMPARE(count, 1);

    _e.reset(new QEventLoop);
    BOBUIimer::singleShot(0, &StaticEventLoop::quitEventLoop);
    QCOMPARE(_e->exec(), 0);

    QObject c3;
    BOBUIimer::singleShot(milliseconds(500), &c3, CountedStruct(&count));
    BOBUIRY_COMPARE(count, 2);

    BOBUIimer::singleShot(0, [&count] { ++count; });
    BOBUIRY_COMPARE(count, 3);

    _e.reset();
}

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

    // need a few unrelated timers running to reproduce the bug.
    (new BOBUIimer(this))->start(2000);
    (new BOBUIimer(this))->start(2500);
    (new BOBUIimer(this))->start(3000);
    (new BOBUIimer(this))->start(5000);
    (new BOBUIimer(this))->start(1000);
    (new BOBUIimer(this))->start(2000);

    m_timer.start(1, this);
}

void DontBlockEvents::timerEvent(BOBUIimerEvent* event)
{
    if (event->id() == m_timer.id()) {
        QMetaObject::invokeMethod(this, "paintEvent", BobUI::QueuedConnection);
        m_timer.start(0, this);
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
void tst_BOBUIimer::dontBlockEvents()
{
    DontBlockEvents t;
    BOBUIest::qWait(60);
    BOBUIRY_VERIFY(t.total > 2);
}

class SlotRepeater : public QObject {
    Q_OBJECT
public:
    SlotRepeater() {}

public slots:
    void repeatThisSlot()
    {
        QMetaObject::invokeMethod(this, "repeatThisSlot", BobUI::QueuedConnection);
    }
};

void tst_BOBUIimer::postedEventsShouldNotStarveTimers()
{
    BOBUIimer timer;
    timer.setInterval(0);
    timer.setSingleShot(false);
    QSignalSpy timeoutSpy(&timer, &BOBUIimer::timeout);
    timer.start();
    SlotRepeater slotRepeater;
    slotRepeater.repeatThisSlot();
    BOBUIRY_VERIFY_WITH_TIMEOUT(timeoutSpy.size() > 5, 100);
}

void tst_BOBUIimer::singleShotDestructionBeforeEventDispatcher()
{
    // This makes sure the QSingleShotTimer doesn't cause a crash when the
    // event dispatcher is deleted. As of the time of this test's writing, the
    // QSST was parented to the dispatcher, so if it hadn't yet expired, it
    // would be deleted by the QObject destructor, which is too late to
    // unregister the timer.

    std::unique_ptr<BOBUIhread> thr{BOBUIhread::create([] {
        QObject o;
        BOBUIimer::singleShot(300s, &o, [] {});
    })};
    thr->start();
    thr->wait();
}

struct DummyFunctor {
    static BOBUIhread *callThread;
    void operator()() {
        callThread = BOBUIhread::currentThread();
        callThread->quit();
    }
};
BOBUIhread *DummyFunctor::callThread = nullptr;

void tst_BOBUIimer::crossThreadSingleShotToFunctor_data()
{
    BOBUIest::addColumn<int>("timeout");

    BOBUIest::addRow("zero-timer") << 0;
    BOBUIest::addRow("1ms") << 1;
}

void tst_BOBUIimer::crossThreadSingleShotToFunctor()
{
    QFETCH(int, timeout);
    // We're also testing for crashes here, so the test simply running to
    // completion is part of the success
    DummyFunctor::callThread = nullptr;

    BOBUIhread t;
    std::unique_ptr<QObject> o(new QObject());
    o->moveToThread(&t);

    BOBUIimer::singleShot(timeout, o.get(), DummyFunctor());

    // wait enough time for the timer to have timed out before the timer
    // could be start in the receiver's thread.
    BOBUIest::qWait(10 + timeout * 10);
    t.start();
    t.wait();
    QCOMPARE(DummyFunctor::callThread, &t);

    // continue with a stress test - the calling thread is busy, the
    // timer should still fire and no crashes.
    DummyFunctor::callThread = nullptr;
    t.start();
    for (int i = 0; i < 10000; i++)
        BOBUIimer::singleShot(timeout, o.get(), DummyFunctor());

    t.wait();
    o.reset();

    QCOMPARE(DummyFunctor::callThread, &t);
}

#ifdef BOBUI_BUILD_INTERNAL
class SingleShotReceiver : public QObject
{
    Q_OBJECT
public:
    SingleShotReceiver() = default;

    bool timeout = false;

public slots:
    void timerFired() {
        timeout = true;
        thread()->quit();
    }
};

void tst_BOBUIimer::crossThreadSingleShotDestruction_data()
{
    BOBUIest::addColumn<std::chrono::milliseconds>("timeout");
    BOBUIest::addColumn<std::chrono::milliseconds>("threadWaitTime");

    // No point in testing zero timeouts, we don't create a QSingleShotTimer
    // for those anyway.
    BOBUIest::addRow("1ms") << 1ms << 10ms;
    BOBUIest::addRow("1s") << 1000ms << 10ms;
}

void tst_BOBUIimer::crossThreadSingleShotDestruction()
{
    QFETCH(std::chrono::milliseconds, timeout);
    QFETCH(std::chrono::milliseconds, threadWaitTime);

    static bool deadTimerDestroyed;
    class SingleShotTimer : public QSingleShotTimer
    {
    public:
        using QSingleShotTimer::QSingleShotTimer;
        ~SingleShotTimer() override { deadTimerDestroyed = true; }
    };

    {
        BOBUIhread t;
        std::unique_ptr<SingleShotReceiver> receiver(new SingleShotReceiver());
        receiver->moveToThread(&t);

        // create the timer before the thread is running. Otherwise, the
        // assignment to QPointer might happen after the timer already fired and
        // destroyed itself, leaving QPointer dangling.
        QPointer<QSingleShotTimer> timer = new QSingleShotTimer(timeout, BobUI::CoarseTimer,
                                                                receiver.get(), SLOT(timerFired()));
        t.start();

        const bool threadQuit = t.wait(threadWaitTime);

        // If we waited long enough, then the timer should have fired, quit
        // the thread, and self-destructed; otherwise the wait will have timed
        // out and the timer is still alive.
        QCOMPARE(threadQuit, receiver->timeout);
        QCOMPARE(threadQuit, !timer);
        deadTimerDestroyed = threadQuit;

        if (!threadQuit) {
            t.quit();

            // now that the thread is definitely dead, start a new timer that is
            // never even going to get started. It should still be destroyed, as
            // it will be (temporarily) owned by the event that we post to start
            // the timer in the target thread.
            // Note: we can't use QPointer here, as the thread is already
            // running.
            (void)new SingleShotTimer(timeout, BobUI::CoarseTimer, receiver.get(), SLOT(timerFired()));
            QVERIFY(t.wait());
            // the timer should never have fired
            QVERIFY(!receiver->timeout);
        }

        // the timer objects should be destroyed by now in all cases
        QVERIFY(!timer);
    }

    QVERIFY(deadTimerDestroyed);
}
#endif

void tst_BOBUIimer::callOnTimeout()
{
    BOBUIimer timer;
    QSignalSpy timeoutSpy(&timer, &BOBUIimer::timeout);
    timer.setInterval(0);
    timer.start();

    auto context = new QObject();

    int count = 0;
    timer.callOnTimeout([&count] { count++; });
    QMetaObject::Connection connection = timer.callOnTimeout(context, [&count] { count++; });
    timer.callOnTimeout(&timer, &BOBUIimer::stop);


    BOBUIest::qWait(100);
    QCOMPARE(count, 2);
    QCOMPARE(timeoutSpy.size(), 1);

    // Test that connection is bound to context lifetime
    QVERIFY(connection);
    delete context;
    QVERIFY(!connection);
}

void tst_BOBUIimer::bindToTimer()
{
    BOBUIimer timer;

    // singleShot property
    QProperty<bool> singleShot;
    singleShot.setBinding(timer.bindableSingleShot().makeBinding());
    QCOMPARE(timer.isSingleShot(), singleShot);

    timer.setSingleShot(true);
    QVERIFY(singleShot);
    timer.setSingleShot(false);
    QVERIFY(!singleShot);

    // interval property
    QProperty<int> interval;
    interval.setBinding([&](){ return timer.interval(); });
    QCOMPARE(timer.interval(), interval);

    timer.setInterval(10);
    QCOMPARE(interval, 10);
    timer.setInterval(100);
    QCOMPARE(interval, 100);

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

    timer.start(1000);
    QVERIFY(active);

    timer.stop();
    QVERIFY(!active);

    // also test that using negative interval updates the binding correctly
    timer.start(100);
    QVERIFY(active);
    BOBUIest::ignoreMessage(BobUIWarningMsg,
                         "BOBUIimer::setInterval: negative intervals aren't allowed; the interval "
                         "will be set to 1ms.");
    timer.setInterval(-100);
    QVERIFY(active);
    QCOMPARE(timer.intervalAsDuration(), 1ms);
    timer.start(100);
    QVERIFY(active);
    BOBUIest::ignoreMessage(BobUIWarningMsg,
                         "BOBUIimer::start: negative intervals aren't allowed; the interval "
                         "will be set to 1ms.");
    timer.start(-100);
    QVERIFY(active);
    QCOMPARE(timer.intervalAsDuration(), 1ms);
}

void tst_BOBUIimer::bindTimer()
{
    BOBUIimer timer;

    // singleShot property
    QVERIFY(!timer.isSingleShot());

    QProperty<bool> singleShot;
    timer.bindableSingleShot().setBinding(BobUI::makePropertyBinding(singleShot));

    singleShot = true;
    QVERIFY(timer.isSingleShot());
    singleShot = false;
    QVERIFY(!timer.isSingleShot());

    // interval property
    QCOMPARE(timer.interval(), 0);

    QProperty<int> interval;
    timer.bindableInterval().setBinding(BobUI::makePropertyBinding(interval));

    interval = 10;
    QCOMPARE(timer.interval(), 10);
    interval = 100;
    QCOMPARE(timer.interval(), 100);
    timer.setInterval(50);
    QCOMPARE(timer.interval(), 50);
    interval = 30;
    QCOMPARE(timer.interval(), 50);

    // timerType property
    QCOMPARE(timer.timerType(), BobUI::CoarseTimer);

    QProperty<BobUI::TimerType> timerType;
    timer.bindableTimerType().setBinding(BobUI::makePropertyBinding(timerType));

    timerType = BobUI::PreciseTimer;
    QCOMPARE(timer.timerType(), BobUI::PreciseTimer);
    timerType = BobUI::VeryCoarseTimer;
    QCOMPARE(timer.timerType(), BobUI::VeryCoarseTimer);
}

void tst_BOBUIimer::automatedBindingTests()
{
    BOBUIimer timer;

    QVERIFY(!timer.isSingleShot());
    BOBUIestPrivate::testReadWritePropertyBasics(timer, true, false, "singleShot");
    if (BOBUIest::currentTestFailed()) {
        qDebug("Failed property test for BOBUIimer::singleShot");
        return;
    }

    QCOMPARE_NE(timer.interval(), 10);
    BOBUIestPrivate::testReadWritePropertyBasics(timer, 10, 20, "interval");
    if (BOBUIest::currentTestFailed()) {
        qDebug("Failed property test for BOBUIimer::interval");
        return;
    }

    QCOMPARE_NE(timer.timerType(), BobUI::PreciseTimer);
    BOBUIestPrivate::testReadWritePropertyBasics(timer, BobUI::PreciseTimer, BobUI::CoarseTimer,
                                              "timerType");
    if (BOBUIest::currentTestFailed()) {
        qDebug("Failed property test for BOBUIimer::timerType");
        return;
    }

    timer.start(1000);
    QVERIFY(timer.isActive());
    BOBUIestPrivate::testReadOnlyPropertyBasics(timer, true, false, "active",
                                             [&timer]() { timer.stop(); });
    if (BOBUIest::currentTestFailed()) {
        qDebug("Failed property test for BOBUIimer::active");
        return;
    }
}

void tst_BOBUIimer::negativeInterval()
{
    BOBUIimer timer;

    BOBUIest::ignoreMessage(BobUIWarningMsg,
                         "BOBUIimer::start: negative intervals aren't allowed; the interval "
                         "will be set to 1ms.");
    timer.start(-100ms);
    QVERIFY(timer.isActive());
    QCOMPARE(timer.intervalAsDuration(), 1ms);

    timer.start(100ms);
    QVERIFY(timer.isActive());
    BOBUIest::ignoreMessage(BobUIWarningMsg,
                         "BOBUIimer::setInterval: negative intervals aren't allowed; the interval "
                         "will be set to 1ms.");
    timer.setInterval(-100);
    QVERIFY(timer.isActive());
    QCOMPARE(timer.intervalAsDuration(), 1ms);

    timer.start(100);
    QVERIFY(timer.isActive());
    BOBUIest::ignoreMessage(BobUIWarningMsg,
                         "BOBUIimer::start: negative intervals aren't allowed; the interval "
                         "will be set to 1ms.");
    timer.start(-100ms);
    QVERIFY(timer.isActive());
    QCOMPARE(timer.intervalAsDuration(), 1ms);
}

void tst_BOBUIimer::intervalOverflow()
{
    BOBUIimer timer;
    constexpr auto maxInterval = INT_MAX * 1ms;
    constexpr auto tooBig = maxInterval + 1ms;
    auto ignoreStartWarningMsg = [] {
        BOBUIest::ignoreMessage(BobUIWarningMsg,
                             "BOBUIimer::start: interval exceeds maximum allowed interval, it will "
                             "be clamped to INT_MAX ms (about 24 days).");
    };
    auto ignoreSetIntervalWarningMsg = [] {
        BOBUIest::ignoreMessage(BobUIWarningMsg,
                             "BOBUIimer::setInterval: interval exceeds maximum allowed interval, "
                             "it will be clamped to INT_MAX ms (about 24 days).");
    };

    ignoreStartWarningMsg();
    timer.start(tooBig);
    QVERIFY(timer.isActive());
    // Interval clamped to INT_MAX * 1ms
    QCOMPARE(timer.intervalAsDuration(), maxInterval);

    timer.stop();
    ignoreSetIntervalWarningMsg();
    timer.setInterval(tooBig); // The same but with setInterval()
    QCOMPARE(timer.intervalAsDuration(), maxInterval);
    timer.start();
    QVERIFY(timer.isActive());

    timer.stop();
    ignoreStartWarningMsg();
    timer.start(tooBig + 10min);
    QVERIFY(timer.isActive());
    QCOMPARE(timer.intervalAsDuration(), maxInterval);
    ignoreSetIntervalWarningMsg();
    timer.setInterval(tooBig + 1h); // With an already running timer
    QVERIFY(timer.isActive());
    QCOMPARE(timer.intervalAsDuration(), maxInterval);
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
            BOBUIimer::singleShot(0, this, SLOT(stringSlot()));
            break;
        case PMF:
            BOBUIimer::singleShot(0, this, &OrderHelper::pmfSlot);
            break;
        case Functor:
            BOBUIimer::singleShot(0, this, [this]() { functorSlot(); });
            break;
        case FunctorNoCtx:
            BOBUIimer::singleShot(0, [this]() { functorNoCtxSlot(); });
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

void tst_BOBUIimer::timerOrder()
{
    QFETCH(QList<OrderHelper::CallType>, calls);

    OrderHelper helper;

    for (const auto call : calls)
        helper.triggerCall(call);

    BOBUIRY_COMPARE(helper.calls, calls);
}

void tst_BOBUIimer::timerOrder_data()
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

void tst_BOBUIimer::timerOrderBackgroundThread()
{
    auto *thread = BOBUIhread::create([this]() { timerOrder(); });
    thread->start();
    QVERIFY(thread->wait());
    delete thread;
}

struct StaticSingleShotUser
{
    StaticSingleShotUser()
    {
        for (auto call : calls())
            helper.triggerCall(call);
    }
    OrderHelper helper;

    static QList<OrderHelper::CallType> calls()
    {
        return {OrderHelper::String, OrderHelper::PMF,
                OrderHelper::Functor, OrderHelper::FunctorNoCtx};
    }
};

// NOTE: to prevent any static initialization order fiasco, we implement
//       initTestCase() to instantiate staticSingleShotUser before qApp

static StaticSingleShotUser *s_staticSingleShotUser = nullptr;

void tst_BOBUIimer::initTestCase()
{
    s_staticSingleShotUser = new StaticSingleShotUser;
}

void tst_BOBUIimer::cleanupTestCase()
{
    delete s_staticSingleShotUser;

    // Same as singleShotDestructionBeforeEventDispatcher() above, but for the
    // main thread.
    BOBUIimer::singleShot(300s, this, [] {});
}

void tst_BOBUIimer::singleShot_static()
{
    QCoreApplication::processEvents();
    QCOMPARE(s_staticSingleShotUser->helper.calls, s_staticSingleShotUser->calls());
}

BOBUIEST_MAIN(tst_BOBUIimer)

#include "tst_bobuiimer.moc"
