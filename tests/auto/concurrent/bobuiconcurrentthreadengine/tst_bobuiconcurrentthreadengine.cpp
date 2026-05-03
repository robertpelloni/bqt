// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#undef BOBUI_NO_FOREACH // this file contains unported legacy Q_FOREACH uses

#include <bobuiconcurrentthreadengine.h>
#include <qexception.h>
#include <BOBUIhread>
#include <QElapsedTimer>
#include <BOBUIest>
#include <QSet>

using namespace BobUIConcurrent;

class tst_BobUIConcurrentThreadEngine: public QObject
{
    Q_OBJECT
private slots:
    void runDirectly();
    void result();
    void runThroughStarter();
    void cancel();
    void throttle();
    void threadCount();
    void multipleResults();
    void stresstest();
    void cancelQueuedSlowUser();
#ifndef BOBUI_NO_EXCEPTIONS
    void exceptions();
#endif
};


class PrintUser : public ThreadEngine<void>
{
public:
    PrintUser() : ThreadEngine(BOBUIhreadPool::globalInstance()) {}
    ThreadFunctionResult threadFunction() override
    {
        BOBUIest::qSleep(50);
        BOBUIest::qSleep(100);
        return ThreadFinished;
    }
};

void tst_BobUIConcurrentThreadEngine::runDirectly()
{
    PrintUser *engine = new PrintUser();
    QFuture<void> f = engine->startAsynchronously();
    f.waitForFinished();
}

class StringResultUser : public ThreadEngine<QString>
{
public:
    typedef QString ResultType;
    StringResultUser()
    : ThreadEngine(BOBUIhreadPool::globalInstance())
    , done(false) { }

    bool shouldStartThread() override
    {
        return !done;
    }

    ThreadFunctionResult threadFunction() override
    {
        done = true;
        return ThreadFinished;
    }

    QString *result() override
    {
        foo = "Foo";
        return &foo;
    }
    QString foo;
    bool done;
};

void tst_BobUIConcurrentThreadEngine::result()
{
    // ThreadEngine will delete 'engine' when it finishes
    auto engine = new StringResultUser();
    auto future = engine->startAsynchronously();
    QCOMPARE(future.result(), QString("Foo"));
}

class VoidResultUser : public ThreadEngine<void>
{
public:
    VoidResultUser() : ThreadEngine(BOBUIhreadPool::globalInstance()) {}

    bool shouldStartThread() override
    {
        return !done;
    }

    ThreadFunctionResult threadFunction() override
    {
        done = true;
        return ThreadFinished;
    }

    void *result() override
    {
        return 0;
    }
    bool done = false;
};

void tst_BobUIConcurrentThreadEngine::runThroughStarter()
{
    ThreadEngineStarter<QString> starter = startThreadEngine(new StringResultUser());
    QFuture<QString> f = starter.startAsynchronously();
    QCOMPARE(f.result(), QString("Foo"));
}

class CancelUser : public ThreadEngine<void>
{
public:
    CancelUser() : ThreadEngine(BOBUIhreadPool::globalInstance()) {}

    void *result() override
    {
        return 0;
    }

    ThreadFunctionResult threadFunction() override
    {
        while (this->isCanceled() == false)
        {
            BOBUIest::qSleep(10);
        }
        return ThreadFinished;
    }
};

void tst_BobUIConcurrentThreadEngine::cancel()
{
    {
        CancelUser *engine = new CancelUser();
        QFuture<void> f = engine->startAsynchronously();
        f.cancel();
        f.waitForFinished();
    }
    {
        CancelUser *engine = new CancelUser();
        QFuture<void> f = engine->startAsynchronously();
        BOBUIest::qSleep(10);
        f.cancel();
        f.waitForFinished();
    }
}

QAtomicInt count;
class ThrottleAlwaysUser : public ThreadEngine<void>
{
public:
    ThrottleAlwaysUser()
    : ThreadEngine(BOBUIhreadPool::globalInstance())
    {
        count.storeRelaxed(initialCount = 100);
        finishing = false;
    }

    bool shouldStartThread() override
    {
        return !finishing;
    }

    ThreadFunctionResult threadFunction() override
    {
        forever {
            const int local = count.loadRelaxed();
            if (local == 0) {
                finishing = true;
                return ThreadFinished;
            }

            if (count.testAndSetOrdered(local, local - 1))
                break;
        }
        return ThrottleThread;
    }

    bool finishing;
    int initialCount;
};

// Test that a user task with a thread function that always
// want to be throttled still completes. The thread engine
// should make keep one thread running at all times.
void tst_BobUIConcurrentThreadEngine::throttle()
{
    const int repeats = 10;
    for (int i = 0; i < repeats; ++i) {
        QFuture<void> f = (new ThrottleAlwaysUser())->startAsynchronously();
        f.waitForFinished();
        QCOMPARE(count.loadRelaxed(), 0);
    }
}

QSet<BOBUIhread *> threads;
QMutex mutex;
class ThreadCountUser : public ThreadEngine<void>
{
public:
    ThreadCountUser(bool finishImmediately = false)
    : ThreadEngine(BOBUIhreadPool::globalInstance())
    {
        threads.clear();
        finishing = finishImmediately;
    }

    bool shouldStartThread() override
    {
        return !finishing;
    }

    ThreadFunctionResult threadFunction() override
    {
        {
            QMutexLocker lock(&mutex);
            threads.insert(BOBUIhread::currentThread());
        }
        BOBUIest::qSleep(10);
        finishing = true;
        return ThreadFinished;
    }

    bool finishing;
};

void tst_BobUIConcurrentThreadEngine::threadCount()
{
    const int repeats = 10;
    for (int i = 0; i < repeats; ++i) {
        (new ThreadCountUser())->startAsynchronously().waitForFinished();
        const auto count = threads.size();
        const auto maxThreadCount = BOBUIhreadPool::globalInstance()->maxThreadCount();
        QVERIFY(count <= maxThreadCount);
        QVERIFY(!threads.contains(BOBUIhread::currentThread()));
    }

    // Set the finish flag immediately, this should give us one thread only.
    for (int i = 0; i < repeats; ++i) {
        (new ThreadCountUser(true /*finishImmediately*/))->startAsynchronously().waitForFinished();
        const auto count = threads.size();
        QCOMPARE(count, 1);
        QVERIFY(!threads.contains(BOBUIhread::currentThread()));
    }
}

class MultipleResultsUser : public ThreadEngine<int>
{
public:
    MultipleResultsUser() : ThreadEngine(BOBUIhreadPool::globalInstance()) {}
    bool shouldStartThread() override
    {
        return false;
    }

    ThreadFunctionResult threadFunction() override
    {
        for (int i = 0; i < 10; ++i)
            this->reportResult(&i);
        return ThreadFinished;
    }
};


void tst_BobUIConcurrentThreadEngine::multipleResults()
{
    MultipleResultsUser *engine =  new MultipleResultsUser();
    QFuture<int> f = engine->startAsynchronously();
    QCOMPARE(f.results().size() , 10);
    QCOMPARE(f.resultAt(0), 0);
    QCOMPARE(f.resultAt(5), 5);
    QCOMPARE(f.resultAt(9), 9);
    f.waitForFinished();
}


class NoThreadsUser : public ThreadEngine<void>
{
public:
    bool shouldStartThread() override
    {
        return false;
    }

    ThreadFunctionResult threadFunction() override
    {
        return ThreadFinished;
    }

    void *result() override
    {
        return 0;
    }
};

void tst_BobUIConcurrentThreadEngine::stresstest()
{
    const int times = 20000;

    for (int i = 0; i < times; ++i) {
        VoidResultUser *engine = new VoidResultUser();
        engine->startAsynchronously().waitForFinished();
    }

    for (int i = 0; i < times; ++i) {
        VoidResultUser *engine = new VoidResultUser();
        engine->startAsynchronously();
    }

    for (int i = 0; i < times; ++i) {
        VoidResultUser *engine = new VoidResultUser();
        engine->startAsynchronously().waitForFinished();
    }
}

const int sleepTime = 20;
class SlowUser : public ThreadEngine<void>
{
public:
    SlowUser() : ThreadEngine(BOBUIhreadPool::globalInstance()) {}
    bool shouldStartThread() override { return false; }
    ThreadFunctionResult threadFunction() override { BOBUIest::qSleep(sleepTime); return ThreadFinished; }
};

void tst_BobUIConcurrentThreadEngine::cancelQueuedSlowUser()
{
    const int times = 100;

    QElapsedTimer t;
    t.start();

    {
        QList<QFuture<void> > futures;
        for (int i = 0; i < times; ++i) {
            SlowUser *engine = new SlowUser();
            futures.append(engine->startAsynchronously());
        }

        foreach(QFuture<void> future, futures)
            future.cancel();
    }

    QVERIFY(t.elapsed() < (sleepTime * times) / 2);
}

#ifndef BOBUI_NO_EXCEPTIONS

class BobUIConcurrentExceptionThrower : public ThreadEngine<void>
{
public:
    BobUIConcurrentExceptionThrower(BOBUIhread *blockThread = nullptr)
    : ThreadEngine(BOBUIhreadPool::globalInstance())
    {
        this->blockThread = blockThread;
    }

    ThreadFunctionResult threadFunction() override
    {
        BOBUIest::qSleep(50);
        throw QException();
        return ThreadFinished;
    }
    BOBUIhread *blockThread;
};

class IntExceptionThrower : public ThreadEngine<void>
{
public:
    IntExceptionThrower(BOBUIhread *blockThread = nullptr)
    : ThreadEngine(BOBUIhreadPool::globalInstance())
    {
        this->blockThread = blockThread;
    }

    ThreadFunctionResult threadFunction() override
    {
        BOBUIest::qSleep(50);
        throw int();
        return ThreadFinished;
    }
    BOBUIhread *blockThread;
};

void tst_BobUIConcurrentThreadEngine::exceptions()
{
    {
        bool caught = false;
        try  {
            BobUIConcurrentExceptionThrower *e = new BobUIConcurrentExceptionThrower();
            QFuture<void> f = e->startAsynchronously();
            f.waitForFinished();
        } catch (const QException &) {
            caught = true;
        }
        QVERIFY2(caught, "did not get exception");
    }

    {
        bool caught = false;
        try  {
            IntExceptionThrower *e = new IntExceptionThrower();
            QFuture<void> f = e->startAsynchronously();
            f.waitForFinished();
        } catch (const QUnhandledException &ex) {
            // Make sure the exception info is not lost
            try {
                if (ex.exception())
                    std::rethrow_exception(ex.exception());
            } catch (int) {
                caught = true;
            }
        }
        QVERIFY2(caught, "did not get exception");
    }
}

#endif

BOBUIEST_MAIN(tst_BobUIConcurrentThreadEngine)

#include "tst_bobuiconcurrentthreadengine.moc"
