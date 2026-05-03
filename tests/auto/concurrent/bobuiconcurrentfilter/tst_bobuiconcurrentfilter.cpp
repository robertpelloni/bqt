// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <bobuiconcurrentfilter.h>
#include <QCoreApplication>
#include <QList>
#include <BOBUIest>
#include <QSet>

#include "../testhelper_functions.h"

class tst_BobUIConcurrentFilter : public QObject
{
    Q_OBJECT

private slots:
    void filter();
    void filterThreadPool();
    void filterWithMoveOnlyCallable();
    void filtered();
    void filteredThreadPool();
    void filteredWithMoveOnlyCallable();
    void filteredWithGenericCallable();
    void filteredReduced();
    void filteredReducedThreadPool();
    void filteredReducedWithMoveOnlyCallables();
    void filteredReducedWithGenericCallable();
    void filteredReducedDifferentType();
    void filteredReducedInitialValue();
    void filteredReducedInitialValueThreadPool();
    void filteredReducedInitialValueWithMoveOnlyCallables();
    void filteredReducedDifferentTypeInitialValue();
    void filteredReduceOptionConvertableToResultType();
    void resultAt();
    void incrementalResults();
    void noDetach();
    void stlContainers();
    void stlContainersLambda();
};

using namespace BobUIConcurrent;

#define CHECK_FAIL(message) \
do {\
    if (BOBUIest::currentTestFailed())\
        QFAIL("failed one line above on " message);\
} while (false)

template <typename SourceObject,
          typename ResultObject,
          typename FilterObject>
void testFilter(const QList<SourceObject> &sourceObjectList,
                const QList<ResultObject> &expectedResult,
                FilterObject filterObject)
{
    QList<SourceObject> copy1 = sourceObjectList;
    QList<SourceObject> copy2 = sourceObjectList;

    BobUIConcurrent::filter(copy1, filterObject).waitForFinished();
    QCOMPARE(copy1, expectedResult);

    BobUIConcurrent::blockingFilter(copy2, filterObject);
    QCOMPARE(copy2, expectedResult);
}

void tst_BobUIConcurrentFilter::filter()
{
    const QList<int> intList {1, 2, 3, 4};
    const QList<int> intListEven {2, 4};
    const QList<Number> numberList {1, 2, 3, 4};
    const QList<Number> numberListEven {2, 4};

    auto lambdaIsEven = [](const int &x) {
        return (x & 1) == 0;
    };

    testFilter(intList, intListEven, KeepEvenIntegers());
    CHECK_FAIL("functor");
    testFilter(intList, intListEven, keepEvenIntegers);
    CHECK_FAIL("function");
    testFilter(numberList, numberListEven, &Number::isEven);
    CHECK_FAIL("member");
    testFilter(intList, intListEven, lambdaIsEven);
    CHECK_FAIL("lambda");

    // non-template sequences
    {

        NonTemplateSequence list({ 1, 2, 3, 4 });
        BobUIConcurrent::filter(list, keepEvenNumbers).waitForFinished();
        QCOMPARE(list, NonTemplateSequence({ 2, 4 }));
    }
    {
        NonTemplateSequence list({ 1, 2, 3, 4 });
        BobUIConcurrent::blockingFilter(list, keepEvenNumbers);
        QCOMPARE(list, NonTemplateSequence({ 2, 4 }));
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

template <typename SourceObject,
          typename ResultObject,
          typename FilterObject>
void testFilterThreadPool(BOBUIhreadPool *pool,
                          const QList<SourceObject> &sourceObjectList,
                          const QList<ResultObject> &expectedResult,
                          FilterObject filterObject)
{
    QList<SourceObject> copy1 = sourceObjectList;
    QList<SourceObject> copy2 = sourceObjectList;

    BobUIConcurrent::filter(pool, copy1, filterObject).waitForFinished();
    QCOMPARE(copy1, expectedResult);
    QCOMPARE(threadCount(), 1); // ensure the only one thread was working

    BobUIConcurrent::blockingFilter(pool, copy2, filterObject);
    QCOMPARE(copy2, expectedResult);
    QCOMPARE(threadCount(), 1); // ensure the only one thread was working
}

class KeepOddIntegers
{
public:
    bool operator()(const int &x)
    {
        storeCurrentThread();
        return x & 1;
    }
};

bool keepOddIntegers(const int &x)
{
    storeCurrentThread();
    return x & 1;
}

void tst_BobUIConcurrentFilter::filterThreadPool()
{
    const QList<int> intList {1, 2, 3, 4};
    const QList<int> intListEven {1, 3};

    auto lambdaIsOdd = [](const int &x) {
        storeCurrentThread();
        return x & 1;
    };

    BOBUIhreadPool pool;
    pool.setMaxThreadCount(1);
    QCOMPARE(semaphore.available(), 1);
    workingThreads.clear();

    testFilterThreadPool(&pool, intList, intListEven, KeepOddIntegers());
    CHECK_FAIL("functor");
    testFilterThreadPool(&pool, intList, intListEven, keepOddIntegers);
    CHECK_FAIL("function");
    testFilterThreadPool(&pool, intList, intListEven, lambdaIsOdd);
    CHECK_FAIL("lambda");

    // non-template sequences
    {

        NonTemplateSequence list({ 1, 2, 3, 4 });
        BobUIConcurrent::filter(list, keepEvenIntegers).waitForFinished();
        QCOMPARE(list, NonTemplateSequence({ 2, 4 }));
    }
    {
        NonTemplateSequence list({ 1, 2, 3, 4 });
        BobUIConcurrent::blockingFilter(list, keepEvenIntegers);
        QCOMPARE(list, NonTemplateSequence({ 2, 4 }));
    }
}

void tst_BobUIConcurrentFilter::filterWithMoveOnlyCallable()
{
    const QList<int> intListEven { 2, 4 };
    {
        QList<int> intList { 1, 2, 3, 4 };
        BobUIConcurrent::filter(intList, KeepEvenIntegersMoveOnly()).waitForFinished();
        QCOMPARE(intList, intListEven);
    }

    {
        QList<int> intList { 1, 2, 3, 4 };
        BobUIConcurrent::blockingFilter(intList, KeepEvenIntegersMoveOnly());
        QCOMPARE(intList, intListEven);
    }

    BOBUIhreadPool pool;
    {
        QList<int> intList { 1, 2, 3, 4 };
        BobUIConcurrent::filter(&pool, intList, KeepEvenIntegersMoveOnly()).waitForFinished();
        QCOMPARE(intList, intListEven);
    }
    {
        QList<int> intList { 1, 2, 3, 4 };
        BobUIConcurrent::blockingFilter(&pool, intList, KeepEvenIntegersMoveOnly());
        QCOMPARE(intList, intListEven);
    }
}

template <typename SourceObject,
          typename ResultObject,
          typename FilterObject>
void testFiltered(const QList<SourceObject> &sourceObjectList,
                  const QList<ResultObject> &expectedResult,
                  FilterObject filterObject)
{
    const QList<ResultObject> result1 = BobUIConcurrent::filtered(
                sourceObjectList, filterObject).results();
    QCOMPARE(result1, expectedResult);

    const QList<ResultObject> result2 = BobUIConcurrent::filtered(
                sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                filterObject).results();
    QCOMPARE(result2, expectedResult);

    const QList<ResultObject> result3 = BobUIConcurrent::blockingFiltered(
                sourceObjectList, filterObject);
    QCOMPARE(result3, expectedResult);

    const QList<ResultObject> result4 = BobUIConcurrent::blockingFiltered<QList<ResultObject>>(
                sourceObjectList.constBegin(), sourceObjectList.constEnd(), filterObject);
    QCOMPARE(result4, expectedResult);
}

void tst_BobUIConcurrentFilter::filtered()
{
    const QList<int> intList {1, 2, 3, 4};
    const QList<int> intListEven {2, 4};
    const QList<Number> numberList {1, 2, 3, 4};
    const QList<Number> numberListEven {2, 4};

    auto lambdaIsEven = [](const int &x) {
        return (x & 1) == 0;
    };

    testFiltered(intList, intListEven, KeepEvenIntegers());
    CHECK_FAIL("functor");
    testFiltered(intList, intListEven, keepEvenIntegers);
    CHECK_FAIL("function");
    testFiltered(numberList, numberListEven, &Number::isEven);
    CHECK_FAIL("member");
    testFiltered(intList, intListEven, lambdaIsEven);
    CHECK_FAIL("lambda");

    // non-template sequences
    {
        NonTemplateSequence list({ 1, 2, 3, 4 });
        auto future = BobUIConcurrent::filtered(list, keepEvenIntegers);
        QCOMPARE(future.results(), QList({ 2, 4 }));
    }
    {
        NonTemplateSequence list({ 1, 2, 3, 4 });
        auto result = BobUIConcurrent::blockingFiltered(list, keepEvenIntegers);
        QCOMPARE(result, NonTemplateSequence({ 2, 4 }));
    }

    {
        // rvalue sequences
        auto future = BobUIConcurrent::filtered(std::vector { 1, 2, 3, 4 }, keepEvenIntegers);
        QCOMPARE(future.results(), QList<int>({ 2, 4 }));

        auto result = BobUIConcurrent::blockingFiltered(std::vector { 1, 2, 3, 4 }, keepEvenIntegers);
        QCOMPARE(result, std::vector<int>({ 2, 4 }));
    }

    {
        // move only types sequences
        auto future = BobUIConcurrent::filtered(MoveOnlyVector<int>({ 1, 2, 3, 4 }), keepEvenIntegers);
        QCOMPARE(future.results(), QList<int>({ 2, 4 }));

#if 0
        // does not work yet
        auto result = BobUIConcurrent::blockingFiltered(
                MoveOnlyVector<int>({ 1, 2, 3, 4 }), keepEvenIntegers);
        QCOMPARE(result, std::vector<int>({ 2, 4 }));
#endif
    }
}

template <typename SourceObject,
          typename ResultObject,
          typename FilterObject>
void testFilteredThreadPool(BOBUIhreadPool *pool,
                            const QList<SourceObject> &sourceObjectList,
                            const QList<ResultObject> &expectedResult,
                            FilterObject filterObject)
{
    const QList<ResultObject> result1 = BobUIConcurrent::filtered(
                pool, sourceObjectList, filterObject).results();
    QCOMPARE(result1, expectedResult);
    QCOMPARE(threadCount(), 1); // ensure the only one thread was working

    const QList<ResultObject> result2 = BobUIConcurrent::filtered(
                pool, sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                filterObject).results();
    QCOMPARE(result2, expectedResult);
    QCOMPARE(threadCount(), 1); // ensure the only one thread was working

    const QList<ResultObject> result3 = BobUIConcurrent::blockingFiltered(
                pool, sourceObjectList, filterObject);
    QCOMPARE(result3, expectedResult);
    QCOMPARE(threadCount(), 1); // ensure the only one thread was working

    const QList<ResultObject> result4 = BobUIConcurrent::blockingFiltered<QList<ResultObject>>(
                pool, sourceObjectList.constBegin(), sourceObjectList.constEnd(), filterObject);
    QCOMPARE(result4, expectedResult);
    QCOMPARE(threadCount(), 1); // ensure the only one thread was working
}

void tst_BobUIConcurrentFilter::filteredThreadPool()
{
    const QList<int> intList {1, 2, 3, 4};
    const QList<int> intListEven {1, 3};

    auto lambdaIsOdd = [](const int &x) {
        storeCurrentThread();
        return x & 1;
    };

    BOBUIhreadPool pool;
    pool.setMaxThreadCount(1);
    QCOMPARE(semaphore.available(), 1);
    workingThreads.clear();

    testFilteredThreadPool(&pool, intList, intListEven, KeepOddIntegers());
    CHECK_FAIL("functor");
    testFilteredThreadPool(&pool, intList, intListEven, keepOddIntegers);
    CHECK_FAIL("function");
    testFilteredThreadPool(&pool, intList, intListEven, lambdaIsOdd);
    CHECK_FAIL("lambda");

    // non-template sequences
    {
        NonTemplateSequence list({ 1, 2, 3, 4 });
        auto future = BobUIConcurrent::filtered(&pool, list, keepEvenIntegers);
        QCOMPARE(future.results(), QList({ 2, 4 }));
    }
    {
        NonTemplateSequence list({ 1, 2, 3, 4 });
        auto result = BobUIConcurrent::blockingFiltered(&pool, list, keepEvenIntegers);
        QCOMPARE(result, NonTemplateSequence({ 2, 4 }));
    }

    {
        // rvalue sequences
        auto future = BobUIConcurrent::filtered(&pool, std::vector { 1, 2, 3, 4 }, keepEvenIntegers);
        QCOMPARE(future.results(), QList<int>({ 2, 4 }));

        auto result =
                BobUIConcurrent::blockingFiltered(&pool, std::vector { 1, 2, 3, 4 }, keepEvenIntegers);
        QCOMPARE(result, std::vector<int>({ 2, 4 }));
    }

    {
        // move-only sequences
        auto future = BobUIConcurrent::filtered(&pool, MoveOnlyVector<int>({ 1, 2, 3, 4 }),
                                             keepEvenIntegers);
        QCOMPARE(future.results(), QList<int>({ 2, 4 }));

#if 0
        // does not work yet
        auto result =
                BobUIConcurrent::blockingFiltered(
                        &pool, MoveOnlyVector<int>({ 1, 2, 3, 4 }), keepEvenIntegers);
        QCOMPARE(result, std::vector<int>({ 2, 4 }));
#endif
    }
}

void tst_BobUIConcurrentFilter::filteredWithMoveOnlyCallable()
{
    const QList<int> intList { 1, 2, 3, 4 };
    const QList<int> intListEven { 2, 4 };
    {
        const auto result = BobUIConcurrent::filtered(intList, KeepEvenIntegersMoveOnly()).results();
        QCOMPARE(result, intListEven);
    }
    {
        const auto result = BobUIConcurrent::filtered(
                    intList.begin(), intList.end(), KeepEvenIntegersMoveOnly()).results();
        QCOMPARE(result, intListEven);
    }
    {
        const auto result = BobUIConcurrent::blockingFiltered(intList, KeepEvenIntegersMoveOnly());
        QCOMPARE(result, intListEven);
    }
    {
        const auto result = BobUIConcurrent::blockingFiltered<QList<int>>(
                intList.begin(), intList.end(), KeepEvenIntegersMoveOnly());
        QCOMPARE(result, intListEven);
    }

    BOBUIhreadPool pool;
    {
        const auto result =
                BobUIConcurrent::filtered(&pool, intList, KeepEvenIntegersMoveOnly()).results();
        QCOMPARE(result, intListEven);
    }
    {
        const auto result = BobUIConcurrent::filtered(&pool, intList.begin(), intList.end(),
                                                   KeepEvenIntegersMoveOnly()).results();
        QCOMPARE(result, intListEven);
    }
    {
        const auto result =
                BobUIConcurrent::blockingFiltered(&pool, intList, KeepEvenIntegersMoveOnly());
        QCOMPARE(result, intListEven);
    }
    {
        const auto result = BobUIConcurrent::blockingFiltered<QList<int>>(
                &pool, intList.begin(), intList.end(), KeepEvenIntegersMoveOnly());
        QCOMPARE(result, intListEven);
    }
}

void tst_BobUIConcurrentFilter::filteredWithGenericCallable()
{
    const QList<int> intList { 1, 2, 3, 4 };
    const QList<int> intListEven { 2, 4 };

    auto keepEven = [](auto val) { return (val & 1) == 0; };
    {
        const auto result = BobUIConcurrent::filtered(intList, keepEven).results();
        QCOMPARE(result, intListEven);
    }
    {
        const auto result = BobUIConcurrent::filtered(intList.begin(), intList.end(),
                                                   keepEven).results();
        QCOMPARE(result, intListEven);
    }
    {
        const auto result = BobUIConcurrent::blockingFiltered(intList, keepEven);
        QCOMPARE(result, intListEven);
    }
    {
        const auto result = BobUIConcurrent::blockingFiltered<QList<int>>(intList.begin(),
                                                                       intList.end(),
                                                                       keepEven);
        QCOMPARE(result, intListEven);
    }

    BOBUIhreadPool pool;
    {
        const auto result = BobUIConcurrent::filtered(&pool, intList, keepEven).results();
        QCOMPARE(result, intListEven);
    }
    {
        const auto result = BobUIConcurrent::filtered(&pool, intList.begin(), intList.end(),
                                                   keepEven).results();
        QCOMPARE(result, intListEven);
    }
    {
        const auto result = BobUIConcurrent::blockingFiltered(&pool, intList, keepEven);
        QCOMPARE(result, intListEven);
    }
    {
        const auto result = BobUIConcurrent::blockingFiltered<QList<int>>(&pool, intList.begin(),
                                                                       intList.end(), keepEven);
        QCOMPARE(result, intListEven);
    }
}

template <typename SourceObject,
          typename ResultObject,
          typename FilterObject,
          typename ReduceObject>
void testFilteredReduced(const QList<SourceObject> &sourceObjectList,
                         const ResultObject &expectedResult,
                         FilterObject filterObject,
                         ReduceObject reduceObject)
{
    // Result type is passed explicitly
    {
        const ResultObject result1 = BobUIConcurrent::filteredReduced<ResultObject>(
                    sourceObjectList, filterObject, reduceObject).result();
        QCOMPARE(result1, expectedResult);

        const ResultObject result2 = BobUIConcurrent::filteredReduced<ResultObject>(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    filterObject, reduceObject).result();
        QCOMPARE(result2, expectedResult);

        const ResultObject result3 = BobUIConcurrent::blockingFilteredReduced<ResultObject>(
                    sourceObjectList, filterObject, reduceObject);
        QCOMPARE(result3, expectedResult);

        const ResultObject result4 = BobUIConcurrent::blockingFilteredReduced<ResultObject>(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    filterObject, reduceObject);
        QCOMPARE(result4, expectedResult);
    }

    // Result type is deduced
    {
        const ResultObject result1 = BobUIConcurrent::filteredReduced(
                    sourceObjectList, filterObject, reduceObject).result();
        QCOMPARE(result1, expectedResult);

        const ResultObject result2 = BobUIConcurrent::filteredReduced(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    filterObject, reduceObject).result();
        QCOMPARE(result2, expectedResult);

        const ResultObject result3 = BobUIConcurrent::blockingFilteredReduced(
                    sourceObjectList, filterObject, reduceObject);
        QCOMPARE(result3, expectedResult);

        const ResultObject result4 = BobUIConcurrent::blockingFilteredReduced(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    filterObject, reduceObject);
        QCOMPARE(result4, expectedResult);
    }
}

template <typename SourceObject,
          typename ResultObject,
          typename FilterObject,
          typename ReduceObject>
void testFilteredReduced(const QList<SourceObject> &sourceObjectList,
                         const ResultObject &expectedResult,
                         FilterObject filterObject,
                         ReduceObject reduceObject,
                         BobUIConcurrent::ReduceOptions options)
{
    const ResultObject result1 = BobUIConcurrent::filteredReduced(
                sourceObjectList, filterObject, reduceObject, options).result();
    QCOMPARE(result1, expectedResult);

    const ResultObject result2 =
            BobUIConcurrent::filteredReduced(sourceObjectList.constBegin(),
                                          sourceObjectList.constEnd(),
                                          filterObject, reduceObject, options).result();
    QCOMPARE(result2, expectedResult);

    const ResultObject result3 = BobUIConcurrent::blockingFilteredReduced(
                sourceObjectList, filterObject, reduceObject, options);
    QCOMPARE(result3, expectedResult);

    const ResultObject result4 = BobUIConcurrent::blockingFilteredReduced(
                sourceObjectList.constBegin(), sourceObjectList.constEnd(), filterObject,
                reduceObject, options);
    QCOMPARE(result4, expectedResult);
}

void numberSumReduceToNumber(Number &sum, const Number &x)
{
    sum = Number(sum.toInt() + x.toInt());
}

class NumberSumReduceToNumber
{
public:
    void operator()(Number &sum, const Number &x)
    {
        sum = Number(sum.toInt() + x.toInt());
    }
};

void tst_BobUIConcurrentFilter::filteredReduced()
{
    const QList<int> intList {1, 2, 3, 4};
    const QList<int> intListEven {2, 4};
    const QList<Number> numberList {1, 2, 3, 4};
    const QList<Number> numberListEven {2, 4};
    const int intSum = 6; // sum of even values
    const Number numberSum = 6; // sum of even values

    void (QList<int>::*pushBackInt)(QList<int>::parameter_type) = &QList<int>::push_back;
    void (QList<Number>::*pushBackNumber)(QList<Number>::parameter_type) =
            &QList<Number>::push_back;

    auto lambdaIsEven = [](const int &x) {
        return (x & 1) == 0;
    };
    auto lambdaIntSumReduce = [](int &sum, const int &x) {
        sum += x;
    };
    auto lambdaNumberSumReduce = [](Number &sum, const Number &x) {
        sum = Number(sum.toInt() + x.toInt());
    };

    // FUNCTOR-other
    testFilteredReduced(intList, intSum, KeepEvenIntegers(), IntSumReduce());
    CHECK_FAIL("functor-functor");
    testFilteredReduced(intList, intSum, KeepEvenIntegers(), intSumReduce);
    CHECK_FAIL("functor-function");
    testFilteredReduced(intList, intListEven, KeepEvenIntegers(), pushBackInt, OrderedReduce);
    CHECK_FAIL("functor-member");
    testFilteredReduced(intList, intSum, KeepEvenIntegers(), lambdaIntSumReduce);
    CHECK_FAIL("functor-lambda");

    // FUNCTION-other
    testFilteredReduced(intList, intSum, keepEvenIntegers, IntSumReduce());
    CHECK_FAIL("function-functor");
    testFilteredReduced(intList, intSum, keepEvenIntegers, intSumReduce);
    CHECK_FAIL("function-function");
    testFilteredReduced(intList, intListEven, keepEvenIntegers, pushBackInt, OrderedReduce);
    CHECK_FAIL("function-member");
    testFilteredReduced(intList, intSum, keepEvenIntegers, lambdaIntSumReduce);
    CHECK_FAIL("function-lambda");

    // MEMBER-other
    testFilteredReduced(numberList, numberSum, &Number::isEven, NumberSumReduceToNumber());
    CHECK_FAIL("member-functor");
    testFilteredReduced(numberList, numberSum, &Number::isEven, numberSumReduceToNumber);
    CHECK_FAIL("member-function");
    testFilteredReduced(numberList, numberListEven, &Number::isEven,
                        pushBackNumber, OrderedReduce);
    CHECK_FAIL("member-member");
    testFilteredReduced(numberList, numberSum, &Number::isEven, lambdaNumberSumReduce);
    CHECK_FAIL("member-lambda");

    // LAMBDA-other
    testFilteredReduced(intList, intSum, lambdaIsEven, IntSumReduce());
    CHECK_FAIL("lambda-functor");
    testFilteredReduced(intList, intSum, lambdaIsEven, intSumReduce);
    CHECK_FAIL("lambda-function");
    testFilteredReduced(intList, intListEven, lambdaIsEven, pushBackInt, OrderedReduce);
    CHECK_FAIL("lambda-member");
    testFilteredReduced(intList, intSum, lambdaIsEven, lambdaIntSumReduce);
    CHECK_FAIL("lambda-lambda");

    // non-template sequences
    {
        NonTemplateSequence list({ 1, 2, 3, 4 });
        auto future = BobUIConcurrent::filteredReduced(list, keepEvenIntegers, intSumReduce);
        QCOMPARE(future.result(), intSum);
    }
    {
        NonTemplateSequence list({ 1, 2, 3, 4 });
        auto result = BobUIConcurrent::blockingFilteredReduced(list, keepEvenIntegers, intSumReduce);
        QCOMPARE(result, intSum);
    }

    {
        // rvalue sequences
        auto future = BobUIConcurrent::filteredReduced(std::vector { 1, 2, 3, 4 }, keepEvenIntegers,
                                                    intSumReduce);
        QCOMPARE(future.result(), intSum);

        auto result = BobUIConcurrent::blockingFilteredReduced(std::vector { 1, 2, 3, 4 },
                                                            keepEvenIntegers, intSumReduce);
        QCOMPARE(result, intSum);
    }

    {
        // move only sequences
        auto future = BobUIConcurrent::filteredReduced(MoveOnlyVector<int>({ 1, 2, 3, 4 }),
                                                    keepEvenIntegers, intSumReduce);
        QCOMPARE(future.result(), intSum);

        auto result = BobUIConcurrent::blockingFilteredReduced(MoveOnlyVector<int>({ 1, 2, 3, 4 }),
                                                            keepEvenIntegers, intSumReduce);
        QCOMPARE(result, intSum);
    }
}

template <typename SourceObject,
          typename ResultObject,
          typename FilterObject,
          typename ReduceObject>
void testFilteredReducedThreadPool(BOBUIhreadPool *pool,
                                   const QList<SourceObject> &sourceObjectList,
                                   const ResultObject &expectedResult,
                                   FilterObject filterObject,
                                   ReduceObject reduceObject)
{
    // Result type is passed explicitly
    {
        const ResultObject result1 = BobUIConcurrent::filteredReduced<ResultObject>(
                    pool, sourceObjectList, filterObject, reduceObject).result();
        QCOMPARE(result1, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result2 =
                BobUIConcurrent::filteredReduced<ResultObject>(pool, sourceObjectList.constBegin(),
                                                            sourceObjectList.constEnd(), filterObject,
                                                            reduceObject).result();
        QCOMPARE(result2, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result3 = BobUIConcurrent::blockingFilteredReduced<ResultObject>(
                    pool, sourceObjectList, filterObject, reduceObject);
        QCOMPARE(result3, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result4 = BobUIConcurrent::blockingFilteredReduced<ResultObject>(
                    pool, sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    filterObject, reduceObject);
        QCOMPARE(result4, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working
    }

    // Result type is deduced
    {
        const ResultObject result1 = BobUIConcurrent::filteredReduced(
                    pool, sourceObjectList, filterObject, reduceObject).result();
        QCOMPARE(result1, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result2 =
                BobUIConcurrent::filteredReduced(pool, sourceObjectList.constBegin(),
                                              sourceObjectList.constEnd(), filterObject,
                                              reduceObject).result();
        QCOMPARE(result2, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result3 = BobUIConcurrent::blockingFilteredReduced(
                    pool, sourceObjectList, filterObject, reduceObject);
        QCOMPARE(result3, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result4 = BobUIConcurrent::blockingFilteredReduced(
                    pool, sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    filterObject, reduceObject);
        QCOMPARE(result4, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working
    }
}

void tst_BobUIConcurrentFilter::filteredReducedThreadPool()
{
    const QList<int> intList {1, 2, 3, 4};
    const int intSum = 4; // sum of even values

    auto lambdaIsOdd = [](const int &x) {
        storeCurrentThread();
        return x & 1;
    };
    auto lambdaSumReduce = [](int &sum, const int &x) {
        sum += x;
    };

    BOBUIhreadPool pool;
    pool.setMaxThreadCount(1);
    QCOMPARE(semaphore.available(), 1);
    workingThreads.clear();

    // FUNCTOR-other
    testFilteredReducedThreadPool(&pool, intList, intSum, KeepOddIntegers(), IntSumReduce());
    CHECK_FAIL("functor-functor");
    testFilteredReducedThreadPool(&pool, intList, intSum, KeepOddIntegers(), intSumReduce);
    CHECK_FAIL("functor-function");
    testFilteredReducedThreadPool(&pool, intList, intSum, KeepOddIntegers(), lambdaSumReduce);
    CHECK_FAIL("functor-lambda");

    // FUNCTION-other
    testFilteredReducedThreadPool(&pool, intList, intSum, keepOddIntegers, IntSumReduce());
    CHECK_FAIL("function-functor");
    testFilteredReducedThreadPool(&pool, intList, intSum, keepOddIntegers, intSumReduce);
    CHECK_FAIL("function-function");
    testFilteredReducedThreadPool(&pool, intList, intSum, keepOddIntegers, lambdaSumReduce);
    CHECK_FAIL("function-lambda");

    // LAMBDA-other
    testFilteredReducedThreadPool(&pool, intList, intSum, lambdaIsOdd, IntSumReduce());
    CHECK_FAIL("lambda-functor");
    testFilteredReducedThreadPool(&pool, intList, intSum, lambdaIsOdd, intSumReduce);
    CHECK_FAIL("lambda-function");
    testFilteredReducedThreadPool(&pool, intList, intSum, lambdaIsOdd, lambdaSumReduce);
    CHECK_FAIL("lambda-lambda");

    // non-template sequences
    {
        NonTemplateSequence list({ 1, 2, 3, 4 });
        auto future = BobUIConcurrent::filteredReduced(&pool, list, keepOddIntegers, intSumReduce);
        QCOMPARE(future.result(), intSum);
    }
    {
        NonTemplateSequence list({ 1, 2, 3, 4 });
        auto result =
                BobUIConcurrent::blockingFilteredReduced(&pool, list, keepOddIntegers, intSumReduce);
        QCOMPARE(result, intSum);
    }

    {
        // rvalue sequences
        auto future = BobUIConcurrent::filteredReduced(&pool, std::vector { 1, 2, 3, 4 },
                                                    keepOddIntegers, intSumReduce);
        QCOMPARE(future.result(), intSum);

        auto result = BobUIConcurrent::blockingFilteredReduced(&pool, std::vector { 1, 2, 3, 4 },
                                                            keepOddIntegers, intSumReduce);
        QCOMPARE(result, intSum);
    }

    {
        // move only sequences
        auto future = BobUIConcurrent::filteredReduced(&pool, MoveOnlyVector<int>({ 1, 2, 3, 4 }),
                                                    keepOddIntegers, intSumReduce);
        QCOMPARE(future.result(), intSum);

        auto result = BobUIConcurrent::blockingFilteredReduced(
                &pool, MoveOnlyVector<int>({ 1, 2, 3, 4 }), keepOddIntegers, intSumReduce);
        QCOMPARE(result, intSum);
    }
}

void tst_BobUIConcurrentFilter::filteredReducedWithMoveOnlyCallables()
{
    const QList<int> intList { 1, 2, 3, 4 };
    const QList<int> intListEven { 2, 4 };
    const auto sum = 6;
    {
        const auto result =
                BobUIConcurrent::filteredReduced(intList, KeepEvenIntegersMoveOnly(),
                                              IntSumReduceMoveOnly()).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result =
                BobUIConcurrent::filteredReduced(intList.begin(), intList.end(),
                                              KeepEvenIntegersMoveOnly(),
                                              IntSumReduceMoveOnly()).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingFilteredReduced(
                    intList, KeepEvenIntegersMoveOnly(), IntSumReduceMoveOnly());
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingFilteredReduced(
                    intList.begin(), intList.end(), KeepEvenIntegersMoveOnly(), IntSumReduceMoveOnly());
        QCOMPARE(result, sum);
    }

    BOBUIhreadPool pool;
    {
        const auto result =
                BobUIConcurrent::filteredReduced(&pool, intList, KeepEvenIntegersMoveOnly(),
                                              IntSumReduceMoveOnly()).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::filteredReduced(
                    &pool, intList.begin(), intList.end(),
                    KeepEvenIntegersMoveOnly(), IntSumReduceMoveOnly()).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingFilteredReduced(
                    &pool, intList, KeepEvenIntegersMoveOnly(), IntSumReduceMoveOnly());
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingFilteredReduced(
                    &pool, intList.begin(), intList.end(), KeepEvenIntegersMoveOnly(),
                    IntSumReduceMoveOnly());
        QCOMPARE(result, sum);
    }
}

void tst_BobUIConcurrentFilter::filteredReducedWithGenericCallable()
{
    const QList<int> intList { 1, 2, 3, 4 };
    const auto sum = 6;

    auto keepEven = [](auto val) { return (val & 1) == 0; };
    auto sumValues = [](auto &sum, auto val) { sum += val; };

    {
        const auto result = BobUIConcurrent::filteredReduced<int>(intList, keepEven,
                                                               sumValues).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::filteredReduced<int>(intList.begin(), intList.end(),
                                                               keepEven, sumValues).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingFilteredReduced<int>(intList, keepEven, sumValues);
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingFilteredReduced<int>(intList.begin(),
                                                                       intList.end(),
                                                                       keepEven, sumValues);
        QCOMPARE(result, sum);
    }

    BOBUIhreadPool pool;
    {
        const auto result = BobUIConcurrent::filteredReduced<int>(&pool, intList, keepEven,
                                                               sumValues).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::filteredReduced<int>(&pool, intList.begin(),
                                                               intList.end(), keepEven,
                                                               sumValues).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingFilteredReduced<int>(&pool, intList,
                                                                       keepEven, sumValues);
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingFilteredReduced<int>(&pool, intList.begin(),
                                                                       intList.end(), keepEven,
                                                                       sumValues);
        QCOMPARE(result, sum);
    }
}

void tst_BobUIConcurrentFilter::filteredReducedDifferentType()
{
    const QList<Number> numberList {1, 2, 3, 4};
    const int sum = 6; // sum of even values

    auto lambdaIsEven = [](const Number &x) {
        return (x.toInt() & 1) == 0;
    };
    auto lambdaSumReduce = [](int &sum, const Number &x) {
        sum += x.toInt();
    };

    // Test the case where reduce function of the form:
    // V function(T &result, const U &intermediate)
    // has T and U types different.

    // FUNCTOR-other
    testFilteredReduced(numberList, sum, KeepEvenNumbers(), NumberSumReduce());
    CHECK_FAIL("functor-functor");
    testFilteredReduced(numberList, sum, KeepEvenNumbers(), numberSumReduce);
    CHECK_FAIL("functor-function");
    testFilteredReduced(numberList, sum, KeepEvenNumbers(), lambdaSumReduce);
    CHECK_FAIL("functor-lambda");

    // FUNCTION-other
    testFilteredReduced(numberList, sum, keepEvenNumbers, NumberSumReduce());
    CHECK_FAIL("function-functor");
    testFilteredReduced(numberList, sum, keepEvenNumbers, numberSumReduce);
    CHECK_FAIL("function-function");
    testFilteredReduced(numberList, sum, keepEvenNumbers, lambdaSumReduce);
    CHECK_FAIL("function-lambda");

    // MEMBER-other
    testFilteredReduced(numberList, sum, &Number::isEven, NumberSumReduce());
    CHECK_FAIL("member-functor");
    testFilteredReduced(numberList, sum, &Number::isEven, numberSumReduce);
    CHECK_FAIL("member-function");
    testFilteredReduced(numberList, sum, &Number::isEven, lambdaSumReduce);
    CHECK_FAIL("member-lambda");

    // LAMBDA-other
    testFilteredReduced(numberList, sum, lambdaIsEven, NumberSumReduce());
    CHECK_FAIL("lambda-functor");
    testFilteredReduced(numberList, sum, lambdaIsEven, numberSumReduce);
    CHECK_FAIL("lambda-function");
    testFilteredReduced(numberList, sum, lambdaIsEven, lambdaSumReduce);
    CHECK_FAIL("lambda-lambda");
}

template <typename SourceObject,
          typename ResultObject,
          typename InitialObject,
          typename FilterObject,
          typename ReduceObject>
void testFilteredReducedInitialValue(const QList<SourceObject> &sourceObjectList,
                                     const ResultObject &expectedResult,
                                     FilterObject filterObject,
                                     ReduceObject reduceObject,
                                     InitialObject &&initialObject)
{
    // Result type is passed explicitly
    {
        const ResultObject result1 = BobUIConcurrent::filteredReduced<ResultObject>(
                    sourceObjectList, filterObject, reduceObject, initialObject).result();
        QCOMPARE(result1, expectedResult);

        const ResultObject result2 = BobUIConcurrent::filteredReduced<ResultObject>(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    filterObject, reduceObject, initialObject).result();
        QCOMPARE(result2, expectedResult);

        const ResultObject result3 = BobUIConcurrent::blockingFilteredReduced<ResultObject>(
                    sourceObjectList, filterObject, reduceObject, initialObject);
        QCOMPARE(result3, expectedResult);

        const ResultObject result4 = BobUIConcurrent::blockingFilteredReduced<ResultObject>(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    filterObject, reduceObject, initialObject);
        QCOMPARE(result4, expectedResult);
    }

    // Result type is deduced
    {
        const ResultObject result1 = BobUIConcurrent::filteredReduced(
                    sourceObjectList, filterObject, reduceObject, initialObject).result();
        QCOMPARE(result1, expectedResult);

        const ResultObject result2 = BobUIConcurrent::filteredReduced(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    filterObject, reduceObject, initialObject).result();
        QCOMPARE(result2, expectedResult);

        const ResultObject result3 = BobUIConcurrent::blockingFilteredReduced(
                    sourceObjectList, filterObject, reduceObject, initialObject);
        QCOMPARE(result3, expectedResult);

        const ResultObject result4 = BobUIConcurrent::blockingFilteredReduced(
                    sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    filterObject, reduceObject, initialObject);
        QCOMPARE(result4, expectedResult);
    }
}

template <typename SourceObject,
          typename ResultObject,
          typename InitialObject,
          typename FilterObject,
          typename ReduceObject>
void testFilteredReducedInitialValue(const QList<SourceObject> &sourceObjectList,
                                     const ResultObject &expectedResult,
                                     FilterObject filterObject,
                                     ReduceObject reduceObject,
                                     InitialObject &&initialObject,
                                     BobUIConcurrent::ReduceOptions options)
{
    const ResultObject result1 = BobUIConcurrent::filteredReduced(
                sourceObjectList, filterObject, reduceObject, initialObject, options).result();
    QCOMPARE(result1, expectedResult);

    const ResultObject result2 =
            BobUIConcurrent::filteredReduced(sourceObjectList.constBegin(),
                                          sourceObjectList.constEnd(), filterObject, reduceObject,
                                          initialObject, options).result();
    QCOMPARE(result2, expectedResult);

    const ResultObject result3 = BobUIConcurrent::blockingFilteredReduced(
                sourceObjectList, filterObject, reduceObject, initialObject, options);
    QCOMPARE(result3, expectedResult);

    const ResultObject result4 = BobUIConcurrent::blockingFilteredReduced(
                sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                filterObject, reduceObject, initialObject, options);
    QCOMPARE(result4, expectedResult);
}

void tst_BobUIConcurrentFilter::filteredReducedInitialValue()
{
    // This is a copy of tst_BobUIConcurrentFilter::filteredReduced
    // with the initial value parameter added

    const QList<int> intList {1, 2, 3, 4};
    const QList<int> intListInitial {10};
    const QList<int> intListAppended {10, 2, 4};
    const QList<Number> numberList {1, 2, 3, 4};
    const QList<Number> numberListInitial {10};
    const QList<Number> numberListAppended {10, 2, 4};
    const int intInitial = 10;
    const int intSum = 16; // sum of even values and initial value
    const Number numberSum = 16; // sum of even values and initial value

    void (QList<int>::*pushBackInt)(QList<int>::parameter_type) = &QList<int>::push_back;
    void (QList<Number>::*pushBackNumber)(QList<Number>::parameter_type) =
            &QList<Number>::push_back;

    auto lambdaIsEven = [](const int &x) {
        return (x & 1) == 0;
    };
    auto lambdaIntSumReduce = [](int &sum, const int &x) {
        sum += x;
    };
    auto lambdaNumberSumReduce = [](Number &sum, const Number &x) {
        sum = Number(sum.toInt() + x.toInt());
    };

    // FUNCTOR-other
    testFilteredReducedInitialValue(intList, intSum, KeepEvenIntegers(),
                                    IntSumReduce(), intInitial);
    CHECK_FAIL("functor-functor");
    testFilteredReducedInitialValue(intList, intSum, KeepEvenIntegers(),
                                    intSumReduce, intInitial);
    CHECK_FAIL("functor-function");
    testFilteredReducedInitialValue(intList, intListAppended, KeepEvenIntegers(),
                                    pushBackInt, intListInitial, OrderedReduce);
    CHECK_FAIL("functor-member");
    testFilteredReducedInitialValue(intList, intSum, KeepEvenIntegers(),
                                    lambdaIntSumReduce, intInitial);
    CHECK_FAIL("functor-lambda");

    // FUNCTION-other
    testFilteredReducedInitialValue(intList, intSum, keepEvenIntegers,
                                    IntSumReduce(), intInitial);
    CHECK_FAIL("function-functor");
    testFilteredReducedInitialValue(intList, intSum, keepEvenIntegers,
                                    intSumReduce, intInitial);
    CHECK_FAIL("function-function");
    testFilteredReducedInitialValue(intList, intListAppended, keepEvenIntegers,
                                    pushBackInt, intListInitial, OrderedReduce);
    CHECK_FAIL("function-member");
    testFilteredReducedInitialValue(intList, intSum, keepEvenIntegers,
                                    lambdaIntSumReduce, intInitial);
    CHECK_FAIL("function-lambda");

    // MEMBER-other
    testFilteredReducedInitialValue(numberList, numberSum, &Number::isEven,
                                    NumberSumReduceToNumber(), intInitial);
    CHECK_FAIL("member-functor");
    testFilteredReducedInitialValue(numberList, numberSum, &Number::isEven,
                                    numberSumReduceToNumber, intInitial);
    CHECK_FAIL("member-function");
    testFilteredReducedInitialValue(numberList, numberListAppended, &Number::isEven,
                                    pushBackNumber, numberListInitial, OrderedReduce);
    CHECK_FAIL("member-member");
    testFilteredReducedInitialValue(numberList, numberSum, &Number::isEven,
                                    lambdaNumberSumReduce, intInitial);
    CHECK_FAIL("member-lambda");

    // LAMBDA-other
    testFilteredReducedInitialValue(intList, intSum, lambdaIsEven,
                                    IntSumReduce(), intInitial);
    CHECK_FAIL("lambda-functor");
    testFilteredReducedInitialValue(intList, intSum, lambdaIsEven,
                                    intSumReduce, intInitial);
    CHECK_FAIL("lambda-function");
    testFilteredReducedInitialValue(intList, intListAppended, lambdaIsEven,
                                    pushBackInt, intListInitial, OrderedReduce);
    CHECK_FAIL("lambda-member");
    testFilteredReducedInitialValue(intList, intSum, lambdaIsEven,
                                    lambdaIntSumReduce, intInitial);
    CHECK_FAIL("lambda-lambda");

    // non-template sequences
    {
        NonTemplateSequence list({ 1, 2, 3, 4 });
        auto future =
                BobUIConcurrent::filteredReduced(list, keepEvenIntegers, intSumReduce, intInitial);
        QCOMPARE(future.result(), intSum);
    }
    {
        NonTemplateSequence list({ 1, 2, 3, 4 });
        auto result = BobUIConcurrent::blockingFilteredReduced(list, keepEvenIntegers, intSumReduce,
                                                            intInitial);
        QCOMPARE(result, intSum);
    }

    {
        // rvalue sequences
        auto future = BobUIConcurrent::filteredReduced(std::vector { 1, 2, 3, 4 }, keepEvenIntegers,
                                                    intSumReduce, intInitial);
        QCOMPARE(future.result(), intSum);

        auto result = BobUIConcurrent::blockingFilteredReduced(
                std::vector { 1, 2, 3, 4 }, keepEvenIntegers, intSumReduce, intInitial);
        QCOMPARE(result, intSum);
    }

    {
        // move only sequences
        auto future = BobUIConcurrent::filteredReduced(MoveOnlyVector<int>({ 1, 2, 3, 4 }),
                                                    keepEvenIntegers, intSumReduce, intInitial);
        QCOMPARE(future.result(), intSum);

        auto result = BobUIConcurrent::blockingFilteredReduced(
                MoveOnlyVector<int>({ 1, 2, 3, 4 }), keepEvenIntegers, intSumReduce, intInitial);
        QCOMPARE(result, intSum);
    }
}

template <typename SourceObject,
          typename ResultObject,
          typename InitialObject,
          typename FilterObject,
          typename ReduceObject>
void testFilteredReducedInitialValueThreadPool(BOBUIhreadPool *pool,
                                               const QList<SourceObject> &sourceObjectList,
                                               const ResultObject &expectedResult,
                                               FilterObject filterObject,
                                               ReduceObject reduceObject,
                                               InitialObject &&initialObject)
{
    // Result type is passed explicitly
    {
        const ResultObject result1 = BobUIConcurrent::filteredReduced<ResultObject>(
                    pool, sourceObjectList, filterObject, reduceObject, initialObject).result();
        QCOMPARE(result1, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result2 =
                BobUIConcurrent::filteredReduced<ResultObject>(pool, sourceObjectList.constBegin(),
                                                            sourceObjectList.constEnd(), filterObject,
                                                            reduceObject, initialObject).result();
        QCOMPARE(result2, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result3 = BobUIConcurrent::blockingFilteredReduced<ResultObject>(
                    pool, sourceObjectList, filterObject, reduceObject, initialObject);
        QCOMPARE(result3, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result4 = BobUIConcurrent::blockingFilteredReduced<ResultObject>(
                    pool, sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    filterObject, reduceObject, initialObject);
        QCOMPARE(result4, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working
    }

    // Result type is deduced
    {
        const ResultObject result1 = BobUIConcurrent::filteredReduced(
                    pool, sourceObjectList, filterObject, reduceObject, initialObject).result();
        QCOMPARE(result1, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result2 =
                BobUIConcurrent::filteredReduced(pool, sourceObjectList.constBegin(),
                                              sourceObjectList.constEnd(), filterObject,
                                              reduceObject, initialObject).result();
        QCOMPARE(result2, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result3 = BobUIConcurrent::blockingFilteredReduced(
                    pool, sourceObjectList, filterObject, reduceObject, initialObject);
        QCOMPARE(result3, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working

        const ResultObject result4 = BobUIConcurrent::blockingFilteredReduced(
                    pool, sourceObjectList.constBegin(), sourceObjectList.constEnd(),
                    filterObject, reduceObject, initialObject);
        QCOMPARE(result4, expectedResult);
        QCOMPARE(threadCount(), 1); // ensure the only one thread was working
    }
}

void tst_BobUIConcurrentFilter::filteredReducedInitialValueThreadPool()
{
    const QList<int> intList {1, 2, 3, 4};
    const int intInitial = 10;
    const int intSum = 14; // sum of even values and initial value

    auto lambdaIsOdd = [](const int &x) {
        storeCurrentThread();
        return x & 1;
    };
    auto lambdaSumReduce = [](int &sum, const int &x) {
        sum += x;
    };

    BOBUIhreadPool pool;
    pool.setMaxThreadCount(1);
    QCOMPARE(semaphore.available(), 1);
    workingThreads.clear();

    // FUNCTOR-other
    testFilteredReducedInitialValueThreadPool(&pool, intList, intSum, KeepOddIntegers(),
                                              IntSumReduce(), intInitial);
    CHECK_FAIL("functor-functor");
    testFilteredReducedInitialValueThreadPool(&pool, intList, intSum, KeepOddIntegers(),
                                              intSumReduce, intInitial);
    CHECK_FAIL("functor-function");
    testFilteredReducedInitialValueThreadPool(&pool, intList, intSum, KeepOddIntegers(),
                                              lambdaSumReduce, intInitial);
    CHECK_FAIL("functor-lambda");

    // FUNCTION-other
    testFilteredReducedInitialValueThreadPool(&pool, intList, intSum, keepOddIntegers,
                                              IntSumReduce(), intInitial);
    CHECK_FAIL("function-functor");
    testFilteredReducedInitialValueThreadPool(&pool, intList, intSum, keepOddIntegers,
                                              intSumReduce, intInitial);
    CHECK_FAIL("function-function");
    testFilteredReducedInitialValueThreadPool(&pool, intList, intSum, keepOddIntegers,
                                              lambdaSumReduce, intInitial);
    CHECK_FAIL("function-lambda");

    // LAMBDA-other
    testFilteredReducedInitialValueThreadPool(&pool, intList, intSum, lambdaIsOdd,
                                              IntSumReduce(), intInitial);
    CHECK_FAIL("lambda-functor");
    testFilteredReducedInitialValueThreadPool(&pool, intList, intSum, lambdaIsOdd,
                                              intSumReduce, intInitial);
    CHECK_FAIL("lambda-function");
    testFilteredReducedInitialValueThreadPool(&pool, intList, intSum, lambdaIsOdd,
                                              lambdaSumReduce, intInitial);
    CHECK_FAIL("lambda-lambda");

    // non-template sequences
    {
        NonTemplateSequence list({ 1, 2, 3, 4 });
        auto future =
                BobUIConcurrent::filteredReduced(list, keepOddIntegers, intSumReduce, intInitial);
        QCOMPARE(future.result(), intSum);
    }
    {
        NonTemplateSequence list({ 1, 2, 3, 4 });
        auto result = BobUIConcurrent::blockingFilteredReduced(list, keepOddIntegers, intSumReduce,
                                                            intInitial);
        QCOMPARE(result, intSum);
    }

    {
        // rvalue sequences
        auto future = BobUIConcurrent::filteredReduced(&pool, std::vector { 1, 2, 3, 4 },
                                                    keepOddIntegers, intSumReduce, intInitial);
        QCOMPARE(future.result(), intSum);

        auto result = BobUIConcurrent::blockingFilteredReduced(
                &pool, std::vector { 1, 2, 3, 4 }, keepOddIntegers, intSumReduce, intInitial);
        QCOMPARE(result, intSum);
    }

    {
        // move only sequences
        auto future = BobUIConcurrent::filteredReduced(&pool, MoveOnlyVector<int>({ 1, 2, 3, 4 }),
                                                    keepOddIntegers, intSumReduce, intInitial);
        QCOMPARE(future.result(), intSum);

        auto result =
                BobUIConcurrent::blockingFilteredReduced(&pool, MoveOnlyVector<int>({ 1, 2, 3, 4 }),
                                                      keepOddIntegers, intSumReduce, intInitial);
        QCOMPARE(result, intSum);
    }
}

void tst_BobUIConcurrentFilter::filteredReducedInitialValueWithMoveOnlyCallables()
{
    const QList<int> intList { 1, 2, 3, 4 };
    const QList<int> intListEven { 2, 4 };
    const auto initial = 10;
    const auto sum = 16;
    {
        const auto result =
                BobUIConcurrent::filteredReduced(intList, KeepEvenIntegersMoveOnly(),
                                              IntSumReduceMoveOnly(), initial).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result =
                BobUIConcurrent::filteredReduced(intList.begin(), intList.end(),
                                              KeepEvenIntegersMoveOnly(),
                                              IntSumReduceMoveOnly(), initial).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingFilteredReduced(
                    intList, KeepEvenIntegersMoveOnly(), IntSumReduceMoveOnly(), initial);
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingFilteredReduced(
                    intList.begin(), intList.end(), KeepEvenIntegersMoveOnly(), IntSumReduceMoveOnly(),
                    initial);
        QCOMPARE(result, sum);
    }

    BOBUIhreadPool pool;
    {
        const auto result =
                BobUIConcurrent::filteredReduced(&pool, intList, KeepEvenIntegersMoveOnly(),
                                              IntSumReduceMoveOnly(), initial).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result =
                BobUIConcurrent::filteredReduced(
                    &pool, intList.begin(), intList.end(),
                    KeepEvenIntegersMoveOnly(), IntSumReduceMoveOnly(), initial).result();
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingFilteredReduced(
                    &pool, intList, KeepEvenIntegersMoveOnly(), IntSumReduceMoveOnly(), initial);
        QCOMPARE(result, sum);
    }
    {
        const auto result = BobUIConcurrent::blockingFilteredReduced(
                    &pool, intList.begin(), intList.end(), KeepEvenIntegersMoveOnly(),
                    IntSumReduceMoveOnly(), initial);
        QCOMPARE(result, sum);
    }
}

void tst_BobUIConcurrentFilter::filteredReducedDifferentTypeInitialValue()
{
    const QList<Number> numberList {1, 2, 3, 4};
    const int initial = 10;
    const int sum = 16; // sum of even values and initial value

    auto lambdaIsEven = [](const Number &x) {
        return (x.toInt() & 1) == 0;
    };
    auto lambdaSumReduce = [](int &sum, const Number &x) {
        sum += x.toInt();
    };

    // Test the case where reduce function of the form:
    // V function(T &result, const U &intermediate)
    // has T and U types different.

    // FUNCTOR-other
    testFilteredReducedInitialValue(numberList, sum, KeepEvenNumbers(), NumberSumReduce(), initial);
    CHECK_FAIL("functor-functor");
    testFilteredReducedInitialValue(numberList, sum, KeepEvenNumbers(), numberSumReduce, initial);
    CHECK_FAIL("functor-function");
    testFilteredReducedInitialValue(numberList, sum, KeepEvenNumbers(), lambdaSumReduce, initial);
    CHECK_FAIL("functor-lambda");

    // FUNCTION-other
    testFilteredReducedInitialValue(numberList, sum, keepEvenNumbers, NumberSumReduce(), initial);
    CHECK_FAIL("function-functor");
    testFilteredReducedInitialValue(numberList, sum, keepEvenNumbers, numberSumReduce, initial);
    CHECK_FAIL("function-function");
    testFilteredReducedInitialValue(numberList, sum, keepEvenNumbers, lambdaSumReduce, initial);
    CHECK_FAIL("function-lambda");

    // MEMBER-other
    testFilteredReducedInitialValue(numberList, sum, &Number::isEven, NumberSumReduce(), initial);
    CHECK_FAIL("member-functor");
    testFilteredReducedInitialValue(numberList, sum, &Number::isEven, numberSumReduce, initial);
    CHECK_FAIL("member-function");
    testFilteredReducedInitialValue(numberList, sum, &Number::isEven, lambdaSumReduce, initial);
    CHECK_FAIL("member-lambda");

    // LAMBDA-other
    testFilteredReducedInitialValue(numberList, sum, lambdaIsEven, NumberSumReduce(), initial);
    CHECK_FAIL("lambda-functor");
    testFilteredReducedInitialValue(numberList, sum, lambdaIsEven, numberSumReduce, initial);
    CHECK_FAIL("lambda-function");
    testFilteredReducedInitialValue(numberList, sum, lambdaIsEven, lambdaSumReduce, initial);
    CHECK_FAIL("lambda-lambda");
}

void tst_BobUIConcurrentFilter::filteredReduceOptionConvertableToResultType()
{
    const QList<int> intList { 1, 2, 3 };
    const int sum = 4;
    BOBUIhreadPool p;
    ReduceOption ro = OrderedReduce;

    // With container
    QCOMPARE(BobUIConcurrent::filteredReduced(intList, keepOddIntegers, intSumReduce, ro).result(),
             sum);
    QCOMPARE(BobUIConcurrent::blockingFilteredReduced(intList, keepOddIntegers, intSumReduce, ro),
             sum);

    // With iterators
    QCOMPARE(BobUIConcurrent::filteredReduced(intList.begin(), intList.end(), keepOddIntegers,
                                           intSumReduce, ro).result(), sum);
    QCOMPARE(BobUIConcurrent::blockingFilteredReduced(intList.begin(), intList.end(), keepOddIntegers,
                                                   intSumReduce, ro), sum);

    // With custom BOBUIhreadPool;
    QCOMPARE(BobUIConcurrent::filteredReduced(&p, intList, keepOddIntegers, intSumReduce, ro).result(),
             sum);
    QCOMPARE(BobUIConcurrent::blockingFilteredReduced(&p, intList, keepOddIntegers, intSumReduce, ro),
             sum);
    QCOMPARE(BobUIConcurrent::filteredReduced(&p, intList.begin(), intList.end(), keepOddIntegers,
                                           intSumReduce, ro).result(), sum);
    QCOMPARE(BobUIConcurrent::blockingFilteredReduced(&p, intList.begin(), intList.end(),
                                                   keepOddIntegers, intSumReduce, ro), sum);

    // The same as above, but specify the result type explicitly (this invokes different overloads)
    QCOMPARE(BobUIConcurrent::filteredReduced<int>(intList, keepOddIntegers, intSumReduce,
                                                ro).result(), sum);
    QCOMPARE(BobUIConcurrent::blockingFilteredReduced<int>(intList, keepOddIntegers, intSumReduce, ro),
             sum);

    QCOMPARE(BobUIConcurrent::filteredReduced<int>(intList.begin(), intList.end(), keepOddIntegers,
                                                intSumReduce, ro).result(), sum);
    QCOMPARE(BobUIConcurrent::blockingFilteredReduced<int>(intList.begin(), intList.end(),
                                                        keepOddIntegers, intSumReduce, ro), sum);

    QCOMPARE(BobUIConcurrent::filteredReduced<int>(&p, intList, keepOddIntegers, intSumReduce,
                                                ro).result(), sum);
    QCOMPARE(BobUIConcurrent::blockingFilteredReduced<int>(&p, intList, keepOddIntegers, intSumReduce,
                                                        ro), sum);
    QCOMPARE(BobUIConcurrent::filteredReduced<int>(&p, intList.begin(), intList.end(), keepOddIntegers,
                                                intSumReduce, ro).result(),sum);
    QCOMPARE(BobUIConcurrent::blockingFilteredReduced<int>(&p, intList.begin(), intList.end(),
                                                        keepOddIntegers, intSumReduce, ro), sum);
}

bool filterfn(int i)
{
    return (i % 2);
}

void tst_BobUIConcurrentFilter::resultAt()
{
    QList<int> ints;
    for (int i = 0; i < 1000; ++i)
        ints << i;

    QFuture<int> future = BobUIConcurrent::filtered(ints, filterfn);
    future.waitForFinished();

    for (int i = 0; i < future.resultCount(); ++i) {
        QCOMPARE(future.resultAt(i), ints.at(i * 2 + 1));
    }
}

bool waitFilterfn(const int &i)
{
    BOBUIest::qWait(1);
    return (i % 2);
}

void tst_BobUIConcurrentFilter::incrementalResults()
{
    const int count = 200;
    QList<int> ints;
    for (int i = 0; i < count; ++i)
        ints << i;

    QFuture<int> future = BobUIConcurrent::filtered(ints, waitFilterfn);

    QList<int> results;

    while (future.isFinished() == false) {
        for (int i = 0; i < future.resultCount(); ++i) {
            results += future.resultAt(i);
        }
        BOBUIest::qWait(1);
    }

    QCOMPARE(future.isFinished(), true);
    QCOMPARE(future.resultCount(), count / 2);
    QCOMPARE(future.results().size(), count / 2);
}

void tst_BobUIConcurrentFilter::noDetach()
{
    {
        QList<int> l = QList<int>() << 1;
        QVERIFY(l.isDetached());

        QList<int> ll = l;
        QVERIFY(!l.isDetached());

        BobUIConcurrent::filtered(l, waitFilterfn).waitForFinished();

        QVERIFY(!l.isDetached());
        QVERIFY(!ll.isDetached());

        BobUIConcurrent::blockingFiltered(l, waitFilterfn);

        QVERIFY(!l.isDetached());
        QVERIFY(!ll.isDetached());

        BobUIConcurrent::filteredReduced(l, waitFilterfn, intSumReduce).waitForFinished();

        QVERIFY(!l.isDetached());
        QVERIFY(!ll.isDetached());

        BobUIConcurrent::filter(l, waitFilterfn).waitForFinished();
        if (!l.isDetached())
            QEXPECT_FAIL("", "BOBUIBUG-20688: Known unstable failure", Abort);
        QVERIFY(l.isDetached());
        QVERIFY(ll.isDetached());
    }
    {
        const QList<int> l = QList<int>() << 1;
        QVERIFY(l.isDetached());

        const QList<int> ll = l;
        QVERIFY(!l.isDetached());

        BobUIConcurrent::filtered(l, waitFilterfn).waitForFinished();

        QVERIFY(!l.isDetached());
        QVERIFY(!ll.isDetached());

        BobUIConcurrent::filteredReduced(l, waitFilterfn, intSumReduce).waitForFinished();

        QVERIFY(!l.isDetached());
        QVERIFY(!ll.isDetached());
    }
}

void tst_BobUIConcurrentFilter::stlContainers()
{
    std::vector<int> vector;
    vector.push_back(1);
    vector.push_back(2);

    std::vector<int> vector2 =  BobUIConcurrent::blockingFiltered(vector, waitFilterfn);
    QCOMPARE(vector2.size(), (std::vector<int>::size_type)(1));
    QCOMPARE(vector2[0], 1);

    std::list<int> list;
    list.push_back(1);
    list.push_back(2);

    std::list<int> list2 =  BobUIConcurrent::blockingFiltered(list, waitFilterfn);
    QCOMPARE(list2.size(), (std::list<int>::size_type)(1));
    QCOMPARE(*list2.begin(), 1);

    BobUIConcurrent::filtered(list, waitFilterfn).waitForFinished();
    BobUIConcurrent::filtered(vector, waitFilterfn).waitForFinished();
    BobUIConcurrent::filtered(vector.begin(), vector.end(), waitFilterfn).waitForFinished();

    BobUIConcurrent::blockingFilter(list, waitFilterfn);
    QCOMPARE(list2.size(), (std::list<int>::size_type)(1));
    QCOMPARE(*list2.begin(), 1);
}

void tst_BobUIConcurrentFilter::stlContainersLambda()
{
    auto waitFilterLambda = [](const int &i) {
        return waitFilterfn(i);
    };

    std::vector<int> vector;
    vector.push_back(1);
    vector.push_back(2);

    std::vector<int> vector2 = BobUIConcurrent::blockingFiltered(vector, waitFilterLambda);
    QCOMPARE(vector2.size(), (std::vector<int>::size_type)(1));
    QCOMPARE(vector2[0], 1);

    std::list<int> list;
    list.push_back(1);
    list.push_back(2);

    std::list<int> list2 = BobUIConcurrent::blockingFiltered(list, waitFilterLambda);
    QCOMPARE(list2.size(), (std::list<int>::size_type)(1));
    QCOMPARE(*list2.begin(), 1);

    BobUIConcurrent::filtered(list, waitFilterLambda).waitForFinished();
    BobUIConcurrent::filtered(vector, waitFilterLambda).waitForFinished();
    BobUIConcurrent::filtered(vector.begin(), vector.end(), waitFilterLambda).waitForFinished();

    BobUIConcurrent::blockingFilter(list, waitFilterLambda);
    QCOMPARE(list.size(), (std::list<int>::size_type)(1));
    QCOMPARE(*list.begin(), 1);
}

BOBUIEST_MAIN(tst_BobUIConcurrentFilter)
#include "tst_bobuiconcurrentfilter.moc"
