// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <bobuiconcurrentmap.h>
#include <qexception.h>
#include <qdebug.h>

#include <BOBUIhread>
#include <QMutex>
#include <BOBUIest>
#include <QSet>
#include <QRandomGenerator>

#include "../testhelper_functions.h"

class tst_BobUIConcurrentMap : public QObject
{
    Q_OBJECT
private slots:
    void map();
    void blockingMap();
    void mapOnRvalue();
    void mapped();
    void mappedThreadPool();
    void mappedWithMoveOnlyCallable();
    void mappedWithGenericCallable();
    void mappedReduced();
    void mappedReducedThreadPool();
    void mappedReducedWithMoveOnlyCallable();
    void mappedReducedWithGenericCallable();
    void mappedReducedDifferentType();
    void mappedReducedInitialValue();
    void mappedReducedInitialValueThreadPool();
    void mappedReducedInitialValueWithMoveOnlyCallable();
    void mappedReducedDifferentTypeInitialValue();
    void mappedReduceOptionConvertableToResultType();
    void assignResult();
    void functionOverloads();
    void noExceptFunctionOverloads();
#ifndef BOBUI_NO_EXCEPTIONS
    void exceptions();
#endif
    void incrementalResults();
    void noDetach();
    void stlContainers();
    void stlContainersLambda();
    void qFutureAssignmentLeak();
    void stressTest();
    void persistentResultTest();
public slots:
    void throttling();
};

using namespace BobUIConcurrent;

void multiplyBy2Immutable([[maybe_unused]] int x)
{
    x *= 2;
}

class MultiplyBy2Immutable
{
public:
    void operator()([[maybe_unused]] int x)
    {
        x *= 2;
    }
};

void multiplyBy2InPlace(int &x)
{
    x *= 2;
}

class MultiplyBy2InPlace
{
public:
    void operator()(int &x)
    {
        x *= 2;
    }
};

class MultiplyBy2InPlaceMoveOnly
{
public:
    MultiplyBy2InPlaceMoveOnly() = default;
    MultiplyBy2InPlaceMoveOnly(MultiplyBy2InPlaceMoveOnly &&) = default;
    MultiplyBy2InPlaceMoveOnly &operator=(MultiplyBy2InPlaceMoveOnly &&other) = default;

    MultiplyBy2InPlaceMoveOnly(const MultiplyBy2InPlaceMoveOnly &) = delete;
    MultiplyBy2InPlaceMoveOnly &operator=(const MultiplyBy2InPlaceMoveOnly &) = delete;

    void operator()(int &x) { x *= 2; }
};

Q_DECLARE_METATYPE(QList<Number>);

void tst_BobUIConcurrentMap::map()
{
    // functors take arguments by reference, modifying the sequence in place
    {
        QList<int> list;
        list << 1 << 2 << 3;

        // functor
        BobUIConcurrent::map(list, MultiplyBy2InPlace()).waitForFinished();
        QCOMPARE(list, QList<int>() << 2 << 4 << 6);
        BobUIConcurrent::map(list.begin(), list.end(), MultiplyBy2InPlace()).waitForFinished();
        QCOMPARE(list, QList<int>() << 4 << 8 << 12);

        // function
        BobUIConcurrent::map(list, multiplyBy2InPlace).waitForFinished();
        QCOMPARE(list, QList<int>() << 8 << 16 << 24);
        BobUIConcurrent::map(list.begin(), list.end(), multiplyBy2InPlace).waitForFinished();
        QCOMPARE(list, QList<int>() << 16 << 32 << 48);

        // bound function
        BobUIConcurrent::map(list, multiplyBy2InPlace).waitForFinished();
        QCOMPARE(list, QList<int>() << 32 << 64 << 96);
        BobUIConcurrent::map(list.begin(), list.end(), multiplyBy2InPlace).waitForFinished();
        QCOMPARE(list, QList<int>() << 64 << 128 << 192);

        // member function
        QList<Number> numberList;
        numberList << 1 << 2 << 3;
        BobUIConcurrent::map(numberList, &Number::multiplyBy2).waitForFinished();
        QCOMPARE(numberList, QList<Number>() << 2 << 4 << 6);
        BobUIConcurrent::map(numberList.begin(), numberList.end(), &Number::multiplyBy2).waitForFinished();
        QCOMPARE(numberList, QList<Number>() << 4 << 8 << 12);

        // lambda
        BobUIConcurrent::map(list, [](int &x){x *= 2;}).waitForFinished();
        QCOMPARE(list, QList<int>() << 128 << 256 << 384);
        BobUIConcurrent::map(list.begin(), list.end(), [](int &x){x *= 2;}).waitForFinished();
        QCOMPARE(list, QList<int>() << 256 << 512 << 768);

        // move-only functor
        BobUIConcurrent::map(list, MultiplyBy2InPlaceMoveOnly()).waitForFinished();
        QCOMPARE(list, QList<int>() << 512 << 1024 << 1536);
        BobUIConcurrent::map(list.begin(), list.end(), MultiplyBy2InPlaceMoveOnly()).waitForFinished();
        QCOMPARE(list, QList<int>() << 1024 << 2048 << 3072);
    }

    // functors don't take arguments by reference, making these no-ops
    {
        QList<int> list;
        list << 1 << 2 << 3;

        // functor
        BobUIConcurrent::map(list, MultiplyBy2Immutable()).waitForFinished();
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);
        BobUIConcurrent::map(list.begin(), list.end(), MultiplyBy2Immutable()).waitForFinished();
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);

        // function
        BobUIConcurrent::map(list, multiplyBy2Immutable).waitForFinished();
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);
        BobUIConcurrent::map(list.begin(), list.end(), multiplyBy2Immutable).waitForFinished();
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);

        // bound function
        BobUIConcurrent::map(list, multiplyBy2Immutable).waitForFinished();
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);
        BobUIConcurrent::map(list.begin(), list.end(), multiplyBy2Immutable).waitForFinished();
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);

        // lambda
        BobUIConcurrent::map(list, []([[maybe_unused]] int x){x *= 2;}).waitForFinished();
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);
        BobUIConcurrent::map(list.begin(), list.end(), []([[maybe_unused]] int x){x *= 2;}).waitForFinished();
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);
    }

    // functors take arguments by reference, modifying the move-only sequence in place
    {
        MoveOnlyVector<int> moveOnlyVector({ 1, 2, 3 });

        // functor
        BobUIConcurrent::map(moveOnlyVector, MultiplyBy2InPlace()).waitForFinished();
        QCOMPARE(moveOnlyVector, MoveOnlyVector<int>({ 2, 4, 6 }));
    }

    // non-template sequences
    {
        NonTemplateSequence list { 1, 2, 3 };
        BobUIConcurrent::map(list, multiplyBy2InPlace).waitForFinished();
        QCOMPARE(list, NonTemplateSequence({ 2, 4, 6 }));
    }

    // custom pool with invalid number of threads
    {
        QList<int> list;
        list << 1 << 2 << 3;
        BOBUIhreadPool pool;
        pool.setMaxThreadCount(0); // explicitly set incorrect value
        // This should not crash
        BobUIConcurrent::map(&pool, list, MultiplyBy2InPlace()).waitForFinished();
        QCOMPARE(list, QList<int>() << 2 << 4 << 6);
    }

#if 0
    // not allowed: map() with immutable sequences makes no sense
    {
        const QList<int> list = QList<int>() << 1 << 2 << 3;

        BobUIConcurrent::map(list, MultiplyBy2Immutable());
        BobUIConcurrent::map(list, multiplyBy2Immutable);
        BobUIConcurrent::map(list, multiplyBy2Immutable);
    }
#endif

#if 0
    // not allowed: in place modification of a temp copy (since temp copy goes out of scope)
    {
        QList<int> list;
        list << 1 << 2 << 3;

        BobUIConcurrent::map(QList<int>(list), MultiplyBy2InPlace());
        BobUIConcurrent::map(QList<int>(list), multiplyBy2);
        BobUIConcurrent::map(QList<int>(list), multiplyBy2InPlace);

        QList<Number> numberList;
        numberList << 1 << 2 << 3;
        BobUIConcurrent::map(QList<Number>(numberList), &Number::multiplyBy2);
    }
#endif

#if 0
    // not allowed: map() on a const list, where functors try to modify the items in the list
    {
        const QList<int> list = QList<int>() << 1 << 2 << 3;

        BobUIConcurrent::map(list, MultiplyBy2InPlace());
        BobUIConcurrent::map(list, multiplyBy2InPlace);
        BobUIConcurrent::map(list, multiplyBy2InPlace);

        const QList<Number> numberList = QList<Number>() << 1 << 2 << 3;
        BobUIConcurrent::map(numberList, &Number::multiplyBy2);
    }
#endif
}

void tst_BobUIConcurrentMap::blockingMap()
{
    // functors take arguments by reference, modifying the sequence in place
    {
        QList<int> list;
        list << 1 << 2 << 3;

        // functor
        BobUIConcurrent::blockingMap(list, MultiplyBy2InPlace());
        QCOMPARE(list, QList<int>() << 2 << 4 << 6);
        BobUIConcurrent::blockingMap(list.begin(), list.end(), MultiplyBy2InPlace());
        QCOMPARE(list, QList<int>() << 4 << 8 << 12);

        // function
        BobUIConcurrent::blockingMap(list, multiplyBy2InPlace);
        QCOMPARE(list, QList<int>() << 8 << 16 << 24);
        BobUIConcurrent::blockingMap(list.begin(), list.end(), multiplyBy2InPlace);
        QCOMPARE(list, QList<int>() << 16 << 32 << 48);

        // bound function
        BobUIConcurrent::blockingMap(list, multiplyBy2InPlace);
        QCOMPARE(list, QList<int>() << 32 << 64 << 96);
        BobUIConcurrent::blockingMap(list.begin(), list.end(), multiplyBy2InPlace);
        QCOMPARE(list, QList<int>() << 64 << 128 << 192);

        // member function
        QList<Number> numberList;
        numberList << 1 << 2 << 3;
        BobUIConcurrent::blockingMap(numberList, &Number::multiplyBy2);
        QCOMPARE(numberList, QList<Number>() << 2 << 4 << 6);
        BobUIConcurrent::blockingMap(numberList.begin(), numberList.end(), &Number::multiplyBy2);
        QCOMPARE(numberList, QList<Number>() << 4 << 8 << 12);

        // lambda
        BobUIConcurrent::blockingMap(list, [](int &x) { x *= 2; });
        QCOMPARE(list, QList<int>() << 128 << 256 << 384);
        BobUIConcurrent::blockingMap(list.begin(), list.end(), [](int &x) { x *= 2; });
        QCOMPARE(list, QList<int>() << 256 << 512 << 768);

        // move-only functor
        BobUIConcurrent::blockingMap(list, MultiplyBy2InPlaceMoveOnly());
        QCOMPARE(list, QList<int>() << 512 << 1024 << 1536);
        BobUIConcurrent::blockingMap(list.begin(), list.end(), MultiplyBy2InPlaceMoveOnly());
        QCOMPARE(list, QList<int>() << 1024 << 2048 << 3072);
    }

    // functors take arguments by reference, modifying the move-only sequence in place
    {
        MoveOnlyVector<int> moveOnlyVector({ 1, 2, 3 });

        // functor
        BobUIConcurrent::blockingMap(moveOnlyVector, MultiplyBy2InPlace());
        QCOMPARE(moveOnlyVector, MoveOnlyVector<int>({ 2, 4, 6 }));
        BobUIConcurrent::blockingMap(moveOnlyVector.begin(), moveOnlyVector.end(),
                                  MultiplyBy2InPlace());
        QCOMPARE(moveOnlyVector, MoveOnlyVector<int>({ 4, 8, 12 }));
    }

    // functors don't take arguments by reference, making these no-ops
    {
        QList<int> list;
        list << 1 << 2 << 3;

        // functor
        BobUIConcurrent::blockingMap(list, MultiplyBy2Immutable());
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);
        BobUIConcurrent::blockingMap(list.begin(), list.end(), MultiplyBy2Immutable());
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);

        // function
        BobUIConcurrent::blockingMap(list, multiplyBy2Immutable);
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);
        BobUIConcurrent::blockingMap(list.begin(), list.end(), multiplyBy2Immutable);
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);

        // bound function
        BobUIConcurrent::blockingMap(list, multiplyBy2Immutable);
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);
        BobUIConcurrent::blockingMap(list.begin(), list.end(), multiplyBy2Immutable);
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);

        // lambda
        BobUIConcurrent::blockingMap(list, []([[maybe_unused]] int x) { x *= 2; });
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);
        BobUIConcurrent::blockingMap(list.begin(), list.end(), []([[maybe_unused]] int x) { x *= 2; });
        QCOMPARE(list, QList<int>() << 1 << 2 << 3);
    }

    // non-template sequences
    {
        NonTemplateSequence list { 1, 2, 3 };
        BobUIConcurrent::blockingMap(list, multiplyBy2InPlace);
        QCOMPARE(list, NonTemplateSequence({ 2, 4, 6 }));
    }

#if 0
    // not allowed: map() with immutable sequences makes no sense
    {
        const QList<int> list = QList<int>() << 1 << 2 << 3;

        BobUIConcurrent::blockingMap(list, MultiplyBy2Immutable());
        BobUIConcurrent::blockkng::map(list, multiplyBy2Immutable);
        BobUIConcurrent::blockingMap(list, multiplyBy2Immutable);
    }
#endif

#if 0
    // not allowed: in place modification of a temp copy (since temp copy goes out of scope)
    {
        QList<int> list;
        list << 1 << 2 << 3;

        BobUIConcurrent::blockingMap(QList<int>(list), MultiplyBy2InPlace());
        BobUIConcurrent::blockingMap(QList<int>(list), multiplyBy2);
        BobUIConcurrent::blockingMap(QList<int>(list), multiplyBy2InPlace);

        QList<Number> numberList;
        numberList << 1 << 2 << 3;
        BobUIConcurrent::blockingMap(QList<Number>(numberList), &Number::multiplyBy2);
    }
#endif

#if 0
    // not allowed: map() on a const list, where functors try to modify the items in the list
    {
        const QList<int> list = QList<int>() << 1 << 2 << 3;

        BobUIConcurrent::blockingMap(list, MultiplyBy2InPlace());
        BobUIConcurrent::blockingMap(list, multiplyBy2InPlace);
        BobUIConcurrent::blockingMap(list, multiplyBy2InPlace);

        const QList<Number> numberList = QList<Number>() << 1 << 2 << 3;
        BobUIConcurrent::blockingMap(numberList, &Number::multiplyBy2);
    }
#endif
}

void tst_BobUIConcurrentMap::mapOnRvalue()
{
    struct ListRange
    {
        using iterator = QList<int>::iterator;
        ListRange(iterator b, iterator e) : m_begin(b), m_end(e) { }

        iterator begin() const { return m_begin; }
        iterator end() const { return m_end; }

    private:
        iterator m_begin;
        iterator m_end;
    };

    const QList expected { 1, 4, 6, 4 };
    {
        QList list { 1, 2, 3, 4 };
        auto future =
                BobUIConcurrent::map(ListRange(list.begin() + 1, list.end() - 1), multiplyBy2InPlace);
        future.waitForFinished();
        QCOMPARE(list, expected);
    }

    {
        QList list { 1, 2, 3, 4 };
        BOBUIhreadPool pool;
        auto future = BobUIConcurrent::map(&pool, ListRange(list.begin() + 1, list.end() - 1),
                                        multiplyBy2InPlace);
        future.waitForFinished();
        QCOMPARE(list, expected);
    }

    {
        QList list { 1, 2, 3, 4 };
        BobUIConcurrent::blockingMap(ListRange(list.begin() + 1, list.end() - 1), multiplyBy2InPlace);
        QCOMPARE(list, expected);
    }

    {
        QList list { 1, 2, 3, 4 };
        BOBUIhreadPool pool;
        BobUIConcurrent::blockingMap(&pool, ListRange(list.begin() + 1, list.end() - 1),
                                  multiplyBy2InPlace);
        QCOMPARE(list, expected);
    }
}

int multiplyBy2(int x)
{
    int y = x * 2;
    return y;
}

class MultiplyBy2
{
public:
    int operator()(int x) const
    {
        int y = x * 2;
        return y;
    }
};

class MultiplyBy2MoveOnly
{
public:
    MultiplyBy2MoveOnly() = default;
    MultiplyBy2MoveOnly(MultiplyBy2MoveOnly &&) = default;
    MultiplyBy2MoveOnly &operator=(MultiplyBy2MoveOnly &&other) = default;

    MultiplyBy2MoveOnly(const MultiplyBy2MoveOnly &) = delete;
    MultiplyBy2MoveOnly &operator=(const MultiplyBy2MoveOnly &) = delete;

    int operator()(int x) const
    {
        int y = x * 2;
        return y;
    }
};

double intToDouble(int x)
{
    return double(x);
}

class IntToDouble
{
public:
    double operator()(int x) const
    {
        return double(x);
    }
};

int stringToInt(const QString &string)
{
    return string.toInt();
}

class StringToInt
{
public:
    int operator()(const QString &string) const
    {
        return string.toInt();
    }
};

#define CHECK_FAIL(message) \
do {\
    if (BOBUIest::currentTestFailed())\
        QFAIL("failed one line above on " message);\
} while (false)

template <typename SourceObject, typename ResultObject, typename MapObject>
void testMapped(const QList<SourceObject> &sourceObjectList, const QList<ResultObject> &expectedResult, MapObject mapObject)
{
    const QList<ResultObject> result1 = BobUIConcurrent::mapped(
                sourceObjectList, mapObject).results();
    QCOMPARE(result1, expectedResult);

    const QList<ResultObject> result2 = BobUIConcurrent::mapped(
                sourceObjectList.constBegin(), sourceObjectList.constEnd(), mapObject).results();
    QCOMPARE(result2, expectedResult);

    const QList<ResultObject> result3 = BobUIConcurrent::blockingMapped(
                sourceObjectList, mapObject);
    QCOMPARE(result3, expectedResult);

    const QList<ResultObject> result4 = BobUIConcurrent::blockingMapped<QList<ResultObject>>(
                sourceObjectList.constBegin(), sourceObjectList.constEnd(), mapObject);
    QCOMPARE(result4, expectedResult);
}

void tst_BobUIConcurrentMap::mapped()
{
    const QList<int> intList {1, 2, 3};
    const QList<Number> numberList {1, 2, 3};
    const QList<QString> stringList {"1", "2", "3"};
    const QList<double> doubleList {1.0, 2.0, 3.0};

    const QList<int> intListMultipiedBy2 {2, 4, 6};
    const QList<Number> numberListMultipiedBy2 {2, 4, 6};

    auto lambdaMultiplyBy2 = [](int x) {
        return 2 * x;
    };
    auto lambdaIntToDouble = [](int x) {
        return double(x);
    };
    auto lambdaStringToInt = [](const QString &string) {
        return string.toInt();
    };

    testMapped(intList, intListMultipiedBy2, MultiplyBy2());
    CHECK_FAIL("functor");
    testMapped(intList, intListMultipiedBy2, multiplyBy2);
    CHECK_FAIL("function");
    testMapped(numberList, numberListMultipiedBy2, &Number::multipliedBy2);
    CHECK_FAIL("member");
    testMapped(intList, intListMultipiedBy2, lambdaMultiplyBy2);
    CHECK_FAIL("lambda");

    // change the value_type, same container
    testMapped(intList, doubleList, IntToDouble());
    CHECK_FAIL("functor");
    testMapped(intList, doubleList, intToDouble);
    CHECK_FAIL("function");
    testMapped(numberList, stringList, &Number::toString);
    CHECK_FAIL("member");
    testMapped(intList, doubleList, lambdaIntToDouble);
    CHECK_FAIL("lambda");

    // change the value_type
    testMapped(stringList, intList, StringToInt());
    CHECK_FAIL("functor");
    testMapped(stringList, intList, stringToInt);
    CHECK_FAIL("function");
    testMapped(numberList, intList, &Number::toInt);
    CHECK_FAIL("member");
    testMapped(stringList, intList, lambdaStringToInt);
    CHECK_FAIL("lambda");

    // not allowed: const member function where all arguments have default values
#if 0
    testMapped(stringList, intList, &QString::toInt);
    CHECK_FAIL("member");
#endif

    {
        // non-template sequences
        NonTemplateSequence list { 1, 2, 3 };

        auto future = BobUIConcurrent::mapped(list, multiplyBy2);
        QCOMPARE(future.results(), QList({ 2, 4, 6 }));

        auto result = BobUIConcurrent::blockingMapped(list, multiplyBy2);
        QCOMPARE(result, NonTemplateSequence({ 2, 4, 6 }));
    }

    {
        // rvalue sequences
        auto future = BobUIConcurrent::mapped(std::vector { 1, 2, 3 }, multiplyBy2);
        QCOMPARE(future.results(), QList<int>({ 2, 4, 6 }));

        auto result = BobUIConcurrent::blockingMapped(std::vector { 1, 2, 3 }, multiplyBy2);
        QCOMPARE(result, std::vector<int>({ 2, 4, 6 }));
    }

    {
        // move only sequences
        auto future = BobUIConcurrent::mapped(MoveOnlyVector<int>({ 1, 2, 3 }), multiplyBy2);
        QCOMPARE(future.results(), QList<int>({ 2, 4, 6 }));

        auto result = BobUIConcurrent::blockingMapped<std::vector<int>>(
                MoveOnlyVector<int>({ 1, 2, 3 }), multiplyBy2);
        QCOMPARE(result, std::vector<int>({ 2, 4, 6 }));
    }
}

static QSemaphore semaphore(1);
static QSet<BOBUIhread *> workingThreads;

void storeCurrentThread()
{
    semaphore.acquire();
    workingThreads.insert(BOBUIhread::currentThread());
    semaphore.release();
}

int threadCount()
{
    semaphore.acquire();
    const int count = workingThreads.size();
    semaphore.release();
    return count;
}

template <typename SourceObject, typename ResultObject, typename MapObject>
void testMappedThreadPool(BOBUIhreadPool *pool,
                          const QList<SourceObject> &sourceObjectList,
                          const QList<ResultObject> &expectedResult,
                          MapObject mapObject)
{
    const QList<ResultObject> result1 = BobUIConcurrent::mapped(pool,
                sourceObjectList, mapObject).results();
    QCOMPARE(result1, expectedResult);
    QCOMPARE(threadCount(), 1); // ensure the only one thread was working

    const QList<ResultObject> result2 = BobUIConcurrent::mapped(pool,
                sourceObjectList.constBegin(), sourceObjectList.constEnd(), mapObject).results();
    QCOMPARE(result2, expectedResult);
    QCOMPARE(threadCount(), 1); // ensure the only one thread was working

    const QList<ResultObject> result3 = BobUIConcurrent::blockingMapped(pool,
                sourceObjectList, mapObject);
    QCOMPARE(result3, expectedResult);
    QCOMPARE(threadCount(), 1); // ensure the only one thread was working

    const QList<ResultObject> result4 = BobUIConcurrent::blockingMapped<QList<ResultObject>>(pool,
                sourceObjectList.constBegin(), sourceObjectList.constEnd(), mapObject);
    QCOMPARE(result4, expectedResult);
    QCOMPARE(threadCount(), 1); // ensure the only one thread was working
}

int multiplyBy3(int x)
{
    storeCurrentThread();
    return x * 3;
}

class MultiplyBy3
{
public:
    int operator()(int x) const
    {
        storeCurrentThread();
        return x * 3;
    }
};

void tst_BobUIConcurrentMap::mappedThreadPool()
{
    const QList<int> intList {1, 2, 3};
    const QList<int> intListMultipiedBy3 {3, 6, 9};

    auto lambdaMultiplyBy3 = [](int x) {
        storeCurrentThread();
        return x * 3;
    };

    BOBUIhreadPool pool;
    pool.setMaxThreadCount(1);
    QCOMPARE(semaphore.available(), 1);
    workingThreads.clear();

    testMappedThreadPool(&pool, intList, intListMultipiedBy3, MultiplyBy3());
    CHECK_FAIL("functor");
    testMappedThreadPool(&pool, intList, intListMultipiedBy3, multiplyBy3);
    CHECK_FAIL("function");
    testMappedThreadPool(&pool, intList, intListMultipiedBy3, lambdaMultiplyBy3);
    CHECK_FAIL("lambda");

    {
        // non-template sequences
        NonTemplateSequence list { 1, 2, 3 };

        auto future = BobUIConcurrent::mapped(&pool, list, multiplyBy2);
        QCOMPARE(future.results(), QList({ 2, 4, 6 }));

        auto result = BobUIConcurrent::blockingMapped(&pool, list, multiplyBy2);
        QCOMPARE(result, NonTemplateSequence({ 2, 4, 6 }));
    }

    {
        // rvalue sequences
        auto future = BobUIConcurrent::mapped(&pool, std::vector { 1, 2, 3 }, multiplyBy2);
        QCOMPARE(future.results(), QList<int>({ 2, 4, 6 }));

        auto result = BobUIConcurrent::blockingMapped<std::vector<int>>(&pool, std::vector { 1, 2, 3 },
                                                                     multiplyBy2);
        QCOMPARE(result, std::vector<int>({ 2, 4, 6 }));
    }
    {
        // move only sequences
        auto future = BobUIConcurrent::mapped(&pool, MoveOnlyVector<int>({ 1, 2, 3 }), multiplyBy2);
        QCOMPARE(future.results(), QList<int>({ 2, 4, 6 }));

        auto result = BobUIConcurrent::blockingMapped<std::vector<int>>(
                &pool, MoveOnlyVector<int>({ 1, 2, 3 }), multiplyBy2);
        QCOMPARE(result, std::vector<int>({ 2, 4, 6 }));
    }
}

void tst_BobUIConcurrentMap::mappedWithMoveOnlyCallable()
{
    const QList<int> intList { 1, 2, 3 };
    const QList<int> intListMultipiedBy2 { 2, 4, 6 };
    {
        const auto result = BobUIConcurrent::mapped(intList, MultiplyBy2()).results();
        QCOMPARE(result, intListMultipiedBy2);
    }
    {
        const auto result =
                BobUIConcurrent::mapped(intList.begin(), intList.end(), MultiplyBy2()).results();
        QCOMPARE(result, intListMultipiedBy2);
    }
    {
        const auto result = BobUIConcurrent::blockingMapped(intList, MultiplyBy2());
        QCOMPARE(result, intListMultipiedBy2);
    }
    {
        const auto result = BobUIConcurrent::blockingMapped<QList<int>>(intList.begin(), intList.end(),
                                                                     MultiplyBy2());
        QCOMPARE(result, intListMultipiedBy2);
    }

    BOBUIhreadPool pool;
    {
        const auto result = BobUIConcurrent::mapped(&pool, intList, MultiplyBy2()).results();
        QCOMPARE(result, intListMultipiedBy2);
    }
    {
        const auto result = BobUIConcurrent::mapped(
                    &pool, intList.begin(), intList.end(), MultiplyBy2()).results();
        QCOMPARE(result, intListMultipiedBy2);
    }
    {
        const auto result = BobUIConcurrent::blockingMapped(&pool, intList, MultiplyBy2());
        QCOMPARE(result, intListMultipiedBy2);
    }
    {
        const auto result = BobUIConcurrent::blockingMapped<QList<int>>(&pool, intList.begin(),
                                                                     intList.end(), MultiplyBy2());
        QCOMPARE(result, intListMultipiedBy2);
    }
}

void tst_BobUIConcurrentMap::mappedWithGenericCallable()
{
    QList<int> intList{1, 2, 3};
    QList<std::string> expectedResult{"1", "2", "3"};

    auto toString = [](const auto &el) { return std::to_string(el); };
    {
        const auto res = BobUIConcurrent::mapped(intList, toString).results();
        QCOMPARE_EQ(res, expectedResult);
    }
    {
        const auto res = BobUIConcurrent::blockingMapped(intList, toString);
        QCOMPARE_EQ(res, expectedResult);
    }

    BOBUIhreadPool pool;
    {
        const auto res = BobUIConcurrent::mapped(&pool, intList, toString).results();
        QCOMPARE_EQ(res, expectedResult);
    }
    {
        const auto res = BobUIConcurrent::blockingMapped(&pool, intList, toString);
        QCOMPARE_EQ(res, expectedResult);
    }
}

int intSquare(int x)
{
    return x * x;
}

class IntSquare
{
public:
    int operator()(int x)
    {
        return x * x;
    }
};

Number numberSquare(Number x)
{
    return Number(x.toInt() * x.toInt());
}

class NumberSquare
{
public:
    Number operator()(Number x)
    {
        return Number(x.toInt() * x.toInt());
    }
};

template <typename SourceObject, typename ResultObject, typename MapObject, typename ReduceObject>
void testMappedReduced(const QList<SourceObject> &sourceObjectList, const ResultObject &expectedResult, MapObject mapObject, ReduceObject reduceObject)
{
    // Result type is passed explicitly
    {
        const ResultObject result1 = BobUIConcurrent::mappedReduced<ResultObject>(
                    sourceObjectList, mapObject, reduceObject).result();
        QCOMPARE(result1, expectedResult);

        const ResultObject result2 = BobUIConcurrent::mappedReduced<ResultObject>(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    mapObject, reduceObject).result();
        QCOMPARE(result2, expectedResult);

        const ResultObject result3 = BobUIConcurrent::blockingMappedReduced<ResultObject>(
                    sourceObjectList, mapObject, reduceObject);
        QCOMPARE(result3, expectedResult);

        const ResultObject result4 = BobUIConcurrent::blockingMappedReduced<ResultObject>(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(), mapObject, reduceObject);
        QCOMPARE(result4, expectedResult);
    }

    // Result type is deduced
    {
        const ResultObject result1 = BobUIConcurrent::mappedReduced(
                    sourceObjectList, mapObject, reduceObject).result();
        QCOMPARE(result1, expectedResult);

        const ResultObject result2 = BobUIConcurrent::mappedReduced(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    mapObject, reduceObject).result();
        QCOMPARE(result2, expectedResult);

        const ResultObject result3 = BobUIConcurrent::blockingMappedReduced(
                    sourceObjectList, mapObject, reduceObject);
        QCOMPARE(result3, expectedResult);

        const ResultObject result4 = BobUIConcurrent::blockingMappedReduced(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(), mapObject, reduceObject);
        QCOMPARE(result4, expectedResult);
    }
}

template <typename SourceObject, typename ResultObject, typename MapObject, typename ReduceObject>
void testMappedReduced(const QList<SourceObject> &sourceObjectList, const ResultObject &expectedResult, MapObject mapObject, ReduceObject reduceObject, BobUIConcurrent::ReduceOptions options)
{
    const ResultObject result1 = BobUIConcurrent::mappedReduced(
                sourceObjectList, mapObject, reduceObject, options).result();
    QCOMPARE(result1, expectedResult);

    const ResultObject result2 =
            BobUIConcurrent::mappedReduced(sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                                        mapObject, reduceObject, options).result();
    QCOMPARE(result2, expectedResult);

    const ResultObject result3 = BobUIConcurrent::blockingMappedReduced(
                sourceObjectList, mapObject, reduceObject, options);
    QCOMPARE(result3, expectedResult);

    const ResultObject result4 = BobUIConcurrent::blockingMappedReduced(
                sourceObjectList.constBegin(), sourceObjectList.constEnd(), mapObject, reduceObject, options);
    QCOMPARE(result4, expectedResult);
}

void tst_BobUIConcurrentMap::mappedReduced()
{
    const QList<int> intList {1, 2, 3};
    const QList<int> intListOfSquares {1, 4, 9};
    const QList<Number> numberList {1, 2, 3};
    const int sum = 6;
    const int sumOfSquares = 14;

    void (QList<int>::*push_back)(QList<int>::parameter_type) = &QList<int>::push_back;

    auto lambdaSquare = [](int x) {
        return x * x;
    };
    auto lambdaSumReduce = [](int &sum, int x) {
        sum += x;
    };

    // FUNCTOR-other
    testMappedReduced(intList, sumOfSquares, IntSquare(), IntSumReduce());
    CHECK_FAIL("functor-functor");
    testMappedReduced(intList, sumOfSquares, IntSquare(), intSumReduce);
    CHECK_FAIL("functor-function");
    testMappedReduced(intList, intListOfSquares, IntSquare(), push_back, OrderedReduce);
    CHECK_FAIL("functor-member");
    testMappedReduced(intList, sumOfSquares, IntSquare(), lambdaSumReduce);
    CHECK_FAIL("functor-lambda");

    // FUNCTION-other
    testMappedReduced(intList, sumOfSquares, intSquare, IntSumReduce());
    CHECK_FAIL("function-functor");
    testMappedReduced(intList, sumOfSquares, intSquare, intSumReduce);
    CHECK_FAIL("function-function");
    testMappedReduced(intList, intListOfSquares, intSquare, push_back, OrderedReduce);
    CHECK_FAIL("function-member");
    testMappedReduced(intList, sumOfSquares, intSquare, lambdaSumReduce);
    CHECK_FAIL("function-lambda");

    // MEMBER-other
    testMappedReduced(numberList, sum, &Number::toInt, IntSumReduce());
    CHECK_FAIL("member-functor");
    testMappedReduced(numberList, sum, &Number::toInt, intSumReduce);
    CHECK_FAIL("member-function");
    testMappedReduced(numberList, intList, &Number::toInt, push_back, OrderedReduce);
    CHECK_FAIL("member-member");
    testMappedReduced(numberList, sum, &Number::toInt, lambdaSumReduce);
    CHECK_FAIL("member-lambda");

    // LAMBDA-other
    testMappedReduced(intList, sumOfSquares, lambdaSquare, IntSumReduce());
    CHECK_FAIL("lambda-functor");
    testMappedReduced(intList, sumOfSquares, lambdaSquare, intSumReduce);
    CHECK_FAIL("lambda-function");
    testMappedReduced(intList, intListOfSquares, lambdaSquare, push_back, OrderedReduce);
    CHECK_FAIL("lambda-member");
    testMappedReduced(intList, sumOfSquares, lambdaSquare, lambdaSumReduce);
    CHECK_FAIL("lambda-lambda");

    {
        // non-template sequences
        NonTemplateSequence list { 1, 2, 3 };

        auto future = BobUIConcurrent::mappedReduced(list, multiplyBy2, intSumReduce);
        QCOMPARE(future.result(), 12);

        auto result = BobUIConcurrent::blockingMappedReduced(list, multiplyBy2, intSumReduce);
        QCOMPARE(result, 12);
    }

    {
        // rvalue sequences
        auto future = BobUIConcurrent::mappedReduced(std::vector { 1, 2, 3 }, intSquare, intSumReduce);
        QCOMPARE(future.result(), sumOfSquares);

        auto result = BobUIConcurrent::blockingMappedReduced(std::vector { 1, 2, 3 }, intSquare,
                                                          intSumReduce);
        QCOMPARE(result, sumOfSquares);
    }

    {
        // move only sequences
        auto future =
                BobUIConcurrent::mappedReduced(MoveOnlyVector<int>({ 1, 2, 3 }), intSquare, intSumReduce);
        QCOMPARE(future.result(), sumOfSquares);

        auto result = BobUIConcurrent::blockingMappedReduced(MoveOnlyVector<int>({ 1, 2, 3 }),
                                                          intSquare, intSumReduce);
        QCOMPARE(result, sumOfSquares);
    }
}

template <typename SourceObject, typename ResultObject, typename MapObject, typename ReduceObject>
void testMappedReducedThreadPool(BOBUIhreadPool *pool,
                                 const QList<SourceObject> &sourceObjectList,
                                 const ResultObject &expectedResult,
                                 MapObject mapObject,
                                 ReduceObject reduceObject)
{
    // Result type is passed explicitly
    {
        const ResultObject result1 = BobUIConcurrent::mappedReduced<ResultObject>(
                    pool, sourceObjectList, mapObject, reduceObject).result();
        QCOMPARE(result1, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result2 =
                BobUIConcurrent::mappedReduced<ResultObject>(pool, sourceObjectList.constBegin(),
                                                          sourceObjectList.constEnd(), mapObject,
                                                          reduceObject).result();
        QCOMPARE(result2, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result3 = BobUIConcurrent::blockingMappedReduced<ResultObject>(
                    pool, sourceObjectList, mapObject, reduceObject);
        QCOMPARE(result3, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result4 = BobUIConcurrent::blockingMappedReduced<ResultObject>(
                    pool, sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    mapObject, reduceObject);
        QCOMPARE(result4, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working
    }

    // Result type is deduced
    {
        const ResultObject result1 = BobUIConcurrent::mappedReduced(
                    pool, sourceObjectList, mapObject, reduceObject).result();
        QCOMPARE(result1, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result2 =
                BobUIConcurrent::mappedReduced(pool, sourceObjectList.constBegin(),
                                            sourceObjectList.constEnd(), mapObject,
                                            reduceObject).result();
        QCOMPARE(result2, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result3 = BobUIConcurrent::blockingMappedReduced(
                    pool, sourceObjectList, mapObject, reduceObject);
        QCOMPARE(result3, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result4 = BobUIConcurrent::blockingMappedReduced(
                    pool, sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    mapObject, reduceObject);
        QCOMPARE(result4, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working
    }
}

int intCube(int x)
{
    storeCurrentThread();
    return x * x * x;
}

class IntCube
{
public:
    int operator()(int x)
    {
        storeCurrentThread();
        return x * x * x;
    }
};

void tst_BobUIConcurrentMap::mappedReducedThreadPool()
{
    const QList<int> intList {1, 2, 3};
    const int sumOfCubes = 36;

    auto lambdaCube = [](int x) {
        return x * x * x;
    };
    auto lambdaSumReduce = [](int &sum, int x) {
        sum += x;
    };

    BOBUIhreadPool pool;
    pool.setMaxThreadCount(1);
    QCOMPARE(semaphore.available(), 1);
    workingThreads.clear();

    // FUNCTOR-other
    testMappedReducedThreadPool(&pool, intList, sumOfCubes, IntCube(), IntSumReduce());
    CHECK_FAIL("functor-functor");
    testMappedReducedThreadPool(&pool, intList, sumOfCubes, IntCube(), intSumReduce);
    CHECK_FAIL("functor-function");
    testMappedReducedThreadPool(&pool, intList, sumOfCubes, IntCube(), lambdaSumReduce);
    CHECK_FAIL("functor-lambda");

    // FUNCTION-other
    testMappedReducedThreadPool(&pool, intList, sumOfCubes, intCube, IntSumReduce());
    CHECK_FAIL("function-functor");
    testMappedReducedThreadPool(&pool, intList, sumOfCubes, intCube, intSumReduce);
    CHECK_FAIL("function-function");
    testMappedReducedThreadPool(&pool, intList, sumOfCubes, intCube, lambdaSumReduce);
    CHECK_FAIL("function-lambda");

    // LAMBDA-other
    testMappedReducedThreadPool(&pool, intList, sumOfCubes, lambdaCube, IntSumReduce());
    CHECK_FAIL("lambda-functor");
    testMappedReducedThreadPool(&pool, intList, sumOfCubes, lambdaCube, intSumReduce);
    CHECK_FAIL("lambda-function");
    testMappedReducedThreadPool(&pool, intList, sumOfCubes, lambdaCube, lambdaSumReduce);
    CHECK_FAIL("lambda-lambda");

    {
        // non-template sequences
        NonTemplateSequence list { 1, 2, 3 };

        auto future = BobUIConcurrent::mappedReduced(&pool, list, multiplyBy2, intSumReduce);
        QCOMPARE(future.result(), 12);

        auto result = BobUIConcurrent::blockingMappedReduced(&pool, list, multiplyBy2, intSumReduce);
        QCOMPARE(result, 12);
    }

    {
        // rvalue sequences
        auto future =
                BobUIConcurrent::mappedReduced(&pool, std::vector { 1, 2, 3 }, intCube, intSumReduce);
        QCOMPARE(future.result(), sumOfCubes);

        auto result = BobUIConcurrent::blockingMappedReduced(&pool, std::vector { 1, 2, 3 }, intCube,
                                                          intSumReduce);
        QCOMPARE(result, sumOfCubes);
    }

    {
        // move only sequences
        auto future = BobUIConcurrent::mappedReduced(&pool, MoveOnlyVector<int>({ 1, 2, 3 }), intCube,
                                                  intSumReduce);
        QCOMPARE(future.result(), sumOfCubes);

        auto result = BobUIConcurrent::blockingMappedReduced(&pool, MoveOnlyVector<int>({ 1, 2, 3 }),
                                                          intCube, intSumReduce);
        QCOMPARE(result, sumOfCubes);
    }

    {
        // pool with invalid number of threads
        BOBUIhreadPool pool;
        pool.setMaxThreadCount(0); // explicitly set incorrect value

        // This should not crash
        NonTemplateSequence list { 1, 2, 3 };
        auto future = BobUIConcurrent::mappedReduced(&pool, list, multiplyBy2, intSumReduce);
        QCOMPARE(future.result(), 12);
    }
}

void tst_BobUIConcurrentMap::mappedReducedWithMoveOnlyCallable()
{
    const QList<int> intList { 1, 2, 3 };
    const auto sum = 12;
    {
        const auto result = BobUIConcurrent::mappedReduced(
                    intList, MultiplyBy2(), IntSumReduceMoveOnly()).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result =
                BobUIConcurrent::mappedReduced(intList.begin(), intList.end(),
                                            MultiplyBy2(), IntSumReduceMoveOnly()).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingMappedReduced(intList, MultiplyBy2(),
                                                                IntSumReduceMoveOnly());
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingMappedReduced(
                intList.begin(), intList.end(), MultiplyBy2(), IntSumReduceMoveOnly());
        QCOMPARE(result, sum);
    }

    BOBUIhreadPool pool;
    {
        const auto result = BobUIConcurrent::mappedReduced(&pool, intList, MultiplyBy2(),
                                                        IntSumReduceMoveOnly()).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result =
                BobUIConcurrent::mappedReduced(&pool, intList.begin(), intList.end(),
                                            MultiplyBy2(), IntSumReduceMoveOnly()).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingMappedReduced(&pool, intList, MultiplyBy2(),
                                                                IntSumReduceMoveOnly());
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingMappedReduced(
                &pool, intList.begin(), intList.end(), MultiplyBy2(), IntSumReduceMoveOnly());
        QCOMPARE(result, sum);
    }
}

void tst_BobUIConcurrentMap::mappedReducedWithGenericCallable()
{
    QList<int> intList{1, 2, 3};
    const std::string expectedResult{"123"};

    auto toString = [](const auto &el) { return std::to_string(el); };
    auto appendString = [](auto &res, const auto &el) { res.append(el); };
    {
        const auto res = BobUIConcurrent::mappedReduced<std::string>(intList, toString, appendString,
                                                                  OrderedReduce).result();
        QCOMPARE_EQ(res, expectedResult);
    }
    {
        const auto res = BobUIConcurrent::blockingMappedReduced<std::string>(intList, toString,
                                                                          appendString,
                                                                          OrderedReduce);
        QCOMPARE_EQ(res, expectedResult);
    }

    BOBUIhreadPool pool;
    {
        const auto res = BobUIConcurrent::mappedReduced<std::string>(&pool, intList, toString,
                                                                  appendString,
                                                                  OrderedReduce).result();
        QCOMPARE_EQ(res, expectedResult);
    }
    {
        const auto res = BobUIConcurrent::blockingMappedReduced<std::string>(&pool, intList, toString,
                                                                          appendString,
                                                                          OrderedReduce);
        QCOMPARE_EQ(res, expectedResult);
    }
}

void tst_BobUIConcurrentMap::mappedReducedDifferentType()
{
    const QList<int> intList {1, 2, 3};
    const QList<Number> numberList {1, 2, 3};
    const int sumOfSquares = 14;

    auto lambdaSquare = [](Number x) {
        return Number(x.toInt() * x.toInt());
    };
    auto lambdaSumReduce = [](int &sum, Number x) {
        sum += x.toInt();
    };

    // Test the case where reduce function of the form:
    // V function(T &result, const U &intermediate)
    // has T and U types different.

    // FUNCTOR-other
    testMappedReduced(intList, sumOfSquares, NumberSquare(), NumberSumReduce());
    CHECK_FAIL("functor-functor");
    testMappedReduced(intList, sumOfSquares, NumberSquare(), numberSumReduce);
    CHECK_FAIL("functor-function");
    testMappedReduced(intList, sumOfSquares, NumberSquare(), lambdaSumReduce);
    CHECK_FAIL("functor-lambda");

    // FUNCTION-other
    testMappedReduced(intList, sumOfSquares, numberSquare, NumberSumReduce());
    CHECK_FAIL("function-functor");
    testMappedReduced(intList, sumOfSquares, numberSquare, numberSumReduce);
    CHECK_FAIL("function-function");
    testMappedReduced(intList, sumOfSquares, numberSquare, lambdaSumReduce);
    CHECK_FAIL("function-lambda");

    // MEMBER-other
    testMappedReduced(numberList, sumOfSquares, &Number::squared, NumberSumReduce());
    CHECK_FAIL("member-functor");
    testMappedReduced(numberList, sumOfSquares, &Number::squared, numberSumReduce);
    CHECK_FAIL("member-function");
    testMappedReduced(numberList, sumOfSquares, &Number::squared, lambdaSumReduce);
    CHECK_FAIL("member-lambda");

    // LAMBDA-other
    testMappedReduced(intList, sumOfSquares, lambdaSquare, NumberSumReduce());
    CHECK_FAIL("lambda-functor");
    testMappedReduced(intList, sumOfSquares, lambdaSquare, numberSumReduce);
    CHECK_FAIL("lambda-function");
    testMappedReduced(intList, sumOfSquares, lambdaSquare, lambdaSumReduce);
    CHECK_FAIL("lambda-lambda");
}

template <typename SourceObject, typename ResultObject, typename InitialObject, typename MapObject, typename ReduceObject>
void testMappedReducedInitialValue(const QList<SourceObject> &sourceObjectList,
                                   const ResultObject &expectedResult,
                                   MapObject mapObject,
                                   ReduceObject reduceObject,
                                   InitialObject &&initialObject)
{
    // Result type is passed explicitly
    {
        const ResultObject result1 =
                BobUIConcurrent::mappedReduced<ResultObject>(sourceObjectList, mapObject, reduceObject,
                                                          initialObject).result();
        QCOMPARE(result1, expectedResult);

        const ResultObject result2 = BobUIConcurrent::mappedReduced<ResultObject>(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    mapObject, reduceObject, initialObject).result();
        QCOMPARE(result2, expectedResult);

        const ResultObject result3 = BobUIConcurrent::blockingMappedReduced<ResultObject>(
                    sourceObjectList, mapObject, reduceObject, initialObject);
        QCOMPARE(result3, expectedResult);

        const ResultObject result4 = BobUIConcurrent::blockingMappedReduced<ResultObject>(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    mapObject, reduceObject, initialObject);
        QCOMPARE(result4, expectedResult);
    }

    // Result type is deduced
    {
        const ResultObject result1 =
                BobUIConcurrent::mappedReduced(sourceObjectList, mapObject, reduceObject,
                                            initialObject).result();
        QCOMPARE(result1, expectedResult);

        const ResultObject result2 = BobUIConcurrent::mappedReduced(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    mapObject, reduceObject, initialObject).result();
        QCOMPARE(result2, expectedResult);

        const ResultObject result3 = BobUIConcurrent::blockingMappedReduced(
                    sourceObjectList, mapObject, reduceObject, initialObject);
        QCOMPARE(result3, expectedResult);

        const ResultObject result4 = BobUIConcurrent::blockingMappedReduced(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    mapObject, reduceObject, initialObject);
        QCOMPARE(result4, expectedResult);
    }
}

template <typename SourceObject, typename ResultObject, typename InitialObject, typename MapObject, typename ReduceObject>
void testMappedReducedInitialValue(const QList<SourceObject> &sourceObjectList,
                                   const ResultObject &expectedResult,
                                   MapObject mapObject,
                                   ReduceObject reduceObject,
                                   InitialObject &&initialObject,
                                   BobUIConcurrent::ReduceOptions options)
{
    const ResultObject result1 = BobUIConcurrent::mappedReduced(
                sourceObjectList, mapObject, reduceObject, initialObject, options).result();
    QCOMPARE(result1, expectedResult);

    const ResultObject result2 =
            BobUIConcurrent::mappedReduced(sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                                        mapObject, reduceObject, initialObject, options).result();
    QCOMPARE(result2, expectedResult);

    const ResultObject result3 = BobUIConcurrent::blockingMappedReduced(
                sourceObjectList, mapObject, reduceObject, initialObject, options);
    QCOMPARE(result3, expectedResult);

    const ResultObject result4 = BobUIConcurrent::blockingMappedReduced(
                sourceObjectList.constBegin(), sourceObjectList.constEnd(), mapObject, reduceObject, initialObject, options);
    QCOMPARE(result4, expectedResult);
}

void tst_BobUIConcurrentMap::mappedReducedInitialValue()
{
    // This is a copy of tst_BobUIConcurrentMap::mappedReduced with the initial value parameter added

    const QList<int> intList {1, 2, 3};
    const QList<int> intListInitial {10};
    const QList<int> intListAppended {10, 1, 2, 3};
    const QList<int> intListSquaresAppended {10, 1, 4, 9};
    const QList<Number> numberList {1, 2, 3};
    const int sum = 16;
    const int sumOfSquares = 24;
    const int intInitial = 10;

    void (QList<int>::*push_back)(QList<int>::parameter_type) = &QList<int>::push_back;

    auto lambdaSquare = [](int x) {
        return x * x;
    };
    auto lambdaSumReduce = [](int &sum, int x) {
        sum += x;
    };

    // FUNCTOR-other
    testMappedReducedInitialValue(intList, sumOfSquares, IntSquare(), IntSumReduce(), intInitial);
    CHECK_FAIL("functor-functor");
    testMappedReducedInitialValue(intList, sumOfSquares, IntSquare(), intSumReduce, intInitial);
    CHECK_FAIL("functor-function");
    testMappedReducedInitialValue(intList, intListSquaresAppended, IntSquare(), push_back, intListInitial, OrderedReduce);
    CHECK_FAIL("functor-member");
    testMappedReducedInitialValue(intList, sumOfSquares, IntSquare(), lambdaSumReduce, intInitial);
    CHECK_FAIL("functor-lambda");

    // FUNCTION-other
    testMappedReducedInitialValue(intList, sumOfSquares, intSquare, IntSumReduce(), intInitial);
    CHECK_FAIL("function-functor");
    testMappedReducedInitialValue(intList, sumOfSquares, intSquare, intSumReduce, intInitial);
    CHECK_FAIL("function-function");
    testMappedReducedInitialValue(intList, intListSquaresAppended, intSquare, push_back, intListInitial, OrderedReduce);
    CHECK_FAIL("function-member");
    testMappedReducedInitialValue(intList, sumOfSquares, intSquare, lambdaSumReduce, intInitial);
    CHECK_FAIL("function-lambda");

    // MEMBER-other
    testMappedReducedInitialValue(numberList, sum, &Number::toInt, IntSumReduce(), intInitial);
    CHECK_FAIL("member-functor");
    testMappedReducedInitialValue(numberList, sum, &Number::toInt, intSumReduce, intInitial);
    CHECK_FAIL("member-function");
    testMappedReducedInitialValue(numberList, intListAppended, &Number::toInt, push_back, intListInitial, OrderedReduce);
    CHECK_FAIL("member-member");
    testMappedReducedInitialValue(numberList, sum, &Number::toInt, lambdaSumReduce, intInitial);
    CHECK_FAIL("member-lambda");

    // LAMBDA-other
    testMappedReducedInitialValue(intList, sumOfSquares, lambdaSquare, IntSumReduce(), intInitial);
    CHECK_FAIL("lambda-functor");
    testMappedReducedInitialValue(intList, sumOfSquares, lambdaSquare, intSumReduce, intInitial);
    CHECK_FAIL("lambda-function");
    testMappedReducedInitialValue(intList, intListSquaresAppended, lambdaSquare, push_back, intListInitial, OrderedReduce);
    CHECK_FAIL("lambda-member");
    testMappedReducedInitialValue(intList, sumOfSquares, lambdaSquare, lambdaSumReduce, intInitial);
    CHECK_FAIL("lambda-lambda");

    {
        // non-template sequences
        NonTemplateSequence list { 1, 2, 3 };

        auto future = BobUIConcurrent::mappedReduced(list, multiplyBy2, intSumReduce, intInitial);
        QCOMPARE(future.result(), intInitial + 12);

        auto result =
                BobUIConcurrent::blockingMappedReduced(list, multiplyBy2, intSumReduce, intInitial);
        QCOMPARE(result, intInitial + 12);
    }

    {
        // rvalue sequences
        auto future = BobUIConcurrent::mappedReduced(std::vector { 1, 2, 3 }, intSquare, intSumReduce,
                                                  intInitial);
        QCOMPARE(future.result(), sumOfSquares);

        auto result = BobUIConcurrent::blockingMappedReduced(std::vector { 1, 2, 3 }, intSquare,
                                                          intSumReduce, intInitial);
        QCOMPARE(result, sumOfSquares);
    }

    {
        // move only sequences
        auto future = BobUIConcurrent::mappedReduced(MoveOnlyVector<int>({ 1, 2, 3 }), intSquare,
                                                  intSumReduce, intInitial);
        QCOMPARE(future.result(), sumOfSquares);

        auto result = BobUIConcurrent::blockingMappedReduced(MoveOnlyVector<int>({ 1, 2, 3 }),
                                                          intSquare, intSumReduce, intInitial);
        QCOMPARE(result, sumOfSquares);
    }
}

template <typename SourceObject, typename ResultObject, typename InitialObject, typename MapObject, typename ReduceObject>
void testMappedReducedInitialValueThreadPool(BOBUIhreadPool *pool,
                                             const QList<SourceObject> &sourceObjectList,
                                             const ResultObject &expectedResult,
                                             MapObject mapObject,
                                             ReduceObject reduceObject,
                                             InitialObject &&initialObject)
{
    // Result type is passed explicitly
    {
        const ResultObject result1 = BobUIConcurrent::mappedReduced<ResultObject>(
                    pool, sourceObjectList, mapObject, reduceObject, initialObject).result();
        QCOMPARE(result1, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result2 =
                BobUIConcurrent::mappedReduced<ResultObject>(pool, sourceObjectList.constBegin(),
                                                          sourceObjectList.constEnd(), mapObject,
                                                          reduceObject, initialObject).result();
        QCOMPARE(result2, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result3 = BobUIConcurrent::blockingMappedReduced<ResultObject>(
                    pool, sourceObjectList, mapObject, reduceObject, initialObject);
        QCOMPARE(result3, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result4 = BobUIConcurrent::blockingMappedReduced<ResultObject>(
                    pool, sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    mapObject, reduceObject, initialObject);
        QCOMPARE(result4, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working
    }

    // Result type is deduced
    {
        const ResultObject result1 = BobUIConcurrent::mappedReduced(
                    pool, sourceObjectList, mapObject, reduceObject, initialObject).result();
        QCOMPARE(result1, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result2 =
                BobUIConcurrent::mappedReduced(pool, sourceObjectList.constBegin(),
                                            sourceObjectList.constEnd(), mapObject,
                                            reduceObject, initialObject).result();
        QCOMPARE(result2, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result3 = BobUIConcurrent::blockingMappedReduced(
                    pool, sourceObjectList, mapObject, reduceObject, initialObject);
        QCOMPARE(result3, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result4 = BobUIConcurrent::blockingMappedReduced(
                    pool, sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    mapObject, reduceObject, initialObject);
        QCOMPARE(result4, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working
    }
}

void tst_BobUIConcurrentMap::mappedReducedInitialValueThreadPool()
{
    // This is a copy of tst_BobUIConcurrentMap::mappedReduced with the initial value parameter added

    const QList<int> intList {1, 2, 3};
    const int sumOfCubes = 46;
    const int intInitial = 10;

    auto lambdaCube = [](int x) {
        return x * x * x;
    };
    auto lambdaSumReduce = [](int &sum, int x) {
        sum += x;
    };

    BOBUIhreadPool pool;
    pool.setMaxThreadCount(1);
    QCOMPARE(semaphore.available(), 1);
    workingThreads.clear();

    // FUNCTOR-other
    testMappedReducedInitialValueThreadPool(&pool, intList, sumOfCubes, IntCube(),
                                            IntSumReduce(), intInitial);
    CHECK_FAIL("functor-functor");
    testMappedReducedInitialValueThreadPool(&pool, intList, sumOfCubes, IntCube(),
                                            intSumReduce, intInitial);
    CHECK_FAIL("functor-function");
    testMappedReducedInitialValueThreadPool(&pool, intList, sumOfCubes, IntCube(),
                                            lambdaSumReduce, intInitial);
    CHECK_FAIL("functor-lambda");

    // FUNCTION-other
    testMappedReducedInitialValueThreadPool(&pool, intList, sumOfCubes, intCube,
                                            IntSumReduce(), intInitial);
    CHECK_FAIL("function-functor");
    testMappedReducedInitialValueThreadPool(&pool, intList, sumOfCubes, intCube,
                                            intSumReduce, intInitial);
    CHECK_FAIL("function-function");
    testMappedReducedInitialValueThreadPool(&pool, intList, sumOfCubes, intCube,
                                            lambdaSumReduce, intInitial);
    CHECK_FAIL("function-lambda");

    // LAMBDA-other
    testMappedReducedInitialValueThreadPool(&pool, intList, sumOfCubes, lambdaCube,
                                            IntSumReduce(), intInitial);
    CHECK_FAIL("lambda-functor");
    testMappedReducedInitialValueThreadPool(&pool, intList, sumOfCubes, lambdaCube,
                                            intSumReduce, intInitial);
    CHECK_FAIL("lambda-function");
    testMappedReducedInitialValueThreadPool(&pool, intList, sumOfCubes, lambdaCube,
                                            lambdaSumReduce, intInitial);
    CHECK_FAIL("lambda-lambda");

    {
        // non-template sequences
        NonTemplateSequence list { 1, 2, 3 };

        auto future =
                BobUIConcurrent::mappedReduced(&pool, list, multiplyBy2, intSumReduce, intInitial);
        QCOMPARE(future.result(), intInitial + 12);

        auto result = BobUIConcurrent::blockingMappedReduced(&pool, list, multiplyBy2, intSumReduce,
                                                          intInitial);
        QCOMPARE(result, intInitial + 12);
    }

    {
        // rvalue sequences
        auto future = BobUIConcurrent::mappedReduced(&pool, std::vector { 1, 2, 3 }, intCube,
                                                  intSumReduce, intInitial);
        QCOMPARE(future.result(), sumOfCubes);

        auto result = BobUIConcurrent::blockingMappedReduced(&pool, std::vector { 1, 2, 3 }, intCube,
                                                          intSumReduce, intInitial);
        QCOMPARE(result, sumOfCubes);
    }

    {
        // move only sequences
        auto future = BobUIConcurrent::mappedReduced(&pool, MoveOnlyVector<int>({ 1, 2, 3 }), intCube,
                                                  intSumReduce, intInitial);
        QCOMPARE(future.result(), sumOfCubes);

        auto result = BobUIConcurrent::blockingMappedReduced(&pool, MoveOnlyVector<int>({ 1, 2, 3 }),
                                                          intCube, intSumReduce, intInitial);
        QCOMPARE(result, sumOfCubes);
    }
}

void tst_BobUIConcurrentMap::mappedReducedInitialValueWithMoveOnlyCallable()
{
    const QList<int> intList { 1, 2, 3 };
    const auto initialValue = 10;
    const auto sum = 22;
    {
        const auto result =
                BobUIConcurrent::mappedReduced(intList, MultiplyBy2(),
                                            IntSumReduceMoveOnly(), initialValue).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result =
                BobUIConcurrent::mappedReduced(intList.begin(), intList.end(), MultiplyBy2(),
                                            IntSumReduceMoveOnly(), initialValue).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingMappedReduced(
                    intList, MultiplyBy2(), IntSumReduceMoveOnly(), initialValue);
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingMappedReduced(
                    intList.begin(), intList.end(), MultiplyBy2(), IntSumReduceMoveOnly(),
                    initialValue);
        QCOMPARE(result, sum);
    }

    BOBUIhreadPool pool;
    {
        const auto result =
                BobUIConcurrent::mappedReduced(&pool, intList, MultiplyBy2(),
                                            IntSumReduceMoveOnly(), initialValue).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::mappedReduced(&pool, intList.begin(), intList.end(),
                                                        MultiplyBy2(), IntSumReduceMoveOnly(),
                                                        initialValue).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingMappedReduced(
                    &pool, intList, MultiplyBy2(), IntSumReduceMoveOnly(), initialValue);
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingMappedReduced(
                    &pool, intList.begin(), intList.end(), MultiplyBy2(), IntSumReduceMoveOnly(),
                    initialValue);
        QCOMPARE(result, sum);
    }
}

void tst_BobUIConcurrentMap::mappedReducedDifferentTypeInitialValue()
{
    // This is a copy of tst_BobUIConcurrentMap::mappedReducedDifferentType
    // with the initial value parameter added

    const QList<Number> numberList {1, 2, 3};
    const int sumOfSquares = 24;
    const int intInitial = 10;

    auto lambdaSquare = [](Number x) {
        return Number(x.toInt() * x.toInt());
    };
    auto lambdaSumReduce = [](int &sum, Number x) {
        sum += x.toInt();
    };

    // FUNCTOR-other
    testMappedReducedInitialValue(numberList, sumOfSquares, NumberSquare(), NumberSumReduce(), intInitial);
    CHECK_FAIL("functor-functor");
    testMappedReducedInitialValue(numberList, sumOfSquares, NumberSquare(), numberSumReduce, intInitial);
    CHECK_FAIL("functor-function");
    testMappedReducedInitialValue(numberList, sumOfSquares, NumberSquare(), lambdaSumReduce, intInitial);
    CHECK_FAIL("functor-lambda");

    // FUNCTION-other
    testMappedReducedInitialValue(numberList, sumOfSquares, numberSquare, NumberSumReduce(), intInitial);
    CHECK_FAIL("function-functor");
    testMappedReducedInitialValue(numberList, sumOfSquares, numberSquare, numberSumReduce, intInitial);
    CHECK_FAIL("function-function");
    testMappedReducedInitialValue(numberList, sumOfSquares, numberSquare, lambdaSumReduce, intInitial);
    CHECK_FAIL("function-lambda");

    // MEMBER-other
    testMappedReducedInitialValue(numberList, sumOfSquares, &Number::squared, NumberSumReduce(), intInitial);
    CHECK_FAIL("member-functor");
    testMappedReducedInitialValue(numberList, sumOfSquares, &Number::squared, numberSumReduce, intInitial);
    CHECK_FAIL("member-function");
    testMappedReducedInitialValue(numberList, sumOfSquares, &Number::squared, lambdaSumReduce, intInitial);
    CHECK_FAIL("member-lambda");

    // LAMBDA-other
    testMappedReducedInitialValue(numberList, sumOfSquares, lambdaSquare, NumberSumReduce(), intInitial);
    CHECK_FAIL("lambda-functor");
    testMappedReducedInitialValue(numberList, sumOfSquares, lambdaSquare, numberSumReduce, intInitial);
    CHECK_FAIL("lambda-function");
    testMappedReducedInitialValue(numberList, sumOfSquares, lambdaSquare, lambdaSumReduce, intInitial);
    CHECK_FAIL("lambda-lambda");
}

void tst_BobUIConcurrentMap::mappedReduceOptionConvertableToResultType()
{
    const QList<int> intList { 1, 2, 3 };
    const int sum = 12;
    BOBUIhreadPool p;
    ReduceOption ro = OrderedReduce;

    // With container
    QCOMPARE(BobUIConcurrent::mappedReduced(intList, multiplyBy2, intSumReduce, ro).result(), sum);
    QCOMPARE(BobUIConcurrent::blockingMappedReduced(intList, multiplyBy2, intSumReduce, ro), sum);

    // With iterators
    QCOMPARE(BobUIConcurrent::mappedReduced(intList.begin(), intList.end(), multiplyBy2, intSumReduce,
                                         ro).result(), sum);
    QCOMPARE(BobUIConcurrent::blockingMappedReduced(intList.begin(), intList.end(), multiplyBy2,
                                                 intSumReduce, ro), sum);

    // With custom BOBUIhreadPool;
    QCOMPARE(BobUIConcurrent::mappedReduced(&p, intList, multiplyBy2, intSumReduce, ro).result(), sum);
    QCOMPARE(BobUIConcurrent::blockingMappedReduced(&p, intList, multiplyBy2, intSumReduce, ro), sum);
    QCOMPARE(BobUIConcurrent::mappedReduced(&p, intList.begin(), intList.end(), multiplyBy2,
                                         intSumReduce, ro).result(), sum);
    QCOMPARE(BobUIConcurrent::blockingMappedReduced(&p, intList.begin(), intList.end(), multiplyBy2,
                                                 intSumReduce, ro), sum);

    // The same as above, but specify the result type explicitly (this invokes different overloads)
    QCOMPARE(BobUIConcurrent::mappedReduced<int>(intList, multiplyBy2, intSumReduce, ro).result(),
             sum);
    QCOMPARE(BobUIConcurrent::blockingMappedReduced<int>(intList, multiplyBy2, intSumReduce, ro), sum);

    QCOMPARE(BobUIConcurrent::mappedReduced<int>(intList.begin(), intList.end(), multiplyBy2,
                                              intSumReduce, ro).result(), sum);
    QCOMPARE(BobUIConcurrent::blockingMappedReduced<int>(intList.begin(), intList.end(), multiplyBy2,
                                                      intSumReduce, ro), sum);

    QCOMPARE(BobUIConcurrent::mappedReduced<int>(&p, intList, multiplyBy2, intSumReduce, ro).result(),
             sum);
    QCOMPARE(BobUIConcurrent::blockingMappedReduced<int>(&p, intList, multiplyBy2, intSumReduce, ro),
             sum);
    QCOMPARE(BobUIConcurrent::mappedReduced<int>(&p, intList.begin(), intList.end(), multiplyBy2,
                                              intSumReduce, ro).result(), sum);
    QCOMPARE(BobUIConcurrent::blockingMappedReduced<int>(&p, intList.begin(), intList.end(),
                                                      multiplyBy2, intSumReduce, ro), sum);
}

int sleeper(int val)
{
    BOBUIest::qSleep(100);
    return val;
}

void tst_BobUIConcurrentMap::assignResult()
{
    const QList<int> startList = QList<int>() << 0 << 1 << 2;
    QList<int> list = BobUIConcurrent::blockingMapped(startList, sleeper);
    QCOMPARE(list.at(0), 0);
    QCOMPARE(list.at(1), 1);
}

int fnConst(const int &i)
{
    return i;
}

int fn(int &i)
{
    return i;
}

int fnConstNoExcept(const int &i) noexcept
{
    return i;
}

int fnNoExcept(int &i) noexcept
{
    return i;
}

QString changeTypeConst(const int &)
{
    return QString();
}

QString changeType(int &)
{
    return QString();
}

QString changeTypeConstNoExcept(const int &) noexcept
{
    return QString();
}

QString changeTypeNoExcept(int &) noexcept
{
    return QString();
}

int changeTypeQStringListConst(const QStringList &)
{
    return 0;
}

int changeTypeQStringList(QStringList &)
{
    return 0;
}

int changeTypeQStringListConstNoExcept(const QStringList &) noexcept
{
    return 0;
}

int changeTypeQStringListNoExcept(QStringList &) noexcept
{
    return 0;
}

class MemFnTester
{
public:
    MemFnTester() {}

    MemFnTester fn()
    {
        return MemFnTester();
    }

    MemFnTester fnConst() const
    {
        return MemFnTester();
    }

    QString changeType()
    {
        return QString();
    }

    QString changeTypeConst() const
    {
        return QString();
    }

    MemFnTester fnNoExcept() noexcept
    {
        return MemFnTester();
    }

    MemFnTester fnConstNoExcept() const noexcept
    {
        return MemFnTester();
    }

    QString changeTypeNoExcept() noexcept
    {
        return QString();
    }

    QString changeTypeConstNoExcept() const noexcept
    {
        return QString();
    }
};

Q_DECLARE_METATYPE(QList<MemFnTester>);

void tst_BobUIConcurrentMap::functionOverloads()
{
    QList<int> intList;
    const QList<int> constIntList;
    QList<MemFnTester> classList;
    const QList<MemFnTester> constMemFnTesterList;

    BobUIConcurrent::mapped(intList, fnConst);
    BobUIConcurrent::mapped(constIntList, fnConst);
    BobUIConcurrent::mapped(classList, &MemFnTester::fnConst);
    BobUIConcurrent::mapped(constMemFnTesterList, &MemFnTester::fnConst);

    BobUIConcurrent::blockingMapped<QList<int>>(intList, fnConst);
    BobUIConcurrent::blockingMapped<QList<int>>(constIntList, fnConst);
    BobUIConcurrent::blockingMapped<QList<MemFnTester>>(classList, &MemFnTester::fnConst);
    BobUIConcurrent::blockingMapped<QList<MemFnTester>>(constMemFnTesterList, &MemFnTester::fnConst);

    BobUIConcurrent::blockingMapped<QList<QString> >(intList, changeTypeConst);
    BobUIConcurrent::blockingMapped<QList<QString> >(constIntList, changeTypeConst);
    BobUIConcurrent::blockingMapped<QList<QString> >(classList, &MemFnTester::changeTypeConst);
    BobUIConcurrent::blockingMapped<QList<QString> >(constMemFnTesterList, &MemFnTester::changeTypeConst);
}

void tst_BobUIConcurrentMap::noExceptFunctionOverloads()
{
    QList<int> intList;
    const QList<int> constIntList;
    QList<MemFnTester> classList;
    const QList<MemFnTester> constMemFnTesterList;

    BobUIConcurrent::mapped(intList, fnConstNoExcept);
    BobUIConcurrent::mapped(constIntList, fnConstNoExcept);
    BobUIConcurrent::mapped(classList, &MemFnTester::fnConstNoExcept);
    BobUIConcurrent::mapped(constMemFnTesterList, &MemFnTester::fnConstNoExcept);

    BobUIConcurrent::blockingMapped<QList<int>>(intList, fnConstNoExcept);
    BobUIConcurrent::blockingMapped<QList<int>>(constIntList, fnConstNoExcept);
    BobUIConcurrent::blockingMapped<QList<MemFnTester>>(classList, &MemFnTester::fnConstNoExcept);
    BobUIConcurrent::blockingMapped<QList<MemFnTester>>(constMemFnTesterList,
                                                     &MemFnTester::fnConstNoExcept);

    BobUIConcurrent::blockingMapped<QList<QString> >(intList, changeTypeConstNoExcept);
    BobUIConcurrent::blockingMapped<QList<QString> >(constIntList, changeTypeConstNoExcept);
    BobUIConcurrent::blockingMapped<QList<QString> >(classList, &MemFnTester::changeTypeConstNoExcept);
    BobUIConcurrent::blockingMapped<QList<QString> >(constMemFnTesterList, &MemFnTester::changeTypeConstNoExcept);
}

QAtomicInt currentInstanceCount;
QAtomicInt peakInstanceCount;
class InstanceCounter
{
public:
    inline InstanceCounter()
    { currentInstanceCount.fetchAndAddRelaxed(1); updatePeak(); }
    inline ~InstanceCounter()
    { currentInstanceCount.fetchAndAddRelaxed(-1);}
    inline InstanceCounter(const InstanceCounter &)
    { currentInstanceCount.fetchAndAddRelaxed(1); updatePeak(); }
    constexpr InstanceCounter &operator=(const InstanceCounter &) noexcept
    { return *this; }

    void updatePeak()
    {
        forever {
            const int localPeak = peakInstanceCount.loadRelaxed();
            const int localCurrent = currentInstanceCount.loadRelaxed();
            if (localCurrent <= localPeak)
                break;
            if (peakInstanceCount.testAndSetOrdered(localPeak, localCurrent))
                break;
        }
    }
};

InstanceCounter slowMap(const InstanceCounter &in)
{
    BOBUIest::qSleep(2);
    return in;
}

InstanceCounter fastMap(const InstanceCounter &in)
{
    BOBUIest::qSleep(QRandomGenerator::global()->bounded(2) + 1);
    return in;
}

void slowReduce(int &result, const InstanceCounter&)
{
    BOBUIest::qSleep(QRandomGenerator::global()->bounded(4) + 1);
    ++result;
}

void fastReduce(int &result, const InstanceCounter&)
{
    ++result;
}

void tst_BobUIConcurrentMap::throttling()
{
    const int itemcount = 100;
    const int allowedTemporaries = BOBUIhread::idealThreadCount() * 40;

    {
        currentInstanceCount.storeRelaxed(0);
        peakInstanceCount.storeRelaxed(0);

        QList<InstanceCounter> instances;
        for (int i = 0; i < itemcount; ++i)
            instances.append(InstanceCounter());

        QCOMPARE(currentInstanceCount.loadRelaxed(), itemcount);

        int results = BobUIConcurrent::blockingMappedReduced(instances, slowMap, fastReduce);
        QCOMPARE(results, itemcount);
        QCOMPARE(currentInstanceCount.loadRelaxed(), itemcount);
        QVERIFY(peakInstanceCount.loadRelaxed() < itemcount + allowedTemporaries);
    }

    {
        QCOMPARE(currentInstanceCount.loadRelaxed(), 0);
        peakInstanceCount.storeRelaxed(0);

        QList<InstanceCounter> instances;
        for (int i = 0; i < itemcount; ++i)
            instances.append(InstanceCounter());

        QCOMPARE(currentInstanceCount.loadRelaxed(), itemcount);
        int results = BobUIConcurrent::blockingMappedReduced(instances, fastMap, slowReduce);

        QCOMPARE(results, itemcount);
        QCOMPARE(currentInstanceCount.loadRelaxed(), itemcount);
        QVERIFY(peakInstanceCount.loadRelaxed() < itemcount + allowedTemporaries);
    }
}

#ifndef BOBUI_NO_EXCEPTIONS
void throwMapper(int &e)
{
    Q_UNUSED(e);
    throw QException();
}

void tst_BobUIConcurrentMap::exceptions()
{
    bool caught = false;
    try  {
        QList<int> list = QList<int>() << 1 << 2 << 3;
        BobUIConcurrent::map(list, throwMapper).waitForFinished();
    } catch (const QException &) {
        caught = true;
    }
    if (!caught)
        QFAIL("did not get exception");
}
#endif

int mapper(const int &i)
{
    BOBUIest::qWait(1);
    return i;
}

void tst_BobUIConcurrentMap::incrementalResults()
{
    const int count = 200;
    QList<int> ints;
    for (int i=0; i < count; ++i)
        ints << i;

    QFuture<int> future = BobUIConcurrent::mapped(ints, mapper);

    QList<int> results;

    while (future.isFinished() == false) {
        for (int i = 0; i < future.resultCount(); ++i) {
            results += future.resultAt(i);
        }

        BOBUIest::qWait(1);
    }

    QCOMPARE(future.isFinished(), true);
    QCOMPARE(future.resultCount(), count);
    QCOMPARE(future.results().size(), count);
}

/*
    Test that mapped does not cause deep copies when holding
    references to BobUI containers.
*/
void tst_BobUIConcurrentMap::noDetach()
{
    {
        QList<int> l = QList<int>() << 1;
        QVERIFY(l.isDetached());

        QList<int> ll = l;
        QVERIFY(!l.isDetached());

        BobUIConcurrent::mapped(l, mapper).waitForFinished();

        QVERIFY(!l.isDetached());
        QVERIFY(!ll.isDetached());

        BobUIConcurrent::mappedReduced(l, mapper, intSumReduce).waitForFinished();

        QVERIFY(!l.isDetached());
        QVERIFY(!ll.isDetached());

        BobUIConcurrent::map(l, multiplyBy2Immutable).waitForFinished();
        QVERIFY(l.isDetached());
        QVERIFY(ll.isDetached());
    }
    {
        const QList<int> l = QList<int>() << 1;
        QVERIFY(l.isDetached());

        const QList<int> ll = l;
        QVERIFY(!l.isDetached());

        BobUIConcurrent::mapped(l, mapper).waitForFinished();

        QVERIFY(!l.isDetached());
        QVERIFY(!ll.isDetached());

        BobUIConcurrent::mappedReduced(l, mapper, intSumReduce).waitForFinished();

        QVERIFY(!l.isDetached());
        QVERIFY(!ll.isDetached());
    }

}

void tst_BobUIConcurrentMap::stlContainers()
{
    std::vector<int> vector;
    vector.push_back(1);
    vector.push_back(2);

    std::vector<int> vector2 = BobUIConcurrent::blockingMapped(vector, mapper);
    QCOMPARE(vector2.size(), 2u);

    std::list<int> list;
    list.push_back(1);
    list.push_back(2);

    std::list<int> list2 = BobUIConcurrent::blockingMapped(list, mapper);
    QCOMPARE(list2.size(), 2u);

    BobUIConcurrent::mapped(list, mapper).waitForFinished();

    BobUIConcurrent::blockingMap(list, multiplyBy2Immutable);
}

void tst_BobUIConcurrentMap::stlContainersLambda()
{
    std::vector<int> vector;
    vector.push_back(1);
    vector.push_back(2);

    std::vector<int> vector2 =
            BobUIConcurrent::blockingMapped(vector, [](const int &i) { return mapper(i); });
    QCOMPARE(vector2.size(), 2u);

    std::list<int> list;
    list.push_back(1);
    list.push_back(2);

    std::list<int> list2 =
            BobUIConcurrent::blockingMapped(list, [](const int &i) { return mapper(i); });
    QCOMPARE(list2.size(), 2u);

    BobUIConcurrent::mapped(list, [](const int &i) { return mapper(i); }).waitForFinished();

    BobUIConcurrent::blockingMap(list, []([[maybe_unused]] int x) { x *= 2; });
}

InstanceCounter ic_fn(const InstanceCounter & ic)
{
    return InstanceCounter(ic);
}

// Verify that held results are deleted when a future is
// assigned over with operator ==
void tst_BobUIConcurrentMap::qFutureAssignmentLeak()
{
    currentInstanceCount.storeRelaxed(0);
    peakInstanceCount.storeRelaxed(0);
    QFuture<InstanceCounter> future;
    {
        QList<InstanceCounter> list;
        for (int i=0;i<1000;++i)
            list += InstanceCounter();
        future = BobUIConcurrent::mapped(list, ic_fn);
        future.waitForFinished();

        future = BobUIConcurrent::mapped(list, ic_fn);
        future.waitForFinished();

        future = BobUIConcurrent::mapped(list, ic_fn);
        future.waitForFinished();
    }

    // Use BOBUIRY_COMPARE because BobUIConcurrent::ThreadEngine::asynchronousFinish()
    // deletes its internals after signaling finished, so it might still be holding
    // on to copies of InstanceCounter for a short while.
    BOBUIRY_COMPARE(currentInstanceCount.loadRelaxed(), 1000);
    future = QFuture<InstanceCounter>();
    BOBUIRY_COMPARE(currentInstanceCount.loadRelaxed(), 0);
}

inline void increment(int &num)
{
    ++num;
}

inline int echo(const int &num)
{
    return num;
}

void add(int &result, const int &sum)
{
    result += sum;
}

void tst_BobUIConcurrentMap::stressTest()
{
    const int listSize = 1000;
    const int sum = (listSize - 1) * (listSize / 2);
    QList<int> list;


    for (int i = 0; i < listSize; ++i) {
        list.append(i);
    }

    for (int i =0 ; i < 100; ++i) {
        QList<int> result = BobUIConcurrent::blockingMapped(list, echo);
        for (int j = 0; j < listSize; ++j)
            QCOMPARE(result.at(j), j);
    }

    for (int i = 0 ; i < 100; ++i) {
        int result = BobUIConcurrent::blockingMappedReduced(list, echo, add);
        QCOMPARE(result, sum);
    }

    for (int i = 0 ; i < 100; ++i) {
        BobUIConcurrent::map(list, increment).waitForFinished();
        for (int j = 0; j < listSize; ++j)
            QCOMPARE(list.at(j), i + j + 1);
    }
}

struct LockedCounter
{
    LockedCounter(QMutex *mutex, QAtomicInt *ai)
        : mtx(mutex),
          ref(ai) {}

    int operator()(int x)
    {
        QMutexLocker locker(mtx);
        ref->ref();
        return ++x;
    }

    QMutex *mtx;
    QAtomicInt *ref;
};

// The Thread engine holds the last reference
// to the QFuture, so this should not leak
// or fail.
void tst_BobUIConcurrentMap::persistentResultTest()
{
    QFuture<void> voidFuture;
    QMutex mtx;
    QAtomicInt ref;
    LockedCounter lc(&mtx, &ref);
    QList<int> list;
    {
        list << 1 << 2 << 3;
        mtx.lock();
        QFuture<int> future = BobUIConcurrent::mapped(list
                                                   ,lc);
        voidFuture = future;
    }
    QCOMPARE(ref.loadAcquire(), 0);
    mtx.unlock(); // Unblock
    voidFuture.waitForFinished();
    QCOMPARE(ref.loadAcquire(), 3);
}

BOBUIEST_MAIN(tst_BobUIConcurrentMap)
#include "tst_bobuiconcurrentmap.moc"
