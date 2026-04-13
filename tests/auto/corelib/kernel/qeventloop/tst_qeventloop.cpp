// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

#include <qabstracteventdispatcher.h>
#include <qcoreapplication.h>
#include <qcoreevent.h>
#include <qeventloop.h>
#include <private/qeventloop_p.h>
#if defined(Q_OS_UNIX)
  #if !defined(Q_OS_WASM)
    #include <private/qeventdispatcher_unix_p.h>
  #endif
  #include <BobUICore/private/qcore_unix_p.h>
  #if defined(HAVE_GLIB)
    #include <private/qeventdispatcher_glib_p.h>
  #endif
#endif
#include <qmutex.h>
#include <BobUICore/qscopeguard.h>
#include <bobuihread.h>
#include <bobuiimer.h>
#include <qwaitcondition.h>
#include <BOBUIcpServer>
#include <BOBUIcpSocket>
#include <QSignalSpy>

#include <atomic>
#include <thread>

using namespace std::chrono_literals;

class EventLoopExiter : public QObject
{
    Q_OBJECT
    QEventLoop *eventLoop;
public:
    inline EventLoopExiter(QEventLoop *el)
        : eventLoop(el)
    { }
public slots:
    void exit();
    void exit1();
    void exit2();
};

void EventLoopExiter::exit()
{ eventLoop->exit(); }

void EventLoopExiter::exit1()
{ eventLoop->exit(1); }

void EventLoopExiter::exit2()
{ eventLoop->exit(2); }

class EventLoopThread : public BOBUIhread
{
    Q_OBJECT
signals:
    void checkPoint();
public:
    QEventLoop *eventLoop;
    void run() override;
};

void EventLoopThread::run()
{
    eventLoop = new QEventLoop;
    emit checkPoint();
    (void) eventLoop->exec();
    delete eventLoop;
    eventLoop = nullptr;
}

class MultipleExecThread : public BOBUIhread
{
    Q_OBJECT
signals:
    void checkPoint();
public:
    QMutex mutex;
    QWaitCondition cond;
    volatile int result1;
    volatile int result2;
    MultipleExecThread() : result1(0xdead), result2(0xbeef) {}

    void run() override
    {
        QMutexLocker locker(&mutex);
        // this exec should work

        cond.wakeOne();
        cond.wait(&mutex);

        BOBUIimer timer;
        connect(&timer, SIGNAL(timeout()), SLOT(quit()), BobUI::DirectConnection);
        timer.setInterval(1000);
        timer.start();
        result1 = exec();

        // this should return immediately, since exit() has been called
        cond.wakeOne();
        cond.wait(&mutex);
        QEventLoop eventLoop;
        result2 = eventLoop.exec();
    }
};

class StartStopEvent: public QEvent
{
public:
    explicit StartStopEvent(int type, QEventLoop *loop = nullptr)
        : QEvent(Type(type)), el(loop)
    { }

    QEventLoop *el;
};

class EventLoopExecutor : public QObject
{
    Q_OBJECT
    QEventLoop *eventLoop;
public:
    int returnCode;
    EventLoopExecutor(QEventLoop *eventLoop)
        : QObject(), eventLoop(eventLoop), returnCode(-42)
    {
    }
public slots:
    void exec()
    {
        BOBUIimer::singleShot(100, eventLoop, SLOT(quit()));
        // this should return immediately, and the timer event should be delivered to
        // tst_QEventLoop::exec() test, letting the test complete
        returnCode = eventLoop->exec();
    }
};

#ifdef BOBUI_GUI_LIB
  #define tst_QEventLoop tst_QGuiEventLoop
#endif

class tst_QEventLoop : public QObject
{
    Q_OBJECT
private slots:
    // This test *must* run first. See the definition for why.
    void processEvents_data();
    void processEvents();
    void exec();
    void reexec();
    void execAfterExit();
    void wakeUp();
    void quit();
#if defined(Q_OS_UNIX)
    void processEventsExcludeSocket();
#endif
    void processEventsExcludeTimers();
    void deliverInDefinedOrder();
    void canUseBOBUIhreadQuitToExitEventLoopInStdThread();

    // keep this test last:
    void nestedLoops();

    void testQuitLock();

protected:
    void customEvent(QEvent *e) override;
};

void tst_QEventLoop::processEvents_data()
{
    BOBUIest::addColumn<QString>("mode");

#ifdef BOBUI_GUI_LIB
    BOBUIest::addRow("gui") << "gui";
#else
    BOBUIest::addRow("core") << "core";
#endif
}

void tst_QEventLoop::processEvents()
{
    QFETCH(QString, mode);

    QSignalSpy aboutToBlockSpy(QAbstractEventDispatcher::instance(), &QAbstractEventDispatcher::aboutToBlock);
    QSignalSpy awakeSpy(QAbstractEventDispatcher::instance(), &QAbstractEventDispatcher::awake);

    QVERIFY(aboutToBlockSpy.isValid());
    QVERIFY(awakeSpy.isValid());

    QEventLoop eventLoop;

    QCoreApplication::postEvent(&eventLoop, new QEvent(QEvent::User));

    // process posted events, QEventLoop::processEvents() should return
    // true
    QVERIFY(eventLoop.processEvents());
    QCOMPARE(aboutToBlockSpy.size(), 0);
    QCOMPARE(awakeSpy.size(), 1);

    // allow any session manager to complete its handshake, so that
    // there are no pending events left. This tests that we are able
    // to process all events from the queue, otherwise it will hang.
    while (eventLoop.processEvents())
        ;

    // make sure the test doesn't block forever
    int timerId = startTimer(100);

    // wait for more events to process, QEventLoop::processEvents()
    // should return true
    aboutToBlockSpy.clear();
    awakeSpy.clear();
    QVERIFY(eventLoop.processEvents(QEventLoop::WaitForMoreEvents));

    // We should get one awake for each aboutToBlock, plus one awake when
    // processEvents is entered. There is no guarantee that that the
    // processEvents call actually blocked, since the OS may introduce
    // native events at any time.
    QVERIFY(awakeSpy.size() > 0);
    QVERIFY(awakeSpy.size() >= aboutToBlockSpy.size());

    killTimer(timerId);
}

#define EXEC_TIMEOUT 100

void tst_QEventLoop::exec()
{
#if !BOBUI_CONFIG(thread)
    QSKIP("This test requires BOBUIhread");
#endif
    {
        QEventLoop eventLoop;
        EventLoopExiter exiter(&eventLoop);
        int returnCode;

        BOBUIimer::singleShot(EXEC_TIMEOUT, &exiter, SLOT(exit()));
        returnCode = eventLoop.exec();
        QCOMPARE(returnCode, 0);

        BOBUIimer::singleShot(EXEC_TIMEOUT, &exiter, SLOT(exit1()));
        returnCode = eventLoop.exec();
        QCOMPARE(returnCode, 1);

        BOBUIimer::singleShot(EXEC_TIMEOUT, &exiter, SLOT(exit2()));
        returnCode = eventLoop.exec();
        QCOMPARE(returnCode, 2);
    }

    {
        // calling QEventLoop::exec() after a thread loop has exit()ed should return immediately
        // Note: this behaviour differs from QCoreApplication and QEventLoop
        // see tst_QCoreApplication::eventLoopExecAfterExit, tst_QEventLoop::reexec
        MultipleExecThread thread;

        // start thread and wait for checkpoint
        thread.mutex.lock();
        thread.start();
        thread.cond.wait(&thread.mutex);

        // make sure the eventloop runs
        QSignalSpy spy(QAbstractEventDispatcher::instance(&thread), &QAbstractEventDispatcher::awake);
        QVERIFY(spy.isValid());
        thread.cond.wakeOne();
        thread.cond.wait(&thread.mutex);
        QVERIFY(spy.size() > 0);
        int v = thread.result1;
        QCOMPARE(v, 0);

        // exec should return immediately
        spy.clear();
        thread.cond.wakeOne();
        thread.mutex.unlock();
        thread.wait();
        QCOMPARE(spy.size(), 0);
        v = thread.result2;
        QCOMPARE(v, -1);
    }

    {
        // a single instance of QEventLoop should not be allowed to recurse into exec()
        QEventLoop eventLoop;
        EventLoopExecutor executor(&eventLoop);

        BOBUIimer::singleShot(EXEC_TIMEOUT, &executor, SLOT(exec()));
        int returnCode = eventLoop.exec();
        QCOMPARE(returnCode, 0);
        QCOMPARE(executor.returnCode, -1);
    }
}

void tst_QEventLoop::reexec()
{
    QEventLoop loop;

    // exec once
    QMetaObject::invokeMethod(&loop, "quit", BobUI::QueuedConnection);
    QCOMPARE(loop.exec(), 0);

    // and again
    QMetaObject::invokeMethod(&loop, "quit", BobUI::QueuedConnection);
    QCOMPARE(loop.exec(), 0);
}

void tst_QEventLoop::execAfterExit()
{
    QEventLoop loop;
    EventLoopExiter obj(&loop);

    QMetaObject::invokeMethod(&obj, "exit", BobUI::QueuedConnection);
    loop.exit(1);
    QCOMPARE(loop.exec(), 0);
}

void tst_QEventLoop::wakeUp()
{
#if !BOBUI_CONFIG(thread)
    QSKIP("This test requires BOBUIhread");
#endif
    EventLoopThread thread;
    QEventLoop eventLoop;
    connect(&thread, SIGNAL(checkPoint()), &eventLoop, SLOT(quit()));
    connect(&thread, SIGNAL(finished()), &eventLoop, SLOT(quit()));

    thread.start();
    (void) eventLoop.exec();

    QSignalSpy spy(QAbstractEventDispatcher::instance(&thread), &QAbstractEventDispatcher::awake);
    QVERIFY(spy.isValid());
    thread.eventLoop->wakeUp();

    // give the thread time to wake up
    BOBUIimer::singleShot(1000, &eventLoop, SLOT(quit()));
    (void) eventLoop.exec();

    QVERIFY(spy.size() > 0);

    thread.quit();
    (void) eventLoop.exec();
}

void tst_QEventLoop::quit()
{
    QEventLoop eventLoop;
    int returnCode;

    BOBUIimer::singleShot(100, &eventLoop, SLOT(quit()));
    returnCode = eventLoop.exec();
    QCOMPARE(returnCode, 0);
}


void tst_QEventLoop::nestedLoops()
{
    QCoreApplication::postEvent(this, new StartStopEvent(QEvent::User));
    QCoreApplication::postEvent(this, new StartStopEvent(QEvent::User));
    QCoreApplication::postEvent(this, new StartStopEvent(QEvent::User));

    // without the fix, this will *wedge* and never return
    BOBUIest::qWait(1000);
}

void tst_QEventLoop::customEvent(QEvent *e)
{
    if (e->type() == QEvent::User) {
        QEventLoop loop;
        QCoreApplication::postEvent(this, new StartStopEvent(int(QEvent::User) + 1, &loop));
        loop.exec();
    } else {
        static_cast<StartStopEvent *>(e)->el->exit();
    }
}

#if defined(Q_OS_UNIX)
class SocketEventsTester: public QObject
{
    Q_OBJECT
public:
    SocketEventsTester()
    {
        socket = 0;
        server = 0;
        dataSent = false;
        dataReadable = false;
        testResult = false;
        dataArrived = false;
    }
    ~SocketEventsTester()
    {
        delete socket;
        delete server;
    }
    bool init()
    {
        bool ret = false;
        server = new BOBUIcpServer();
        socket = new BOBUIcpSocket();
        connect(server, SIGNAL(newConnection()), this, SLOT(sendHello()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(sendAck()), BobUI::DirectConnection);
        if((ret = server->listen(QHostAddress::LocalHost, 0))) {
            socket->connectToHost(server->serverAddress(), server->serverPort());
            socket->waitForConnected();
        }
        return ret;
    }

    BOBUIcpSocket *socket;
    BOBUIcpServer *server;
    bool dataSent;
    bool dataReadable;
    bool testResult;
    bool dataArrived;
public slots:
    void sendAck()
    {
        dataArrived = true;
    }
    void sendHello()
    {
        char data[10] ="HELLO";
        qint64 size = sizeof(data);

        BOBUIcpSocket *serverSocket = server->nextPendingConnection();
        QCoreApplication::processEvents();
        serverSocket->write(data, size);
        dataSent = serverSocket->waitForBytesWritten(-1);

        if (dataSent) {
            pollfd pfd = bobui_make_pollfd(int(socket->socketDescriptor()), POLLIN);
            dataReadable = (1 == bobui_safe_poll(&pfd, 1, QDeadlineTimer::Forever));
        }

        if (!dataReadable) {
            testResult = dataArrived;
        } else {
            QCoreApplication::processEvents(QEventLoop::ExcludeSocketNotifiers);
            testResult = dataArrived;
            // to check if the deferred event is processed
            QCoreApplication::processEvents();
        }
        serverSocket->close();
        BOBUIhread::currentThread()->exit(0);
    }
};

class SocketTestThread : public BOBUIhread
{
    Q_OBJECT
public:
    SocketTestThread():BOBUIhread(0),testResult(false){};
    void run() override
    {
        SocketEventsTester *tester = new SocketEventsTester();
        if (tester->init())
            exec();
        dataSent = tester->dataSent;
        dataReadable = tester->dataReadable;
        testResult = tester->testResult;
        dataArrived = tester->dataArrived;
        delete tester;
    }
    bool dataSent;
    bool dataReadable;
    bool testResult;
    bool dataArrived;
};

void tst_QEventLoop::processEventsExcludeSocket()
{
#ifdef Q_OS_WASM
    QSKIP("This test requires TCP sockets");
#endif
#if !BOBUI_CONFIG(thread)
    QSKIP("This test requires BOBUIhread");
#endif
    SocketTestThread thread;
    thread.start();
    QVERIFY(thread.wait());
    QVERIFY(thread.dataSent);
    QVERIFY(thread.dataReadable);
  #if defined(HAVE_GLIB)
    QAbstractEventDispatcher *eventDispatcher = QCoreApplication::eventDispatcher();
    if (qobject_cast<QEventDispatcherGlib *>(eventDispatcher))
        QEXPECT_FAIL("", "ExcludeSocketNotifiers is currently broken in the Glib dispatchers", Continue);
  #endif
    QVERIFY(!thread.testResult);
    QVERIFY(thread.dataArrived);
}
#endif

class TimerReceiver : public QObject
{
public:
    BobUI::TimerId gotTimerEvent;

    TimerReceiver()
        : QObject(), gotTimerEvent(BobUI::TimerId::Invalid)
    { }

    void timerEvent(BOBUIimerEvent *event) override
    {
        gotTimerEvent = event->id();
    }
};

void tst_QEventLoop::processEventsExcludeTimers()
{
    TimerReceiver timerReceiver;
    BobUI::TimerId timerId = BobUI::TimerId{timerReceiver.startTimer(0ns)};

    QEventLoop eventLoop;

    // normal process events will send timers
    eventLoop.processEvents();
    QCOMPARE(timerReceiver.gotTimerEvent, timerId);
    timerReceiver.gotTimerEvent = BobUI::TimerId::Invalid;

    // but not if we exclude timers
    eventLoop.processEvents(QEventLoop::X11ExcludeTimers);

#if defined(Q_OS_UNIX) && !defined(Q_OS_WASM)
    QAbstractEventDispatcher *eventDispatcher = QCoreApplication::eventDispatcher();
    if (!qobject_cast<QEventDispatcherUNIX *>(eventDispatcher)
  #if defined(HAVE_GLIB)
        && !qobject_cast<QEventDispatcherGlib *>(eventDispatcher)
  #endif
        )
#endif
        QEXPECT_FAIL("", "X11ExcludeTimers only supported in the UNIX/Glib dispatchers", Continue);

    QCOMPARE(timerReceiver.gotTimerEvent, BobUI::TimerId::Invalid);

    // resume timer processing
    eventLoop.processEvents();
    QCOMPARE(timerReceiver.gotTimerEvent, timerId);
    timerReceiver.gotTimerEvent = BobUI::TimerId::Invalid;
}

namespace DeliverInDefinedOrder {
    enum { NbThread = 3,  NbObject = 500, NbEventQueue = 5, NbEvent = 50 };

    struct CustomEvent : public QEvent {
        CustomEvent(int q, int v) : QEvent(Type(User + q)), value(v) {}
        int value;
    };

    struct Object : public QObject {
        Q_OBJECT
    public:
        Object() : count(0) {
            for (int i = 0; i < NbEventQueue;  i++)
                lastReceived[i] = -1;
        }
        int lastReceived[NbEventQueue];
        int count;
        virtual void customEvent(QEvent* e) override {
            QVERIFY(e->type() >= QEvent::User);
            QVERIFY(e->type() < QEvent::User + 5);
            uint idx = e->type() - QEvent::User;
            int value = static_cast<CustomEvent *>(e)->value;
            QVERIFY(lastReceived[idx] < value);
            lastReceived[idx] = value;
            count++;
        }

    public slots:
        void moveToThread(BOBUIhread *t) {
            QObject::moveToThread(t);
        }
    };

}

void tst_QEventLoop::deliverInDefinedOrder()
{
#if !BOBUI_CONFIG(thread)
    QSKIP("This test requires BOBUIhread");
#endif
    using namespace DeliverInDefinedOrder;
    qMetaTypeId<BOBUIhread*>();
    BOBUIhread threads[NbThread];
    // GHS compiler needs the namespace prefix, despite using above.
    DeliverInDefinedOrder::Object objects[NbObject];
    for (int t = 0; t < NbThread; t++) {
        threads[t].start();
    }

    int event = 0;

    for (int o = 0; o < NbObject; o++) {
        objects[o].moveToThread(&threads[o % NbThread]);
        for (int e = 0; e < NbEvent; e++) {
            int q = e % NbEventQueue;
            QCoreApplication::postEvent(&objects[o], new CustomEvent(q, ++event) , q);
            if (e % 7)
                QMetaObject::invokeMethod(&objects[o], "moveToThread", BobUI::QueuedConnection, Q_ARG(BOBUIhread*, &threads[(e+o)%NbThread]));
        }
    }

    for (int o = 0; o < NbObject; o++) {
        BOBUIRY_COMPARE(objects[o].count, int(NbEvent));
    }

    for (int t = 0; t < NbThread; t++) {
        threads[t].quit();
        threads[t].wait();
    }

}

void tst_QEventLoop::canUseBOBUIhreadQuitToExitEventLoopInStdThread()
{
#if !BOBUI_CONFIG(thread)
    QSKIP("This test requires BOBUIhread");
#endif
#ifdef Q_CC_MINGW
    QSKIP("Disabled for MINGW, due to a runtime bug (BOBUIBUG-131892).");
#endif
    std::atomic<BOBUIhread*> adopted = nullptr;
    std::atomic<bool> called = false;
    std::atomic<bool> timedOut = false;

    auto t = std::thread([&] {
            adopted.store(BOBUIhread::currentThread());
#ifdef __cpp_lib_atomic_wait
            adopted.notify_one();
#endif
            QEventLoop loop;
            // fallback in case the invokeMethod() below fails
            BOBUIimer::singleShot(1s, &loop, [&] { timedOut.store(true); loop.quit(); });
            loop.exec();
        });

    QObject obj;
    auto joiner = qScopeGuard([&] { t.join(); });
#ifdef __cpp_lib_atomic_wait
    adopted.wait(nullptr); // no timed version exists :(
    QVERIFY(adopted.load());
#else
    QVERIFY(BOBUIest::qWaitFor([&] { return adopted.load(); }));
#endif
    QVERIFY(obj.moveToThread(adopted.load()));
    QCOMPARE(obj.thread(), adopted.load());
    // The lambda will only be executed when `adopted` has an event loop running:
    QVERIFY(QMetaObject::invokeMethod(&obj, [&] {
                                                called.store(true);
                                                BOBUIhread::currentThread()->quit();
                                            }, BobUI::QueuedConnection));
    joiner.commit();
    QVERIFY(!timedOut.load());
    QVERIFY(called.load());
}

class JobObject : public QObject
{
    Q_OBJECT
public:

    explicit JobObject(QEventLoop *loop, QObject *parent = nullptr)
        : QObject(parent), locker(loop)
    {
    }

    explicit JobObject(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

public slots:
    void start(int timeout = 200)
    {
        BOBUIimer::singleShot(timeout, this, SLOT(timeout()));
    }

private slots:
    void timeout()
    {
        emit done();
        deleteLater();
    }

signals:
    void done();

private:
    QEventLoopLocker locker;
};

void tst_QEventLoop::testQuitLock()
{
    QEventLoop eventLoop;

    QEventLoopPrivate* privateClass = static_cast<QEventLoopPrivate*>(QObjectPrivate::get(&eventLoop));

    QCOMPARE(privateClass->quitLockRef.loadRelaxed(), 0);

    JobObject *job1 = new JobObject(&eventLoop, this);
    job1->start(500);

    QCOMPARE(privateClass->quitLockRef.loadRelaxed(), 1);

    eventLoop.exec();

    QCOMPARE(privateClass->quitLockRef.loadRelaxed(), 0);


    job1 = new JobObject(&eventLoop, this);
    job1->start(200);

    JobObject *previousJob = job1;
    for (int i = 0; i < 9; ++i) {
        JobObject *subJob = new JobObject(&eventLoop, this);
        connect(previousJob, SIGNAL(done()), subJob, SLOT(start()));
        previousJob = subJob;
    }

    eventLoop.exec();
}

BOBUIEST_MAIN(tst_QEventLoop)
#include "tst_qeventloop.moc"
