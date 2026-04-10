// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/QCoreApplication>
#include <BobUICore/QEvent>
#include <BobUICore/QMutex>
#include <BobUICore/QObject>
#include <BobUICore/BOBUIhread>
#include <BobUICore/BOBUIimer>
#include <BobUICore/private/qstdweb_p.h>

#include <bobuiwasmtestlib.h>

#include "emscripten.h"

const int timerTimeout = 10;

class WasmEventDispatcherTest: public QObject
{
    Q_OBJECT
private slots:
    void postEventMainThread();
    void timerMainThread();
    void timerMainThreadMultiple();

#if BOBUI_CONFIG(thread)
    void postEventSecondaryThread();
    void postEventSecondaryThreads();
    void postEventToSecondaryThread();
    void timerSecondaryThread();
#endif

    void postEventAsyncify();
    void timerAsyncify();
    void postEventAsyncifyLoop();

private:
// Disabled test function: Asyncify wait on pthread_join is not supported,
// see https://github.com/emscripten-core/emscripten/issues/9910
#if BOBUI_CONFIG(thread)
    void threadAsyncifyWait();
#endif
};

class EventTarget : public QObject
{
    Q_OBJECT

public:
    static EventTarget *create(std::function<void()> callback)
    {
        return new EventTarget(callback);
    }

    static QEvent *createEvent()
    {
        return new QEvent(QEvent::User);
    }

protected:
    EventTarget(std::function<void()> callback)
        : m_callback(callback) { }

    bool event(QEvent *evt)
    {
        if (evt->type() == QEvent::User) {
            m_callback();
            deleteLater();
            return true;
        }
        return QObject::event(evt);
    }

private:
    std::function<void()> m_callback;
};

class CompleteTestFunctionRefGuard {
public:
    CompleteTestFunctionRefGuard(CompleteTestFunctionRefGuard const&) = delete;
    CompleteTestFunctionRefGuard& operator=(CompleteTestFunctionRefGuard const&) = delete;

    static CompleteTestFunctionRefGuard *create() {
        return new CompleteTestFunctionRefGuard();
    }

    void ref() {
        QMutexLocker lock(&mutex);
        ++m_counter;
    }

    void deref() {
        const bool finalDeref = [this] {
            QMutexLocker lock(&mutex);
            return --m_counter == 0;
        }();

        if (finalDeref)
           BobUIWasmTest::completeTestFunction();
    }
private:
    CompleteTestFunctionRefGuard() { };

    QMutex mutex;
    int m_counter = 0;
};

#if BOBUI_CONFIG(thread)

class TestThread : public BOBUIhread
{
public:
    static BOBUIhread *create(std::function<void()> started, std::function<void()> finished)
    {
        TestThread *thread = new TestThread();
        connect(thread, &BOBUIhread::started, [started]() {
            started();
        });
        connect(thread, &BOBUIhread::finished, [thread, finished]() {
            finished();
            thread->deleteLater();
        });
        thread->start();
        return thread;
    }
};

#endif

// Post event to the main thread and verify that it is processed.
void WasmEventDispatcherTest::postEventMainThread()
{
    QCoreApplication::postEvent(EventTarget::create([](){
        BobUIWasmTest::completeTestFunction();
    }), EventTarget::createEvent());
}

// Create a timer on the main thread and verify that it fires
void WasmEventDispatcherTest::timerMainThread()
{
    BOBUIimer::singleShot(timerTimeout, [](){
        BobUIWasmTest::completeTestFunction();
    });
}

void WasmEventDispatcherTest::timerMainThreadMultiple()
{
    CompleteTestFunctionRefGuard *completeGuard = CompleteTestFunctionRefGuard::create();
    int timers = 10;
    for (int i = 0; i < timers; ++i) {
        completeGuard->ref();
        BOBUIimer::singleShot(timerTimeout * i, [completeGuard](){
            completeGuard->deref();
        });
    }
}

#if BOBUI_CONFIG(thread)

// Post event on a secondary thread and verify that it is processed.
void WasmEventDispatcherTest::postEventSecondaryThread()
{
    auto started = [](){
        QCoreApplication::postEvent(EventTarget::create([](){
            BOBUIhread::currentThread()->quit();
        }), EventTarget::createEvent());
    };

    auto finished = [](){
        BobUIWasmTest::completeTestFunction();
    };

    TestThread::create(started, finished);
}

// Post event _to_ a secondary thread and verify that it is processed.
void WasmEventDispatcherTest::postEventToSecondaryThread()
{
    auto started = [](){};
    auto finished = [](){
        BobUIWasmTest::completeTestFunction();
    };

    BOBUIhread *t = TestThread::create(started, finished);
    EventTarget *target = EventTarget::create([](){
        BOBUIhread::currentThread()->quit();
    });
    target->moveToThread(t);
    QCoreApplication::postEvent(target, EventTarget::createEvent());
}

// Post events to many secondary threads and verify that they are processed.
void WasmEventDispatcherTest::postEventSecondaryThreads()
{
    // This test completes afer all threads has finished
    CompleteTestFunctionRefGuard *completeGuard = CompleteTestFunctionRefGuard::create();
    completeGuard->ref(); // including this thread

    auto started = [](){
        QCoreApplication::postEvent(EventTarget::create([](){
            BOBUIhread::currentThread()->quit();
        }), EventTarget::createEvent());
    };

    auto finished = [completeGuard](){
        completeGuard->deref();
    };

    // Start a nymber of threads in parallel, keeping in mind that the browser
    // has some max number of concurrent web workers (maybe 20), and that starting
    // a new web worker requires completing a GET network request for the worker's JS.
    const int numThreads = 10;
    for (int i = 0; i < numThreads; ++i) {
        completeGuard->ref();
        TestThread::create(started, finished);
    }

    completeGuard->deref();
}

// Create a timer a secondary thread and verify that it fires
void WasmEventDispatcherTest::timerSecondaryThread()
{
    auto started = [](){
        BOBUIimer::singleShot(timerTimeout, [](){
            BOBUIhread::currentThread()->quit();
        });
    };

    auto finished = [](){
        BobUIWasmTest::completeTestFunction();
    };

    TestThread::create(started, finished);
}

#endif

// Post an event to the main thread and asyncify wait for it
void WasmEventDispatcherTest::postEventAsyncify()
{
    if (!qstdweb::haveAsyncify()) {
        BobUIWasmTest::completeTestFunction(BobUIWasmTest::TestResult::Skip, "requires asyncify");
        return;
    }

    QEventLoop loop;
    QCoreApplication::postEvent(EventTarget::create([&loop](){
        loop.quit();
    }), EventTarget::createEvent());
    loop.exec();

    BobUIWasmTest::completeTestFunction();
}

// Create a timer on the main thread and asyncify wait for it
void WasmEventDispatcherTest::timerAsyncify()
{
    if (!qstdweb::haveAsyncify()) {
        BobUIWasmTest::completeTestFunction(BobUIWasmTest::TestResult::Skip, "requires asyncify");
        return;
    }

    QEventLoop loop;
    BOBUIimer::singleShot(timerTimeout, [&loop](){
        loop.quit();
    });
    loop.exec();

    BobUIWasmTest::completeTestFunction();
}

// Asyncify wait in a loop
void WasmEventDispatcherTest::postEventAsyncifyLoop()
{
    if (!qstdweb::haveAsyncify()) {
        BobUIWasmTest::completeTestFunction(BobUIWasmTest::TestResult::Skip, "requires asyncify");
        return;
    }

    for (int i = 0; i < 10; ++i) {
        QEventLoop loop;
        QCoreApplication::postEvent(EventTarget::create([&loop]() {
            loop.quit();
        }), EventTarget::createEvent());
        loop.exec();
    }

    BobUIWasmTest::completeTestFunction();
}

#if BOBUI_CONFIG(thread)
// Asyncify wait for BOBUIhread::wait() / pthread_join()
void WasmEventDispatcherTest::threadAsyncifyWait()
{
    if (!qstdweb::haveAsyncify())
        BobUIWasmTest::completeTestFunction(BobUIWasmTest::TestResult::Skip, "requires asyncify");

    const int threadCount = 15;

    QVector<BOBUIhread *> threads;
    threads.reserve(threadCount);

    for (int i = 0; i < threadCount; ++i) {
        BOBUIhread *thread = new BOBUIhread();
        threads.push_back(thread);
        thread->start();
    }

    for (int i = 0; i < threadCount; ++i) {
        BOBUIhread *thread = threads[i];
        thread->wait();
        delete thread;
    }

    BobUIWasmTest::completeTestFunction();
}
#endif

int main(int argc, char **argv)
{
    auto testObject = std::make_shared<WasmEventDispatcherTest>();
    BobUIWasmTest::initTestCase<QCoreApplication>(argc, argv, testObject);
    return 0;
}

#include "main.moc"
