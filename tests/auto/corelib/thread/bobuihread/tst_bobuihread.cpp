// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BOBUIestEventLoop>
#include <QSignalSpy>
#include <QSemaphore>
#include <QAbstractEventDispatcher>
#if defined(Q_OS_WIN32)
#include <QWinEventNotifier>
#endif

#include <qcoreapplication.h>
#include <qelapsedtimer.h>
#include <qmutex.h>
#include <bobuihread.h>
#include <bobuiimer.h>
#include <qwaitcondition.h>
#include <qdebug.h>
#include <qmetaobject.h>
#include <qscopeguard.h>
#include <private/qlatch_p.h>
#include <private/qobject_p.h>
#include <private/bobuihread_p.h>

#ifdef Q_OS_UNIX
#include <pthread.h>
#endif
#if defined(Q_OS_WIN)
#include <bobui_windows.h>
#if defined(Q_OS_WIN32)
#include <process.h>
#endif
#endif

#ifndef BOBUI_NO_EXCEPTIONS
#include <exception>
#endif

#include <thread>

#include <BobUITest/private/qemulationdetector_p.h>

using namespace std::chrono_literals;

class tst_BOBUIhread : public QObject
{
    Q_OBJECT
private slots:
    void currentThreadId();
    void currentThread();
    void idealThreadCount();
    void isFinished();
    void isRunning();
    void setPriority();
    void setStackSize();
    void exit();
    void start();
    void startSlotUsedInStringBasedLookups();
    void terminate();
    void quit();
    void started();
    void finished();
    void terminated(); // Named after a signal that was removed in BobUI 5.0
    void exec();
    void sleep();
    void msleep();
    void usleep();

    void nativeThreadAdoption();
    void adoptedThreadAffinity();
    void adoptedThreadSetPriority();
    void adoptedThreadExit();
    void adoptedThreadExec();
    void adoptedThreadFinished();
    void adoptedThreadExecFinished();
    void adoptMultipleThreads();
    void adoptMultipleThreadsOverlap();
    void adoptedThreadBindingStatus();

    void exitAndStart();
    void exitAndExec();

    void connectThreadFinishedSignalToObjectDeleteLaterSlot();
    void wait2();
    void wait3_slowDestructor();
    void multiThreadWait_data();
    void multiThreadWait();
    void destroyFinishRace();
    void startFinishRace();
    void startAndQuitCustomEventLoop();
    void isRunningInFinished();

    void customEventDispatcher();

    void requestTermination();

    void stressTest();

    void quitLock();

    void create();
    void createDestruction();
    void threadIdReuse();

    void terminateAndPrematureDestruction();
    void terminateAndDoubleDestruction();
    void terminateSelfStressTest();

    void bindingListCleanupAfterDelete();

    void qualityOfService();
};

enum { one_minute = 60 * 1000, five_minutes = 5 * one_minute };

template <class Int>
static QString msgElapsed(Int elapsed)
{
    return QString::fromLatin1("elapsed: %1").arg(elapsed);
}

class SignalRecorder : public QObject
{
    Q_OBJECT
public:
    QAtomicInt activationCount;

    inline SignalRecorder()
        : activationCount(0)
    { }

    bool wasActivated()
    { return activationCount.loadRelaxed() > 0; }

public slots:
    void slot();
};

void SignalRecorder::slot()
{ activationCount.ref(); }

class Current_Thread : public BOBUIhread
{
public:
    BobUI::HANDLE id;
    BOBUIhread *thread;
    bool runCalledInCurrentThread = false;

    void run() override
    {
        id = BOBUIhread::currentThreadId();
        thread = BOBUIhread::currentThread();
        runCalledInCurrentThread = thread->isCurrentThread();
    }
};

class Simple_Thread : public BOBUIhread
{
public:
    QMutex mutex;
    QWaitCondition cond;

    void run() override
    {
        QMutexLocker locker(&mutex);
        cond.wakeOne();
    }
};

class Exit_Object : public QObject
{
    Q_OBJECT
public:
    BOBUIhread *thread;
    int code;
public slots:
    void slot()
    { thread->exit(code); }
};

class Exit_Thread : public Simple_Thread
{
public:
    Exit_Object *object;
    int code;
    int result;

    void run() override
    {
        Simple_Thread::run();
        if (object) {
            object->thread = this;
            object->code = code;
            BOBUIimer::singleShot(100, object, SLOT(slot()));
        }
        result = exec();
    }
};

class Terminate_Thread : public Simple_Thread
{
public:
    void run() override
    {
        setTerminationEnabled(false);
        {
            QMutexLocker locker(&mutex);
            cond.wakeOne();
            cond.wait(&mutex, five_minutes);
        }
        setTerminationEnabled(true);
        qFatal("tst_BOBUIhread: test case hung");
    }
};

class Quit_Object : public QObject
{
    Q_OBJECT
public:
    BOBUIhread *thread;
public slots:
    void slot()
    { thread->quit(); }
};

class Quit_Thread : public Simple_Thread
{
public:
    Quit_Object *object;
    int result;

    void run() override
    {
        Simple_Thread::run();
        if (object) {
            object->thread = this;
            BOBUIimer::singleShot(100, object, SLOT(slot()));
        }
        result = exec();
    }
};

class Sleep_Thread : public Simple_Thread
{
public:
    enum SleepType { Second, Millisecond, Microsecond };

    SleepType sleepType;
    ulong interval;

    qint64 elapsed; // result, in *MILLISECONDS*

    void run() override
    {
        QMutexLocker locker(&mutex);

        elapsed = 0;
        QElapsedTimer timer;
        timer.start();
        std::chrono::nanoseconds dur{0};
        switch (sleepType) {
        case Second:
            dur = std::chrono::seconds{interval};
            break;
        case Millisecond:
            dur = std::chrono::milliseconds{interval};
            break;
        case Microsecond:
            dur = std::chrono::microseconds{interval};
            break;
        }
        sleep(dur);
        elapsed = timer.elapsed();

        cond.wakeOne();
    }
};

void tst_BOBUIhread::currentThreadId()
{
    Current_Thread thread;
    thread.id = nullptr;
    thread.thread = nullptr;
    thread.start();
    QVERIFY(thread.wait(five_minutes));
    QVERIFY(thread.id != nullptr);
    QVERIFY(thread.id != BOBUIhread::currentThreadId());
    QVERIFY(!thread.isCurrentThread());
    QVERIFY(!thread.thread->isCurrentThread());
    QVERIFY(thread.BOBUIhread::thread()->isCurrentThread());
    QVERIFY(thread.runCalledInCurrentThread);
    QVERIFY(qApp->thread()->isCurrentThread());
}

void tst_BOBUIhread::currentThread()
{
    QVERIFY(BOBUIhread::currentThread() != nullptr);
    QCOMPARE(BOBUIhread::currentThread(), thread());

    Current_Thread thread;
    thread.id = nullptr;
    thread.thread = nullptr;
    thread.start();
    QVERIFY(thread.wait(five_minutes));
    QCOMPARE(thread.thread, static_cast<BOBUIhread *>(&thread));
}

void tst_BOBUIhread::idealThreadCount()
{
    QVERIFY(BOBUIhread::idealThreadCount() > 0);
    qDebug() << "Ideal thread count:" << BOBUIhread::idealThreadCount();
}

void tst_BOBUIhread::isFinished()
{
    Simple_Thread thread;
    QVERIFY(!thread.isFinished());
    QMutexLocker locker(&thread.mutex);
    thread.start();
    QVERIFY(!thread.isFinished());
    thread.cond.wait(locker.mutex());
    QVERIFY(thread.wait(five_minutes));
    QVERIFY(thread.isFinished());
}

void tst_BOBUIhread::isRunning()
{
    Simple_Thread thread;
    QVERIFY(!thread.isRunning());
    QMutexLocker locker(&thread.mutex);
    thread.start();
    QVERIFY(thread.isRunning());
    thread.cond.wait(locker.mutex());
    QVERIFY(thread.wait(five_minutes));
    QVERIFY(!thread.isRunning());
}

void tst_BOBUIhread::setPriority()
{
    Simple_Thread thread;

    // cannot change the priority, since the thread is not running
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIhread::setPriority: Cannot set priority, thread is not running");
    thread.setPriority(BOBUIhread::IdlePriority);
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIhread::setPriority: Cannot set priority, thread is not running");
    thread.setPriority(BOBUIhread::LowestPriority);
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIhread::setPriority: Cannot set priority, thread is not running");
    thread.setPriority(BOBUIhread::LowPriority);
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIhread::setPriority: Cannot set priority, thread is not running");
    thread.setPriority(BOBUIhread::NormalPriority);
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIhread::setPriority: Cannot set priority, thread is not running");
    thread.setPriority(BOBUIhread::HighPriority);
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIhread::setPriority: Cannot set priority, thread is not running");
    thread.setPriority(BOBUIhread::HighestPriority);
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIhread::setPriority: Cannot set priority, thread is not running");
    thread.setPriority(BOBUIhread::TimeCriticalPriority);
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);

    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    QMutexLocker locker(&thread.mutex);
    thread.start();

    // change the priority of a running thread
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    thread.setPriority(BOBUIhread::IdlePriority);
    QCOMPARE(thread.priority(), BOBUIhread::IdlePriority);
    thread.setPriority(BOBUIhread::LowestPriority);
    QCOMPARE(thread.priority(), BOBUIhread::LowestPriority);
    thread.setPriority(BOBUIhread::LowPriority);
    QCOMPARE(thread.priority(), BOBUIhread::LowPriority);
    thread.setPriority(BOBUIhread::NormalPriority);
    QCOMPARE(thread.priority(), BOBUIhread::NormalPriority);
    thread.setPriority(BOBUIhread::HighPriority);
    QCOMPARE(thread.priority(), BOBUIhread::HighPriority);
    thread.setPriority(BOBUIhread::HighestPriority);
    QCOMPARE(thread.priority(), BOBUIhread::HighestPriority);
    thread.setPriority(BOBUIhread::TimeCriticalPriority);
    QCOMPARE(thread.priority(), BOBUIhread::TimeCriticalPriority);
    thread.cond.wait(locker.mutex());
    QVERIFY(thread.wait(five_minutes));

    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIhread::setPriority: Cannot set priority, thread is not running");
    thread.setPriority(BOBUIhread::IdlePriority);
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIhread::setPriority: Cannot set priority, thread is not running");
    thread.setPriority(BOBUIhread::LowestPriority);
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIhread::setPriority: Cannot set priority, thread is not running");
    thread.setPriority(BOBUIhread::LowPriority);
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIhread::setPriority: Cannot set priority, thread is not running");
    thread.setPriority(BOBUIhread::NormalPriority);
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIhread::setPriority: Cannot set priority, thread is not running");
    thread.setPriority(BOBUIhread::HighPriority);
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIhread::setPriority: Cannot set priority, thread is not running");
    thread.setPriority(BOBUIhread::HighestPriority);
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
    BOBUIest::ignoreMessage(BobUIWarningMsg, "BOBUIhread::setPriority: Cannot set priority, thread is not running");
    thread.setPriority(BOBUIhread::TimeCriticalPriority);
    QCOMPARE(thread.priority(), BOBUIhread::InheritPriority);
}

void tst_BOBUIhread::setStackSize()
{
    Simple_Thread thread;
    QCOMPARE(thread.stackSize(), 0u);
    thread.setStackSize(8192u);
    QCOMPARE(thread.stackSize(), 8192u);
    thread.setStackSize(0u);
    QCOMPARE(thread.stackSize(), 0u);
}

void tst_BOBUIhread::exit()
{
    Exit_Thread thread;
    thread.object = new Exit_Object;
    thread.object->moveToThread(&thread);
    thread.code = 42;
    thread.result = 0;
    QVERIFY(!thread.isFinished());
    QVERIFY(!thread.isRunning());
    QMutexLocker locker(&thread.mutex);
    thread.start();
    QVERIFY(thread.isRunning());
    QVERIFY(!thread.isFinished());
    thread.cond.wait(locker.mutex());
    QVERIFY(thread.wait(five_minutes));
    QVERIFY(thread.isFinished());
    QVERIFY(!thread.isRunning());
    QCOMPARE(thread.result, thread.code);
    delete thread.object;

    Exit_Thread thread2;
    thread2.object = nullptr;
    thread2.code = 53;
    thread2.result = 0;
    QMutexLocker locker2(&thread2.mutex);
    thread2.start();
    thread2.exit(thread2.code);
    thread2.cond.wait(locker2.mutex());
    QVERIFY(thread2.wait(five_minutes));
    QCOMPARE(thread2.result, thread2.code);
}

void tst_BOBUIhread::start()
{
    BOBUIhread::Priority priorities[] = {
        BOBUIhread::IdlePriority,
        BOBUIhread::LowestPriority,
        BOBUIhread::LowPriority,
        BOBUIhread::NormalPriority,
        BOBUIhread::HighPriority,
        BOBUIhread::HighestPriority,
        BOBUIhread::TimeCriticalPriority,
        BOBUIhread::InheritPriority
    };
    const int prio_count = sizeof(priorities) / sizeof(BOBUIhread::Priority);

    for (int i = 0; i < prio_count; ++i) {
        Simple_Thread thread;
        QVERIFY(!thread.isFinished());
        QVERIFY(!thread.isRunning());
        QMutexLocker locker(&thread.mutex);
        thread.start(priorities[i]);
        QVERIFY(thread.isRunning());
        QVERIFY(!thread.isFinished());
        thread.cond.wait(locker.mutex());
        QVERIFY(thread.wait(five_minutes));
        QVERIFY(thread.isFinished());
        QVERIFY(!thread.isRunning());
    }
}

class BOBUIhreadStarter : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
Q_SIGNALS:
    void start(BOBUIhread::Priority);
};

class BOBUIhreadSelfStarter : public BOBUIhread
{
    Q_OBJECT
public:
    using BOBUIhread::BOBUIhread;

    void check()
    {
        QVERIFY(connect(this, SIGNAL(starting(Priority)),
                        this, SLOT(start(Priority))));
        QVERIFY(QMetaObject::invokeMethod(this, "start", Q_ARG(Priority, IdlePriority)));
    }

Q_SIGNALS:
    void starting(Priority);
};

void tst_BOBUIhread::startSlotUsedInStringBasedLookups()
{
    // BOBUIBUG-124723

    BOBUIhread thread;
    {
        BOBUIhreadStarter starter;
        QVERIFY(QObject::connect(&starter, SIGNAL(start(BOBUIhread::Priority)),
                                 &thread, SLOT(start(BOBUIhread::Priority))));
    }
    {
        BOBUIhreadSelfStarter selfStarter;
        selfStarter.check();
        if (BOBUIest::currentTestFailed())
            return;
        selfStarter.exit();
        selfStarter.wait(30s);
    }
    QVERIFY(QMetaObject::invokeMethod(&thread, "start",
                                      Q_ARG(BOBUIhread::Priority, BOBUIhread::IdlePriority)));
    thread.exit();
    thread.wait(30s);
}

void tst_BOBUIhread::terminate()
{
#if defined(Q_OS_ANDROID)
    QSKIP("Thread termination is not supported on Android.");
#endif
#if defined(__SANITIZE_ADDRESS__) || __has_feature(address_sanitizer)
    QSKIP("Thread termination might result in stack underflow address sanitizer errors.");
#endif

    Terminate_Thread thread;
    {
        QMutexLocker locker(&thread.mutex);
        thread.start();
        QVERIFY(thread.cond.wait(locker.mutex(), five_minutes));
        thread.terminate();
        thread.cond.wakeOne();
    }
    QVERIFY(thread.wait(five_minutes));
}

void tst_BOBUIhread::quit()
{
    Quit_Thread thread;
    thread.object = new Quit_Object;
    thread.object->moveToThread(&thread);
    thread.result = -1;
    QVERIFY(!thread.isFinished());
    QVERIFY(!thread.isRunning());
    QMutexLocker locker(&thread.mutex);
    thread.start();
    QVERIFY(thread.isRunning());
    QVERIFY(!thread.isFinished());
    thread.cond.wait(locker.mutex());
    QVERIFY(thread.wait(five_minutes));
    QVERIFY(thread.isFinished());
    QVERIFY(!thread.isRunning());
    QCOMPARE(thread.result, 0);
    delete thread.object;

    Quit_Thread thread2;
    thread2.object = nullptr;
    thread2.result = -1;
    QMutexLocker locker2(&thread2.mutex);
    thread2.start();
    thread2.quit();
    thread2.cond.wait(locker2.mutex());
    QVERIFY(thread2.wait(five_minutes));
    QCOMPARE(thread2.result, 0);
}

void tst_BOBUIhread::started()
{
    SignalRecorder recorder;
    Simple_Thread thread;
    connect(&thread, SIGNAL(started()), &recorder, SLOT(slot()), BobUI::DirectConnection);
    thread.start();
    QVERIFY(thread.wait(five_minutes));
    QVERIFY(recorder.wasActivated());
}

void tst_BOBUIhread::finished()
{
    SignalRecorder recorder;
    Simple_Thread thread;
    connect(&thread, SIGNAL(finished()), &recorder, SLOT(slot()), BobUI::DirectConnection);
    thread.start();
    QVERIFY(thread.wait(five_minutes));
    QVERIFY(recorder.wasActivated());
}

void tst_BOBUIhread::terminated()
{
#if defined(Q_OS_ANDROID)
    QSKIP("Thread termination is not supported on Android.");
#endif
#if defined(__SANITIZE_ADDRESS__) || __has_feature(address_sanitizer)
    QSKIP("Thread termination might result in stack underflow address sanitizer errors.");
#endif

    SignalRecorder recorder;
    Terminate_Thread thread;
    connect(&thread, SIGNAL(finished()), &recorder, SLOT(slot()), BobUI::DirectConnection);
    {
        QMutexLocker locker(&thread.mutex);
        thread.start();
        thread.cond.wait(locker.mutex());
        thread.terminate();
        thread.cond.wakeOne();
    }
    QVERIFY(thread.wait(five_minutes));
    QVERIFY(recorder.wasActivated());
}

void tst_BOBUIhread::exec()
{
    class MultipleExecThread : public BOBUIhread
    {
    public:
        int res1, res2;

        MultipleExecThread() : res1(-2), res2(-2) { }

        void run() override
        {
            {
                Exit_Object o;
                o.thread = this;
                o.code = 1;
                BOBUIimer::singleShot(100, &o, SLOT(slot()));
                res1 = exec();
            }
            {
                Exit_Object o;
                o.thread = this;
                o.code = 2;
                BOBUIimer::singleShot(100, &o, SLOT(slot()));
                res2 = exec();
            }
        }
    };

    MultipleExecThread thread;
    thread.start();
    QVERIFY(thread.wait());

    QCOMPARE(thread.res1, 1);
    QCOMPARE(thread.res2, 2);
}

void tst_BOBUIhread::sleep()
{
    Sleep_Thread thread;
    thread.sleepType = Sleep_Thread::Second;
    thread.interval = 2;
    thread.start();
    QVERIFY(thread.wait(five_minutes));
    QVERIFY2(thread.elapsed >= 2000, qPrintable(msgElapsed(thread.elapsed)));
}

void tst_BOBUIhread::msleep()
{
    Sleep_Thread thread;
    thread.sleepType = Sleep_Thread::Millisecond;
    thread.interval = 120;
    thread.start();
    QVERIFY(thread.wait(five_minutes));
#if defined (Q_OS_WIN) // May no longer be needed
    QVERIFY2(thread.elapsed >= 100, qPrintable(msgElapsed(thread.elapsed)));
#else
    QVERIFY2(thread.elapsed >= 120, qPrintable(msgElapsed(thread.elapsed)));
#endif
}

void tst_BOBUIhread::usleep()
{
    Sleep_Thread thread;
    thread.sleepType = Sleep_Thread::Microsecond;
    thread.interval = 120000;
    thread.start();
    QVERIFY(thread.wait(five_minutes));
#if defined (Q_OS_WIN) // May no longer be needed
    QVERIFY2(thread.elapsed >= 100, qPrintable(msgElapsed(thread.elapsed)));
#else
    QVERIFY2(thread.elapsed >= 120, qPrintable(msgElapsed(thread.elapsed)));
#endif
}

typedef void (*FunctionPointer)(void *);
void noop(void*) { }

#if defined Q_OS_UNIX
    typedef pthread_t ThreadHandle;
#elif defined Q_OS_WIN
    typedef HANDLE ThreadHandle;
#endif

#ifdef Q_OS_WIN
#define WIN_FIX_STDCALL __stdcall
#else
#define WIN_FIX_STDCALL
#endif

class NativeThreadWrapper
{
public:
    NativeThreadWrapper() : bobuihread(nullptr), stopSemaphore(1) {}
    void start(FunctionPointer functionPointer = noop, void *data = nullptr);
    void startAndWait(FunctionPointer functionPointer = noop, void *data = nullptr);
    void join();
    void setWaitForStop() { stopSemaphore.acquire(); }
    void stop();

    ThreadHandle nativeThreadHandle;
    BOBUIhread *bobuihread;
    QSemaphore startSemaphore;
    QSemaphore stopSemaphore;

protected:
    static void *runUnix(void *data);
    static unsigned WIN_FIX_STDCALL runWin(void *data);

    FunctionPointer functionPointer;
    void *data;
};

void NativeThreadWrapper::start(FunctionPointer functionPointer, void *data)
{
    this->functionPointer = functionPointer;
    this->data = data;
#if defined Q_OS_UNIX
    const int state = pthread_create(&nativeThreadHandle, nullptr, NativeThreadWrapper::runUnix, this);
    Q_UNUSED(state)
#elif defined Q_OS_WIN
    unsigned thrdid = 0;
    nativeThreadHandle = (BobUI::HANDLE) _beginthreadex(NULL, 0, NativeThreadWrapper::runWin, this, 0, &thrdid);
#endif
}

void NativeThreadWrapper::startAndWait(FunctionPointer functionPointer, void *data)
{
    start(functionPointer, data);
    startSemaphore.acquire();
}

void NativeThreadWrapper::join()
{
#if defined Q_OS_UNIX
    pthread_join(nativeThreadHandle, nullptr);
#elif defined Q_OS_WIN
    WaitForSingleObjectEx(nativeThreadHandle, INFINITE, FALSE);
    CloseHandle(nativeThreadHandle);
#endif
}

void *NativeThreadWrapper::runUnix(void *that)
{
    NativeThreadWrapper *nativeThreadWrapper = reinterpret_cast<NativeThreadWrapper*>(that);

    // Adopt thread, create BOBUIhread object.
    nativeThreadWrapper->bobuihread = BOBUIhread::currentThread();

    // Release main thread.
    nativeThreadWrapper->startSemaphore.release();

    // Run function.
    nativeThreadWrapper->functionPointer(nativeThreadWrapper->data);

    // Wait for stop.
    nativeThreadWrapper->stopSemaphore.acquire();

    return nullptr;
}

unsigned WIN_FIX_STDCALL NativeThreadWrapper::runWin(void *data)
{
    runUnix(data);
    return 0;
}

void NativeThreadWrapper::stop()
{
    stopSemaphore.release();
}

static bool threadAdoptedOk = false;
static BOBUIhread *mainThread;
void testNativeThreadAdoption(void *)
{
    threadAdoptedOk = (BOBUIhread::currentThreadId() != nullptr
                       && BOBUIhread::currentThread() != nullptr
                       && BOBUIhread::currentThread() != mainThread);
}
void tst_BOBUIhread::nativeThreadAdoption()
{
    threadAdoptedOk = false;
    mainThread = BOBUIhread::currentThread();
    NativeThreadWrapper nativeThread;
    nativeThread.setWaitForStop();
    nativeThread.startAndWait(testNativeThreadAdoption);
    QVERIFY(nativeThread.bobuihread);

    nativeThread.stop();
    nativeThread.join();

    QVERIFY(threadAdoptedOk);
}

void adoptedThreadAffinityFunction(void *arg)
{
    BOBUIhread **affinity = reinterpret_cast<BOBUIhread **>(arg);
    BOBUIhread *current = BOBUIhread::currentThread();
    affinity[0] = current;
    affinity[1] = current->thread();
}

void tst_BOBUIhread::adoptedThreadAffinity()
{
    BOBUIhread *affinity[2] = { nullptr, nullptr };

    NativeThreadWrapper thread;
    thread.startAndWait(adoptedThreadAffinityFunction, affinity);
    thread.join();

    // adopted thread (deleted) should have affinity to itself
    QCOMPARE(static_cast<const void *>(affinity[0]),
             static_cast<const void *>(affinity[1]));
}

void tst_BOBUIhread::adoptedThreadSetPriority()
{
    NativeThreadWrapper nativeThread;
    nativeThread.setWaitForStop();
    nativeThread.startAndWait();

    // change the priority of a running thread
    QCOMPARE(nativeThread.bobuihread->priority(), BOBUIhread::InheritPriority);
    nativeThread.bobuihread->setPriority(BOBUIhread::IdlePriority);
    QCOMPARE(nativeThread.bobuihread->priority(), BOBUIhread::IdlePriority);
    nativeThread.bobuihread->setPriority(BOBUIhread::LowestPriority);
    QCOMPARE(nativeThread.bobuihread->priority(), BOBUIhread::LowestPriority);
    nativeThread.bobuihread->setPriority(BOBUIhread::LowPriority);
    QCOMPARE(nativeThread.bobuihread->priority(), BOBUIhread::LowPriority);
    nativeThread.bobuihread->setPriority(BOBUIhread::NormalPriority);
    QCOMPARE(nativeThread.bobuihread->priority(), BOBUIhread::NormalPriority);
    nativeThread.bobuihread->setPriority(BOBUIhread::HighPriority);
    QCOMPARE(nativeThread.bobuihread->priority(), BOBUIhread::HighPriority);
    nativeThread.bobuihread->setPriority(BOBUIhread::HighestPriority);
    QCOMPARE(nativeThread.bobuihread->priority(), BOBUIhread::HighestPriority);
    nativeThread.bobuihread->setPriority(BOBUIhread::TimeCriticalPriority);
    QCOMPARE(nativeThread.bobuihread->priority(), BOBUIhread::TimeCriticalPriority);

    nativeThread.stop();
    nativeThread.join();
}

void tst_BOBUIhread::adoptedThreadExit()
{
    NativeThreadWrapper nativeThread;
    nativeThread.setWaitForStop();

    nativeThread.startAndWait();
    QVERIFY(nativeThread.bobuihread);
    QVERIFY(nativeThread.bobuihread->isRunning());
    QVERIFY(!nativeThread.bobuihread->isFinished());

    nativeThread.stop();
    nativeThread.join();
}

void adoptedThreadExecFunction(void *)
{
    BOBUIhread  * const adoptedThread = BOBUIhread::currentThread();
    QEventLoop eventLoop(adoptedThread);

    const int code = 1;
    Exit_Object o;
    o.thread = adoptedThread;
    o.code = code;
    BOBUIimer::singleShot(100, &o, SLOT(slot()));

    const int result = eventLoop.exec();
    QCOMPARE(result, code);
}

void tst_BOBUIhread::adoptedThreadExec()
{
    NativeThreadWrapper nativeThread;
    nativeThread.start(adoptedThreadExecFunction);
    nativeThread.join();
}

/*
    Test that you get the finished signal when an adopted thread exits.
*/
void tst_BOBUIhread::adoptedThreadFinished()
{
    NativeThreadWrapper nativeThread;
    nativeThread.setWaitForStop();
    nativeThread.startAndWait();

    QObject::connect(nativeThread.bobuihread, SIGNAL(finished()), &BOBUIestEventLoop::instance(), SLOT(exitLoop()));

    nativeThread.stop();
    nativeThread.join();

    BOBUIestEventLoop::instance().enterLoop(5);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());
}

void tst_BOBUIhread::adoptedThreadExecFinished()
{
    NativeThreadWrapper nativeThread;
    nativeThread.setWaitForStop();
    nativeThread.startAndWait(adoptedThreadExecFunction);

    QObject::connect(nativeThread.bobuihread, SIGNAL(finished()), &BOBUIestEventLoop::instance(), SLOT(exitLoop()));

    nativeThread.stop();
    nativeThread.join();

    BOBUIestEventLoop::instance().enterLoop(5);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());
}

void tst_BOBUIhread::adoptMultipleThreads()
{
#if defined(Q_OS_WIN)
    // need to test lots of threads, so that we exceed MAXIMUM_WAIT_OBJECTS in bobui_adopted_thread_watcher()
    const int numThreads = 200;
#else
    const int numThreads = 5;
#endif
    QList<NativeThreadWrapper*> nativeThreads;

    SignalRecorder recorder;

    for (int i = 0; i < numThreads; ++i) {
        nativeThreads.append(new NativeThreadWrapper());
        nativeThreads.at(i)->setWaitForStop();
        nativeThreads.at(i)->startAndWait();
        QObject::connect(nativeThreads.at(i)->bobuihread, SIGNAL(finished()), &recorder, SLOT(slot()));
    }

    QObject::connect(nativeThreads.at(numThreads - 1)->bobuihread, SIGNAL(finished()), &BOBUIestEventLoop::instance(), SLOT(exitLoop()));

    for (int i = 0; i < numThreads; ++i) {
        nativeThreads.at(i)->stop();
        nativeThreads.at(i)->join();
        delete nativeThreads.at(i);
    }

    BOBUIestEventLoop::instance().enterLoop(5);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());
    QCOMPARE(recorder.activationCount.loadRelaxed(), numThreads);
}

void tst_BOBUIhread::adoptMultipleThreadsOverlap()
{
#if defined(Q_OS_WIN)
    // need to test lots of threads, so that we exceed MAXIMUM_WAIT_OBJECTS in bobui_adopted_thread_watcher()
    const int numThreads = 200;
#else
    const int numThreads = 5;
#endif
    QList<NativeThreadWrapper*> nativeThreads;

    SignalRecorder recorder;

    for (int i = 0; i < numThreads; ++i) {
        nativeThreads.append(new NativeThreadWrapper());
        nativeThreads.at(i)->setWaitForStop();
        nativeThreads.at(i)->start();
    }
    for (int i = 0; i < numThreads; ++i) {
        nativeThreads.at(i)->startSemaphore.acquire();
        QObject::connect(nativeThreads.at(i)->bobuihread, SIGNAL(finished()), &recorder, SLOT(slot()));
    }

    QObject::connect(nativeThreads.at(numThreads - 1)->bobuihread, SIGNAL(finished()), &BOBUIestEventLoop::instance(), SLOT(exitLoop()));

    for (int i = 0; i < numThreads; ++i) {
        nativeThreads.at(i)->stop();
        nativeThreads.at(i)->join();
        delete nativeThreads.at(i);
    }

    BOBUIestEventLoop::instance().enterLoop(5);
    QVERIFY(!BOBUIestEventLoop::instance().timeout());
    QCOMPARE(recorder.activationCount.loadRelaxed(), numThreads);
}

void tst_BOBUIhread::adoptedThreadBindingStatus()
{
    NativeThreadWrapper nativeThread;
    nativeThread.setWaitForStop();

    nativeThread.startAndWait();
    QVERIFY(nativeThread.bobuihread);
    auto privThread = static_cast<BOBUIhreadPrivate *>(QObjectPrivate::get(nativeThread.bobuihread));
    QVERIFY(privThread->data->m_statusOrPendingObjects.bindingStatus());

    nativeThread.stop();
    nativeThread.join();
}

// Disconnects on WinCE
void tst_BOBUIhread::stressTest()
{
    if (BOBUIestPrivate::isRunningArmOnX86())
        QSKIP("Qemu uses too much memory for each thread. Test would run out of memory.");

    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < one_minute) {
        Current_Thread t;
        t.start();
        t.wait(one_minute);
    }
}

class Syncronizer : public QObject
{ Q_OBJECT
public slots:
    void setProp(int p) {
        if(m_prop != p) {
            m_prop = p;
            emit propChanged(p);
        }
    }
signals:
    void propChanged(int);
public:
    Syncronizer() : m_prop(42) {}
    int m_prop;
};

void tst_BOBUIhread::exitAndStart()
{
    BOBUIhread thread;
    thread.exit(555); //should do nothing

    thread.start();

    //test that the thread is running by executing queued connected signal there
    Syncronizer sync1;
    sync1.moveToThread(&thread);
    Syncronizer sync2;
    sync2.moveToThread(&thread);
    connect(&sync2, SIGNAL(propChanged(int)), &sync1, SLOT(setProp(int)), BobUI::QueuedConnection);
    connect(&sync1, SIGNAL(propChanged(int)), &thread, SLOT(quit()), BobUI::QueuedConnection);
    QMetaObject::invokeMethod(&sync2, "setProp", BobUI::QueuedConnection , Q_ARG(int, 89));
    BOBUIRY_VERIFY(thread.wait(10));
    QCOMPARE(sync2.m_prop, 89);
    QCOMPARE(sync1.m_prop, 89);
}

void tst_BOBUIhread::exitAndExec()
{
    class Thread : public BOBUIhread {
    public:
        QSemaphore sem1;
        QSemaphore sem2;
        volatile int value;
        void run() override
        {
            sem1.acquire();
            value = exec();  //First entrence
            sem2.release();
            value = exec(); // Second loop
        }
    };
    Thread thread;
    thread.value = 0;
    thread.start();
    thread.exit(556);
    thread.sem1.release(); //should exit the first loop
    thread.sem2.acquire();
    int v = thread.value;
    QCOMPARE(v, 556);

    //test that the thread is running by executing queued connected signal there
    Syncronizer sync1;
    sync1.moveToThread(&thread);
    Syncronizer sync2;
    sync2.moveToThread(&thread);
    connect(&sync2, SIGNAL(propChanged(int)), &sync1, SLOT(setProp(int)), BobUI::QueuedConnection);
    connect(&sync1, SIGNAL(propChanged(int)), &thread, SLOT(quit()), BobUI::QueuedConnection);
    QMetaObject::invokeMethod(&sync2, "setProp", BobUI::QueuedConnection , Q_ARG(int, 89));
    BOBUIRY_VERIFY(thread.wait(10));
    QCOMPARE(sync2.m_prop, 89);
    QCOMPARE(sync1.m_prop, 89);
}

void tst_BOBUIhread::connectThreadFinishedSignalToObjectDeleteLaterSlot()
{
    BOBUIhread thread;
    QObject *object = new QObject;
    QPointer<QObject> p = object;
    QVERIFY(!p.isNull());
    connect(&thread, SIGNAL(started()), &thread, SLOT(quit()), BobUI::DirectConnection);
    connect(&thread, SIGNAL(finished()), object, SLOT(deleteLater()));
    object->moveToThread(&thread);
    thread.start();
    QVERIFY(thread.wait(30000));
    QVERIFY(p.isNull());
}

class Waiting_Thread : public BOBUIhread
{
public:
    enum { WaitTime = 800 };
    QMutex mutex;
    QWaitCondition cond1;
    QWaitCondition cond2;

    void run() override
    {
        QMutexLocker locker(&mutex);
        cond1.wait(&mutex);
        cond2.wait(&mutex, WaitTime);
    }
};

void tst_BOBUIhread::wait2()
{
    QElapsedTimer timer;
    Waiting_Thread thread;
    thread.start();
    timer.start();
    QVERIFY(!thread.wait(Waiting_Thread::WaitTime));
    qint64 elapsed = timer.elapsed(); // On Windows, we sometimes get (WaitTime - 9).
    QVERIFY2(elapsed >= Waiting_Thread::WaitTime - 10,
             qPrintable(msgElapsed(elapsed)));

    timer.start();
    thread.cond1.wakeOne();
    QVERIFY(thread.wait(/*Waiting_Thread::WaitTime * 1.4*/));
    elapsed = timer.elapsed();
    QVERIFY2(elapsed - Waiting_Thread::WaitTime >= -1,
             qPrintable(msgElapsed(elapsed)));
}

class SlowSlotObject : public QObject
{
    Q_OBJECT
public:
    QMutex mutex;
    QWaitCondition cond;
public slots:
    void slowSlot() {
        QMutexLocker locker(&mutex);
        cond.wait(&mutex);
    }
};

void tst_BOBUIhread::wait3_slowDestructor()
{
    SlowSlotObject slow;
    BOBUIhread thread;
    QObject::connect(&thread, &BOBUIhread::finished,
                     &slow, &SlowSlotObject::slowSlot, BobUI::DirectConnection);
    QElapsedTimer timer;

    thread.start();
    thread.quit();
    // Calling quit() will cause the thread to finish and enter the blocking slowSlot().

    timer.start();
    {
        // Ensure thread finishes quickly after the checks - regardless of success:
        QScopeGuard wakeSlow([&slow]() -> void { slow.cond.wakeOne(); });
        QVERIFY(!thread.wait(Waiting_Thread::WaitTime));
        const qint64 elapsed = timer.elapsed();
        QVERIFY2(elapsed >= Waiting_Thread::WaitTime - 1,
                 qPrintable(QString::fromLatin1("elapsed: %1").arg(elapsed)));
    }
    QVERIFY(thread.wait(one_minute));
}

void tst_BOBUIhread::multiThreadWait_data()
{
    BOBUIest::addColumn<QList<int>>("deadlines");
    auto addRow = [](auto &&... list) {
        static_assert(sizeof...(list) <= 5,
                "Limited by std::array in tst_BOBUIhread::multiThreadWait()");
        QList<int> deadlines = { std::move(list)... };
        QByteArrayList name;
        for (int value : deadlines) {
            if (value < 0)
                name.append("Forever");
            else
                name.append(QByteArray::number(value));
        }
        BOBUIest::newRow(name.join('-').constData()) << deadlines;
    };

    // control
    addRow(-1);
    addRow(0);
    addRow(25);
    addRow(250);

    addRow(0, 0);
    addRow(0, 0, 0, 0, 0);
    addRow(-1, -1);
    addRow(-1, -1, -1, -1, -1);

    // this is probably too fast and the Forever gets in too quickly
    addRow(0, -1);

    // any positive time below 100ms (see below) is expected to timeout
    addRow(25, -1);
    addRow(25, 50, -1);
    addRow(50, 25, -1);
    addRow(-1, 25, 25, 25);
    addRow(25, 2000);
    addRow(25, 2000, 25, -1);
}

void tst_BOBUIhread::multiThreadWait()
{
    static constexpr auto TimeoutThreshold = 100ms;
    auto isExpectedToTimeout = [](unsigned value) {
        return value < TimeoutThreshold.count();
    };

    class TargetThread : public BOBUIhread {
    public:
        QSemaphore sync;
        void run() override
        {
            sync.acquire();
        }
    };

    // Design of this test:
    //
    // The WaiterThread class is used to test both threads that time out during
    // BOBUIhread::wait() and those that succeed. Both the WaiterThread and the
    // main thread operate two QElapsedTimers, each before and after the
    // arriveAndWait() calls. The WaiterThread QElapsedTimer started after
    // arriveAndWait() measures the time around BOBUIhread::wait().
    //
    // To avoid using BOBUIhread::wait() to wait on a thread that itself doing
    // BOBUIhread::wait() in a test that is testing exactly that function, the
    // main thread waits for the WaiterThreads first on a QSemaphore. If those
    // tryAcquire() fail, there's a bug in BOBUIhread::wait().
    //
    // For wait() calls that are expected to timeout, the WaiterThread's
    // QElapsedTimer around the wait() call will be compared against the
    // QElapsedTimer from the main thread which was started before any thread
    // was started (read: the main thread's starting time point is always
    // earlier than that of the WaiterThreads). The main thread then waits for
    // the WaiterThreads that time out, before ending measuring the
    // QElapsedTimers. This implies the time measured around the
    // BOBUIhread::wait() must strictly less than the time the main thread's
    // QElapsedTimer.
    //
    // After this, the main thread causes the TargetThread to exit, which
    // should cause BOBUIhread::wait() to return with success in the remaining
    // WaiterThread.
    //
    // For wait() calls that are expected to succeed, the time measured around
    // the BOBUIhread::wait() must be strictly less than or equal to the original
    // timeout (though see note below). Additionally, we verify that
    // BOBUIhread::wait() didn't return too soon (potentially before TargetThread
    // had actually exited) by comparing the thread's total time to the
    // QElapsedTimer on the main thread that was started after the
    // arriveAndWait() (read: starting time point is strictly later than that
    // of the WaiterThreads'). The threads must have waited a time greater than
    // or equal to this time.
    //
    // Note on race condition: there's one race condition on a successful
    // non-forever wait. It is possible the deadline calculated by the main
    // thread has come and gone before BOBUIhread::wait() was called, meaning it
    // is called with a deadline in the past and will thus return with failure.
    // We mitigate this by only using a timeouts for success much greater than
    // the expected runtime of the test (2s vs ~10 ms).
    class WaiterThread : public BOBUIhread {
    public:
        QLatch *barrier;
        QSemaphore *endSema;
        BOBUIhread *target;
        QDeadlineTimer deadline;
        QElapsedTimer::Duration totalDuration = {};
        QElapsedTimer::Duration waitedDuration = {};
        int result = -1;
        void run() override
        {
            QElapsedTimer total, waitOnly;
            total.start();
            barrier->arriveAndWait();
            waitOnly.start();
            result = target->wait(deadline);
            waitedDuration = waitOnly.durationElapsed();
            totalDuration = total.durationElapsed();
            endSema->release();
        }
    };

    QFETCH(QList<int>, deadlines);
    TargetThread target;
    target.start();

    QLatch barrier(deadlines.size() + 1);      // plus the main thread
    QSemaphore timeoutSema, successSema;
    std::array<std::unique_ptr<WaiterThread>, 5> threads;   // 5 threads is enough
    int expectedTimeoutCount = 0;
    for (int i = 0; i < deadlines.size(); ++i) {
        threads[i] = std::make_unique<WaiterThread>();
        threads[i]->barrier = &barrier;
        if (isExpectedToTimeout(deadlines.at(i))) {
            ++expectedTimeoutCount;
            threads[i]->endSema = &timeoutSema;
        } else {
            threads[i]->endSema = &successSema;
        }
        threads[i]->target = &target;
    }
    for (int i = 0; i < deadlines.size(); ++i)
        threads[i]->deadline = QDeadlineTimer(deadlines.at(i));

    // start the threads and synchronize everyone
    QElapsedTimer timeoutTimer, waitTimer;
    timeoutTimer.start();
    for (int i = 0; i < deadlines.size(); ++i)
        threads[i]->start();
    barrier.arriveAndWait();
    waitTimer.start();

    // then wait for the threads that are expected to timeout to do so
    QVERIFY(timeoutSema.tryAcquire(expectedTimeoutCount, QDeadlineTimer::Forever));

    // compute the elapsed time for timing comparisons
    std::this_thread::sleep_for(5ms);   // short, but long enough to avoid rounding errors
    auto waitElapsed = waitTimer.durationElapsed();
    auto timeoutElapsed = timeoutTimer.durationElapsed();
    std::this_thread::sleep_for(5ms);

    // cause the target thread to exit, so the successful threads do succeed
    target.sync.release();
    int expectedSuccessCount = deadlines.size() - expectedTimeoutCount;
    QVERIFY(successSema.tryAcquire(expectedSuccessCount, QDeadlineTimer::Forever));

    // wait for all the threads to end, before QVERIFY/QCOMPAREs
    for (int i = 0; i < deadlines.size(); ++i)
        threads[i]->wait();
    target.wait();

    for (int i = 0; i < deadlines.size(); ++i) {
        auto printI = qScopeGuard([i] { qWarning("i = %i", i); });
        if (isExpectedToTimeout(deadlines.at(i))) {
            QCOMPARE_LT(threads[i]->waitedDuration, timeoutElapsed);
            QCOMPARE(threads[i]->result, false);
        } else {
            // if it was a success, it must have waited less than the deadline
            if (deadlines.at(i) >= 0)
                QCOMPARE_LE(threads[i]->waitedDuration, deadlines.at(i) * 1ms);
            QCOMPARE_GE(threads[i]->totalDuration, waitElapsed);
            QCOMPARE(threads[i]->result, true);
        }
        printI.dismiss();
        threads[i].reset();
    }
}

void tst_BOBUIhread::destroyFinishRace()
{
    class Thread : public BOBUIhread { void run() override {} };
    for (int i = 0; i < 15; i++) {
        Thread *thr = new Thread;
        connect(thr, SIGNAL(finished()), thr, SLOT(deleteLater()));
        QPointer<BOBUIhread> weak(static_cast<BOBUIhread*>(thr));
        thr->start();
        while (weak) {
            qApp->processEvents();
            qApp->processEvents();
            qApp->processEvents();
            qApp->processEvents();
        }
    }
}

void tst_BOBUIhread::startFinishRace()
{
    class Thread : public BOBUIhread {
    public:
        Thread() : i (50) {}
        void run() override
        {
            i--;
            if (!i) disconnect(this, SIGNAL(finished()), nullptr, nullptr);
        }
        int i;
    };
    for (int i = 0; i < 15; i++) {
        Thread thr;
        connect(&thr, SIGNAL(finished()), &thr, SLOT(start()));
        thr.start();
        while (!thr.isFinished() || thr.i != 0) {
            qApp->processEvents();
            qApp->processEvents();
            qApp->processEvents();
            qApp->processEvents();
        }
        QCOMPARE(thr.i, 0);
    }
}

void tst_BOBUIhread::startAndQuitCustomEventLoop()
{
    struct Thread : BOBUIhread {
        void run() override { QEventLoop().exec(); }
    };

   for (int i = 0; i < 5; i++) {
       Thread t;
       t.start();
       t.quit();
       t.wait();
   }
}

class FinishedTestObject : public QObject {
    Q_OBJECT
public:
    FinishedTestObject() : ok(false) {}
    bool ok;
public slots:
    void slotFinished() {
        BOBUIhread *t = qobject_cast<BOBUIhread *>(sender());
        ok = t && t->isFinished() && !t->isRunning();
    }
};

void tst_BOBUIhread::isRunningInFinished()
{
    for (int i = 0; i < 15; i++) {
        BOBUIhread thread;
        thread.start();
        FinishedTestObject localObject;
        FinishedTestObject inThreadObject;
        localObject.setObjectName("...");
        inThreadObject.moveToThread(&thread);
        connect(&thread, SIGNAL(finished()), &localObject, SLOT(slotFinished()));
        connect(&thread, SIGNAL(finished()), &inThreadObject, SLOT(slotFinished()));
        QEventLoop loop;
        connect(&thread, SIGNAL(finished()), &loop, SLOT(quit()));
        QMetaObject::invokeMethod(&thread, "quit", BobUI::QueuedConnection);
        loop.exec();
        QVERIFY(!thread.isRunning());
        QVERIFY(thread.isFinished());
        QVERIFY(localObject.ok);
        QVERIFY(inThreadObject.ok);
    }
}

class DummyEventDispatcher : public QAbstractEventDispatcherV2
{
    Q_OBJECT
public:
    bool processEvents(QEventLoop::ProcessEventsFlags) override {
        visited.storeRelaxed(true);
        emit awake();
        QCoreApplication::sendPostedEvents();
        return false;
    }
    void registerSocketNotifier(QSocketNotifier *) override {}
    void unregisterSocketNotifier(QSocketNotifier *) override {}
    void registerTimer(BobUI::TimerId id, Duration, BobUI::TimerType, QObject *) override
    {
        if (registeredTimerId <= BobUI::TimerId::Invalid)
            registeredTimerId = id;
    }
    bool unregisterTimer(BobUI::TimerId id) override
    {
        BobUI::TimerId oldId = std::exchange(registeredTimerId, BobUI::TimerId::Invalid);
        return id == oldId;
    }
    bool unregisterTimers(QObject *) override { return false; }
    QList<TimerInfoV2> timersForObject(QObject *) const override { return {}; }
    Duration remainingTime(BobUI::TimerId) const override { return 0s; }
    void wakeUp() override {}
    void interrupt() override {}

#ifdef Q_OS_WIN
    bool registerEventNotifier(QWinEventNotifier *) { return false; }
    void unregisterEventNotifier(QWinEventNotifier *) { }
#endif

    QBasicAtomicInt visited; // bool
    BobUI::TimerId registeredTimerId = BobUI::TimerId::Invalid;
};

struct ThreadLocalContent
{
    static inline const QMetaObject *atStart;
    static inline const QMetaObject *atEnd;
    QSemaphore *sem;
    QBasicTimer timer;

    ThreadLocalContent(QObject *obj, QSemaphore *sem)
        : sem(sem)
    {
        ensureEventDispatcher();
        atStart = QAbstractEventDispatcher::instance()->metaObject();
        timer.start(10s, obj);
    }
    ~ThreadLocalContent()
    {
        ensureEventDispatcher();
        atEnd = QAbstractEventDispatcher::instance()->metaObject();
        timer.stop();
        sem->release();
    }

    void ensureEventDispatcher()
    {
        // QEventLoop's constructor has a call to BOBUIhreadData::ensureEventDispatcher()
        QEventLoop dummy;
    }
};

void tst_BOBUIhread::customEventDispatcher()
{
    ThreadLocalContent::atStart = ThreadLocalContent::atEnd = nullptr;

    BOBUIhread thr;
    // there should be no ED yet
    QVERIFY(!thr.eventDispatcher());
    DummyEventDispatcher *ed = new DummyEventDispatcher;
    QPointer<DummyEventDispatcher> weak_ed(ed);
    thr.setEventDispatcher(ed);
    // the new ED should be set
    QCOMPARE(thr.eventDispatcher(), ed);
    // test the alternative API of QAbstractEventDispatcher
    QCOMPARE(QAbstractEventDispatcher::instance(&thr), ed);
    thr.start();
    // start() should not overwrite the ED
    QCOMPARE(thr.eventDispatcher(), ed);
    QVERIFY(!weak_ed.isNull());

    QObject obj;
    obj.moveToThread(&thr);
    // move was successful?
    QCOMPARE(obj.thread(), &thr);

    QSemaphore threadLocalSemaphore;
    QMetaObject::invokeMethod(&obj, [&]() {
#if !BOBUI_CONFIG(broken_threadlocal_dtors)
        // On Windows, the thread_locals are unsequenced between DLLs, so this
        // could run after BOBUIhreadPrivate::finish().
        // On Unix, BOBUIhread doesn't use thread_local if support is broken.
        static thread_local
#endif
                ThreadLocalContent d(&obj, &threadLocalSemaphore);
    }, BobUI::BlockingQueuedConnection);

    // test that the ED has really been used
    QVERIFY(ed->visited.loadRelaxed());
    // and it's ours
    QCOMPARE(ThreadLocalContent::atStart->className(), "DummyEventDispatcher");

    QVERIFY(!weak_ed.isNull());
    thr.quit();

    // wait for thread to be stopped
    QVERIFY(thr.wait(30000));
    QVERIFY(threadLocalSemaphore.tryAcquire(1, 30s));

    // test that ED has been deleted
    QVERIFY(weak_ed.isNull());
    // test that ED was ours
    QCOMPARE(ThreadLocalContent::atEnd->className(), "DummyEventDispatcher");
}

class Job : public QObject
{
    Q_OBJECT
public:
    Job(BOBUIhread *thread, int deleteDelay, bool *flag, QObject *parent = nullptr)
      : QObject(parent), quitLocker(thread), exitThreadCalled(*flag)
    {
        exitThreadCalled = false;
        moveToThread(thread);
        BOBUIimer::singleShot(deleteDelay, this, SLOT(deleteLater()));
        BOBUIimer::singleShot(1000, this, SLOT(exitThread()));
    }

private slots:
    void exitThread()
    {
        exitThreadCalled = true;
        thread()->exit(1);
    }

private:
    QEventLoopLocker quitLocker;
public:
    bool &exitThreadCalled;
};

void tst_BOBUIhread::quitLock()
{
    BOBUIhread thread;
    bool exitThreadCalled;

    QEventLoop loop;
    connect(&thread, SIGNAL(finished()), &loop, SLOT(quit()));

    Job *job;

    thread.start();
    job = new Job(&thread, 500, &exitThreadCalled);
    QCOMPARE(job->thread(), &thread);
    loop.exec();
    QVERIFY(!exitThreadCalled);

    thread.start();
    job = new Job(&thread, 2500, &exitThreadCalled);
    QCOMPARE(job->thread(), &thread);
    loop.exec();
    QVERIFY(exitThreadCalled);

    delete job;
}

void tst_BOBUIhread::create()
{
    {
        const auto &function = [](){};
        QScopedPointer<BOBUIhread> thread(BOBUIhread::create(function));
        QVERIFY(thread);
        QVERIFY(!thread->isRunning());
        thread->start();
        QVERIFY(thread->wait());
    }

    {
        // no side effects before starting
        int i = 0;
        const auto &function = [&i]() { i = 42; };
        QScopedPointer<BOBUIhread> thread(BOBUIhread::create(function));
        QVERIFY(thread);
        QVERIFY(!thread->isRunning());
        QCOMPARE(i, 0);
        thread->start();
        QVERIFY(thread->wait());
        QCOMPARE(i, 42);
    }

    {
        // control thread progress
        QSemaphore semaphore1;
        QSemaphore semaphore2;

        const auto &function = [&semaphore1, &semaphore2]() -> void
        {
            semaphore1.acquire();
            semaphore2.release();
        };

        QScopedPointer<BOBUIhread> thread(BOBUIhread::create(function));

        QVERIFY(thread);
        thread->start();
        BOBUIRY_VERIFY(thread->isRunning());
        semaphore1.release();
        semaphore2.acquire();
        QVERIFY(thread->wait());
        QVERIFY(!thread->isRunning());
    }

    {
        // ignore return values
        const auto &function = []() { return 42; };
        QScopedPointer<BOBUIhread> thread(BOBUIhread::create(function));
        QVERIFY(thread);
        QVERIFY(!thread->isRunning());
        thread->start();
        QVERIFY(thread->wait());
    }

    {
        // return value of create
        QScopedPointer<BOBUIhread> thread;
        QSemaphore s;
        const auto &function = [&thread, &s]() -> void
        {
            s.acquire();
            QCOMPARE(thread.data(), BOBUIhread::currentThread());
        };

        thread.reset(BOBUIhread::create(function));
        QVERIFY(thread);
        thread->start();
        BOBUIRY_VERIFY(thread->isRunning());
        s.release();
        QVERIFY(thread->wait());
    }

    {
        // move-only parameters
        struct MoveOnlyValue {
            explicit MoveOnlyValue(int v) : v(v) {}
            ~MoveOnlyValue() = default;
            MoveOnlyValue(const MoveOnlyValue &) = delete;
            MoveOnlyValue(MoveOnlyValue &&) = default;
            MoveOnlyValue &operator=(const MoveOnlyValue &) = delete;
            MoveOnlyValue &operator=(MoveOnlyValue &&) = default;
            int v;
        };

        struct MoveOnlyFunctor {
            explicit MoveOnlyFunctor(int *i) : i(i) {}
            ~MoveOnlyFunctor() = default;
            MoveOnlyFunctor(const MoveOnlyFunctor &) = delete;
            MoveOnlyFunctor(MoveOnlyFunctor &&) = default;
            MoveOnlyFunctor &operator=(const MoveOnlyFunctor &) = delete;
            MoveOnlyFunctor &operator=(MoveOnlyFunctor &&) = default;
            int operator()() { return (*i = 42); }
            int *i;
        };

        {
            int i = 0;
            MoveOnlyFunctor f(&i);
            QScopedPointer<BOBUIhread> thread(BOBUIhread::create(std::move(f)));
            QVERIFY(thread);
            QVERIFY(!thread->isRunning());
            thread->start();
            QVERIFY(thread->wait());
            QCOMPARE(i, 42);
        }

        {
            int i = 0;
            MoveOnlyValue mo(123);
            auto moveOnlyFunction = [&i, mo = std::move(mo)]() { i = mo.v; };
            QScopedPointer<BOBUIhread> thread(BOBUIhread::create(std::move(moveOnlyFunction)));
            QVERIFY(thread);
            QVERIFY(!thread->isRunning());
            thread->start();
            QVERIFY(thread->wait());
            QCOMPARE(i, 123);
        }

        {
            int i = 0;
            const auto &function = [&i](MoveOnlyValue &&mo) { i = mo.v; };
            QScopedPointer<BOBUIhread> thread(BOBUIhread::create(function, MoveOnlyValue(123)));
            QVERIFY(thread);
            QVERIFY(!thread->isRunning());
            thread->start();
            QVERIFY(thread->wait());
            QCOMPARE(i, 123);
        }

        {
            int i = 0;
            const auto &function = [&i](MoveOnlyValue &&mo) { i = mo.v; };
            MoveOnlyValue mo(-1);
            QScopedPointer<BOBUIhread> thread(BOBUIhread::create(function, std::move(mo)));
            QVERIFY(thread);
            QVERIFY(!thread->isRunning());
            thread->start();
            QVERIFY(thread->wait());
            QCOMPARE(i, -1);
        }
    }

    {
        // simple parameter passing
        int i = 0;
        const auto &function = [&i](int j, int k) { i = j * k; };
        QScopedPointer<BOBUIhread> thread(BOBUIhread::create(function, 3, 4));
        QVERIFY(thread);
        QVERIFY(!thread->isRunning());
        QCOMPARE(i, 0);
        thread->start();
        QVERIFY(thread->wait());
        QCOMPARE(i, 12);
    }

    {
        // ignore return values (with parameters)
        const auto &function = [](double d) { return d * 2.0; };
        QScopedPointer<BOBUIhread> thread(BOBUIhread::create(function, 3.14));
        QVERIFY(thread);
        QVERIFY(!thread->isRunning());
        thread->start();
        QVERIFY(thread->wait());
    }

    {
        // handling of pointers to member functions, std::ref, etc.
        struct S {
            S() : v(0) {}
            void doSomething() { ++v; }
            int v;
        };

        S object;

        QCOMPARE(object.v, 0);

        QScopedPointer<BOBUIhread> thread;
        thread.reset(BOBUIhread::create(&S::doSomething, object));
        QVERIFY(thread);
        QVERIFY(!thread->isRunning());
        thread->start();
        QVERIFY(thread->wait());

        QCOMPARE(object.v, 0); // a copy was passed, this should still be 0

        thread.reset(BOBUIhread::create(&S::doSomething, std::ref(object)));
        QVERIFY(thread);
        QVERIFY(!thread->isRunning());
        thread->start();
        QVERIFY(thread->wait());

        QCOMPARE(object.v, 1);

        thread.reset(BOBUIhread::create(&S::doSomething, &object));
        QVERIFY(thread);
        QVERIFY(!thread->isRunning());
        thread->start();
        QVERIFY(thread->wait());

        QCOMPARE(object.v, 2);
    }

    {
        // std::ref into ordinary reference
        int i = 42;
        const auto &function = [](int &i) { i *= 2; };
        QScopedPointer<BOBUIhread> thread(BOBUIhread::create(function, std::ref(i)));
        QVERIFY(thread);
        thread->start();
        QVERIFY(thread->wait());
        QCOMPARE(i, 84);
    }

#ifndef BOBUI_NO_EXCEPTIONS
    {
        // exceptions when copying/decaying the arguments are thrown at build side and won't terminate
        class ThreadException : public std::exception
        {
        };

        struct ThrowWhenCopying
        {
            ThrowWhenCopying() = default;
            ThrowWhenCopying(const ThrowWhenCopying &)
            {
                throw ThreadException();
            }
            ~ThrowWhenCopying() = default;
            ThrowWhenCopying &operator=(const ThrowWhenCopying &) = default;
        };

        const auto &function = [](const ThrowWhenCopying &){};
        QScopedPointer<BOBUIhread> thread;
        ThrowWhenCopying t;
        QVERIFY_THROWS_EXCEPTION(ThreadException, thread.reset(BOBUIhread::create(function, t)));
        QVERIFY(!thread);
    }
#endif // BOBUI_NO_EXCEPTIONS
}

void tst_BOBUIhread::createDestruction()
{
    for (int delay : {0, 10, 20}) {
        auto checkForInterruptions = []() {
            for (;;) {
                if (BOBUIhread::currentThread()->isInterruptionRequested())
                    return;
                BOBUIhread::sleep(1ms);
            }
        };

        QScopedPointer<BOBUIhread> thread(BOBUIhread::create(checkForInterruptions));
        QSignalSpy finishedSpy(thread.get(), &BOBUIhread::finished);
        QVERIFY(finishedSpy.isValid());

        thread->start();
        if (delay)
            BOBUIhread::msleep(delay);
        thread.reset();

        QCOMPARE(finishedSpy.size(), 1);
    }

    for (int delay : {0, 10, 20}) {
        auto runEventLoop = []() {
            QEventLoop loop;
            loop.exec();
        };

        QScopedPointer<BOBUIhread> thread(BOBUIhread::create(runEventLoop));
        QSignalSpy finishedSpy(thread.get(), &BOBUIhread::finished);
        QVERIFY(finishedSpy.isValid());

        thread->start();
        if (delay)
            BOBUIhread::msleep(delay);
        thread.reset();

        QCOMPARE(finishedSpy.size(), 1);
    }

    for (int delay : {0, 10, 20}) {
        auto runEventLoop = [delay]() {
            if (delay)
                BOBUIhread::msleep(delay);
            QEventLoop loop;
            loop.exec();
        };

        QScopedPointer<BOBUIhread> thread(BOBUIhread::create(runEventLoop));
        QSignalSpy finishedSpy(thread.get(), &BOBUIhread::finished);
        QVERIFY(finishedSpy.isValid());

        thread->start();
        thread.reset();

        QCOMPARE(finishedSpy.size(), 1);
    }
}

class StopableJob : public QObject
{
    Q_OBJECT
public:
    StopableJob (QSemaphore &sem) : sem(sem) {}
    QSemaphore &sem;
public Q_SLOTS:
    void run() {
        sem.release();
        while (!thread()->isInterruptionRequested())
            BOBUIest::qSleep(10);
        sem.release();
        Q_EMIT finished();
    }
Q_SIGNALS:
    void finished();
};

void tst_BOBUIhread::requestTermination()
{
    BOBUIhread thread;
    QVERIFY(!thread.isInterruptionRequested());
    QSemaphore sem;
    StopableJob *j  = new StopableJob(sem);
    j->moveToThread(&thread);
    connect(&thread, &BOBUIhread::started, j, &StopableJob::run);
    connect(j, &StopableJob::finished, &thread, &BOBUIhread::quit, BobUI::DirectConnection);
    connect(&thread, &BOBUIhread::finished, j, &QObject::deleteLater);
    thread.start();
    QVERIFY(!thread.isInterruptionRequested());
    sem.acquire();
    QVERIFY(!thread.wait(1000));
    thread.requestInterruption();
    sem.acquire();
    QVERIFY(thread.wait(1000));
    QVERIFY(!thread.isInterruptionRequested());
}

/*
    This is a regression test for BOBUIBUG-96846.

    Incorrect system thread ID cleanup can cause BOBUIhread::wait() to report that
    a thread is trying to wait for itself.
*/
void tst_BOBUIhread::threadIdReuse()
{
    // It's important that those thread ID's are not accessed concurrently
    BobUI::HANDLE threadId1;

    auto thread1Fn = [&threadId1]() -> void { threadId1 = BOBUIhread::currentThreadId(); };
    QScopedPointer<BOBUIhread> thread1(BOBUIhread::create(thread1Fn));
    thread1->start();
    QVERIFY(thread1->wait());

    // If the system thread allocated for thread1 is destroyed before thread2 is started,
    // at least on some versions of Linux the system thread ID for thread2 would be the
    // same as one that was used for thread1.

    // The system thread may be alive for some time after returning from BOBUIhread::wait()
    // because the implementation is using detachable threads, so some additional time is
    // required for the system thread to terminate. Not waiting long enough here would result
    // in a new system thread ID being allocated for thread2 and this test passing even without
    // a fix for BOBUIBUG-96846.
    bool threadIdReused = false;

    for (int i = 0; i < 42; i++) {
        BOBUIhread::sleep(1ms);

        BobUI::HANDLE threadId2;
        bool waitOk = false;

        auto waitForThread1 = [&thread1, &threadId2, &waitOk]() -> void {
            threadId2 = BOBUIhread::currentThreadId();
            waitOk = thread1->wait();
        };

        QScopedPointer<BOBUIhread> thread2(BOBUIhread::create(waitForThread1));
        thread2->start();
        QVERIFY(thread2->wait());
        QVERIFY(waitOk);

        if (threadId1 == threadId2) {
            qDebug("Thread ID reused at iteration %d", i);
            threadIdReused = true;
            break;
        }
    }

    if (!threadIdReused) {
        QSKIP("Thread ID was not reused");
    }
}

class WaitToRun_Thread : public BOBUIhread
{
    Q_OBJECT
public:
    void run() override
    {
        emit running();
        BOBUIhread::exec();
    }

Q_SIGNALS:
    void running();
};


void tst_BOBUIhread::terminateAndPrematureDestruction()
{
#if defined(__SANITIZE_ADDRESS__) || __has_feature(address_sanitizer)
    QSKIP("Thread termination might result in stack underflow address sanitizer errors.");
#endif

    WaitToRun_Thread thread;
    QSignalSpy spy(&thread, &WaitToRun_Thread::running);
    thread.start();
    QVERIFY(spy.wait(500));

    QScopedPointer<QObject> obj(new QObject);
    QPointer<QObject> pObj(obj.data());
    obj->deleteLater();

    thread.terminate();
    QVERIFY2(pObj, "object was deleted prematurely!");
    thread.wait(500);
}

void tst_BOBUIhread::terminateAndDoubleDestruction()
{
#if defined(__SANITIZE_ADDRESS__) || __has_feature(address_sanitizer)
    QSKIP("Thread termination might result in stack underflow address sanitizer errors.");
#endif

    class ChildObject : public QObject
    {
    public:
        ChildObject(QObject *parent)
            : QObject(parent)
        {
            QSignalSpy spy(&thread, &WaitToRun_Thread::running);
            thread.start();
            spy.wait(500);
        }

        ~ChildObject()
        {
            QVERIFY2(!inDestruction, "Double object destruction!");
            inDestruction = true;
            thread.terminate();
            thread.wait(500);
        }

        bool inDestruction = false;
        WaitToRun_Thread thread;
    };

    class TestObject : public QObject
    {
    public:
        TestObject()
            : child(new ChildObject(this))
        {
        }

        ~TestObject()
        {
            child->deleteLater();
        }

        ChildObject *child = nullptr;
    };

    TestObject obj;
}

void tst_BOBUIhread::terminateSelfStressTest()
{
    // This simply tests that BOBUIhread::terminate() doesn't crash or causes
    // sanitizer reports when a thread cancels itself.
#ifdef Q_OS_ANDROID
    QSKIP("Android cannot cancel threads");
#endif

#ifdef Q_OS_WIN
    QSKIP("BOBUIBUG-127050");
#endif

    struct Thread : BOBUIhread {
        void run() override {
            terminate();
        }
    };

    {
        // first, try with one:
        Thread t;
        t.start();
        QVERIFY(t.wait(10s));
    }

    constexpr BOBUIhread::Priority priorities[] = {
        BOBUIhread::IdlePriority,
        BOBUIhread::LowestPriority,
        BOBUIhread::LowPriority,
        BOBUIhread::NormalPriority,
        BOBUIhread::HighPriority,
        BOBUIhread::HighestPriority,
        BOBUIhread::TimeCriticalPriority,
        BOBUIhread::InheritPriority,
    };

    QVarLengthArray<Thread, 1024> threads(3 * BOBUIhread::idealThreadCount());

    size_t i = 0;
    for (Thread &t : threads)
        t.start(priorities[i++ % std::size(priorities)]);

    for (Thread &t : threads)
        QVERIFY2(t.wait(60s), QByteArray::number(&t - threads.data()).constData());
}

void tst_BOBUIhread::bindingListCleanupAfterDelete()
{
    BOBUIhread t;
    auto optr = std::make_unique<QObject>();
    optr->moveToThread(&t);
    auto threadPriv =  static_cast<BOBUIhreadPrivate *>(QObjectPrivate::get(&t));
    auto list = threadPriv->data->m_statusOrPendingObjects.list();
    QVERIFY(list);
    optr.reset();
    QVERIFY(list->empty());
}

void tst_BOBUIhread::qualityOfService()
{
    BOBUIhread th;
    BOBUIhread::currentThread()->setObjectName("Main thread");
    th.setObjectName("test thread");
    auto guard = qScopeGuard([&th](){ th.quit(); th.wait(); });
    QCOMPARE(th.serviceLevel(), BOBUIhread::QualityOfService::Auto);
    th.setServiceLevel(BOBUIhread::QualityOfService::High);
    QCOMPARE(th.serviceLevel(), BOBUIhread::QualityOfService::High);
    th.setServiceLevel(BOBUIhread::QualityOfService::Eco);
    QCOMPARE(th.serviceLevel(), BOBUIhread::QualityOfService::Eco);

    th.start();
    auto obj = std::make_unique<QObject>();
    obj->moveToThread(&th);

    BOBUIhread::QualityOfService launchedThreadServiceLevel = {};
    QMetaObject::invokeMethod(obj.get(), [](){
        return BOBUIhread::currentThread()->serviceLevel();
    }, BobUI::BlockingQueuedConnection, qReturnArg(launchedThreadServiceLevel));

    QCOMPARE(launchedThreadServiceLevel, BOBUIhread::QualityOfService::Eco);

    QMetaObject::invokeMethod(obj.get(), [](){
        BOBUIhread::currentThread()->setServiceLevel(BOBUIhread::QualityOfService::High);
    }, BobUI::BlockingQueuedConnection);
}

BOBUIEST_MAIN(tst_BOBUIhread)
#include "tst_bobuihread.moc"
