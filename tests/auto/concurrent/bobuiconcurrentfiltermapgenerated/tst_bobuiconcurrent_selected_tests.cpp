// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "tst_bobuiconcurrentfiltermapgenerated.h"

void tst_BobUIConcurrentFilterMapGenerated::mapReduceThroughDifferentTypes()
{
    /* test for
    template<typename typename ResultType, typename Iterator, typename MapFunctor, typename
    ReduceFunctor, typename reductionitemtype> ResultType blockingMappedReduced(Iterator begin,
    Iterator end, MapFunctor function, ReduceFunctor reduceFunction, reductionitemtype &&
    initialValue, ReduceOptions);

    with
      inputsequence=standard
      inputitemtype=standard
      maptype=different
      mappeditemtype=standard
      reductiontype=different
      reductionitemtype=standard
      mapfunction=function
      mapfunctionpassing=lvalue
      reductionfunction=function
      reductionfunctionpassing=lvalue
      reductioninitialvaluepassing=lvalue
      reductionoptions=unspecified
    */

    auto input_sequence = []() {
        std::vector<SequenceItem<tag_input>> result;
        result.push_back(SequenceItem<tag_input>(1, true));
        result.push_back(SequenceItem<tag_input>(2, true));
        result.push_back(SequenceItem<tag_input>(3, true));
        result.push_back(SequenceItem<tag_input>(4, true));
        result.push_back(SequenceItem<tag_input>(5, true));
        result.push_back(SequenceItem<tag_input>(6, true));
        return result;
    }();

    auto map = myMap<SequenceItem<tag_input>, SequenceItem<tag_mapped>>;
    auto reductor = myReduce<SequenceItem<tag_mapped>, SequenceItem<tag_reduction>>;
    auto initialvalue = SequenceItem<tag_reduction>(0, true);

    auto result = BobUIConcurrent::blockingMappedReduced(input_sequence.begin(), input_sequence.end(),
                                                      map, reductor, initialvalue);

    auto expected_result = SequenceItem<tag_reduction>(42, true);
    QCOMPARE(result, expected_result);
}

void tst_BobUIConcurrentFilterMapGenerated::moveOnlyFilterObject()
{
    /* test for
    template<typename Sequence, typename KeepFunctor>
    void blockingFilter(BOBUIhreadPool* pool, Sequence & sequence, KeepFunctor filterFunction);

    with
      inputsequence=standard
      inputitemtype=standard
      filterfunction=moveonlyfunctor
      filterfunctionpassing=rvalue
    */

    BOBUIhreadPool pool;
    pool.setMaxThreadCount(1);
    auto input_sequence = []() {
        std::vector<SequenceItem<tag_input>> result;
        result.push_back(SequenceItem<tag_input>(1, true));
        result.push_back(SequenceItem<tag_input>(2, true));
        result.push_back(SequenceItem<tag_input>(3, true));
        result.push_back(SequenceItem<tag_input>(4, true));
        result.push_back(SequenceItem<tag_input>(5, true));
        result.push_back(SequenceItem<tag_input>(6, true));
        return result;
    }();

    BobUIConcurrent::blockingFilter(&pool, input_sequence,
                                 MyMoveOnlyFilter<SequenceItem<tag_input>> {});

    auto expected_result = []() {
        std::vector<SequenceItem<tag_input>> result;
        result.push_back(SequenceItem<tag_input>(1, true));
        result.push_back(SequenceItem<tag_input>(3, true));
        result.push_back(SequenceItem<tag_input>(5, true));
        return result;
    }();

    QCOMPARE(input_sequence, expected_result);
}

void tst_BobUIConcurrentFilterMapGenerated::moveOnlyMapObject()
{
    /* test for
    template<typename Sequence, typename MapFunctor>
    void blockingMap(BOBUIhreadPool* pool, Sequence & sequence, MapFunctor function);

    with
      inputsequence=standard
      inputitemtype=standard
      mapfunction=moveonlyfunctor
      mapfunctionpassing=rvalue
    */

    BOBUIhreadPool pool;
    pool.setMaxThreadCount(1);
    auto input_sequence = []() {
        std::vector<SequenceItem<tag_input>> result;
        result.push_back(SequenceItem<tag_input>(1, true));
        result.push_back(SequenceItem<tag_input>(2, true));
        result.push_back(SequenceItem<tag_input>(3, true));
        result.push_back(SequenceItem<tag_input>(4, true));
        result.push_back(SequenceItem<tag_input>(5, true));
        result.push_back(SequenceItem<tag_input>(6, true));
        return result;
    }();

    BobUIConcurrent::blockingMap(&pool, input_sequence,
                              MyMoveOnlyInplaceMap<SequenceItem<tag_input>> {});

    auto expected_result = []() {
        std::vector<SequenceItem<tag_input>> result;
        result.push_back(SequenceItem<tag_input>(2, true));
        result.push_back(SequenceItem<tag_input>(4, true));
        result.push_back(SequenceItem<tag_input>(6, true));
        result.push_back(SequenceItem<tag_input>(8, true));
        result.push_back(SequenceItem<tag_input>(10, true));
        result.push_back(SequenceItem<tag_input>(12, true));
        return result;
    }();

    QCOMPARE(input_sequence, expected_result);
}

void tst_BobUIConcurrentFilterMapGenerated::moveOnlyReduceObject()
{
    /* test for
    template<typename typename ResultType, typename Sequence, typename MapFunctor, typename
    ReduceFunctor> ResultType blockingMappedReduced(BOBUIhreadPool* pool, const Sequence & sequence,
    MapFunctor function, ReduceFunctor reduceFunction, ReduceOptions);

    with
      inputsequence=standard
      inputsequencepassing=lvalue
      inputitemtype=standard
      maptype=same
      mappeditemtype=standard
      reductiontype=same
      reductionitemtype=standard
      mapfunction=functor
      mapfunctionpassing=lvalue
      reductionfunction=moveonlyfunctor
      reductionfunctionpassing=rvalue
      reductionoptions=unspecified
    */

    BOBUIhreadPool pool;
    pool.setMaxThreadCount(1);
    auto input_sequence = []() {
        std::vector<SequenceItem<tag_input>> result;
        result.push_back(SequenceItem<tag_input>(1, true));
        result.push_back(SequenceItem<tag_input>(2, true));
        result.push_back(SequenceItem<tag_input>(3, true));
        result.push_back(SequenceItem<tag_input>(4, true));
        result.push_back(SequenceItem<tag_input>(5, true));
        result.push_back(SequenceItem<tag_input>(6, true));
        return result;
    }();

    auto map = MyMap<SequenceItem<tag_input>, SequenceItem<tag_input>> {};

    auto result = BobUIConcurrent::blockingMappedReduced<SequenceItem<tag_input>>(
            &pool, input_sequence, map,
            MyMoveOnlyReduce<SequenceItem<tag_input>, SequenceItem<tag_input>> {});

    auto expected_result = SequenceItem<tag_input>(42, true);

    QCOMPARE(result, expected_result);
}

void tst_BobUIConcurrentFilterMapGenerated::functorAsReduction()
{
    /* test for
    template<typename typename ResultType, typename Sequence, typename KeepFunctor, typename
    ReduceFunctor, typename reductionitemtype> ResultType blockingFilteredReduced(BOBUIhreadPool* pool,
    const Sequence & sequence, KeepFunctor filterFunction, ReduceFunctor reduceFunction,
    reductionitemtype && initialValue, ReduceOptions);

    with
      inputsequence=standard
      inputsequencepassing=lvalue
      inputitemtype=standard
      reductiontype=same
      reductionitemtype=standard
      filterfunction=functor
      filterfunctionpassing=lvalue
      reductionfunction=functor
      reductionfunctionpassing=lvalue
      reductioninitialvaluepassing=lvalue
      reductionoptions=unspecified
    */

    BOBUIhreadPool pool;
    pool.setMaxThreadCount(1);
    auto input_sequence = []() {
        std::vector<SequenceItem<tag_input>> result;
        result.push_back(SequenceItem<tag_input>(1, true));
        result.push_back(SequenceItem<tag_input>(2, true));
        result.push_back(SequenceItem<tag_input>(3, true));
        result.push_back(SequenceItem<tag_input>(4, true));
        result.push_back(SequenceItem<tag_input>(5, true));
        result.push_back(SequenceItem<tag_input>(6, true));
        return result;
    }();
    auto filter = MyFilter<SequenceItem<tag_input>> {};

    auto reductor = MyReduce<SequenceItem<tag_input>, SequenceItem<tag_input>> {};
    auto initialvalue = SequenceItem<tag_input>(0, true);

    auto result = BobUIConcurrent::blockingFilteredReduced<SequenceItem<tag_input>>(
            &pool, input_sequence, filter, reductor, initialvalue);

    auto expected_result = SequenceItem<tag_input>(9, true);

    QCOMPARE(result, expected_result);
}

void tst_BobUIConcurrentFilterMapGenerated::moveOnlyReductionItem()
{
    /* test for
    template<typename typename ResultType, typename Sequence, typename KeepFunctor, typename
    ReduceFunctor, typename reductionitemtype> ResultType blockingFilteredReduced(BOBUIhreadPool* pool,
    const Sequence & sequence, KeepFunctor filterFunction, ReduceFunctor reduceFunction,
    reductionitemtype && initialValue, ReduceOptions);

    with
      inputsequence=standard
      inputsequencepassing=lvalue
      inputitemtype=standard
      reductiontype=different
      reductionitemtype=moveonly
      filterfunction=moveonlyfunctor
      filterfunctionpassing=rvalue
      reductionfunction=function
      reductionfunctionpassing=lvalue
      reductioninitialvaluepassing=rvalue
      reductionoptions=unspecified
    */
/* TODO: does not work yet
    BOBUIhreadPool pool;
    pool.setMaxThreadCount(1);
    auto input_sequence = []() {
        std::vector<SequenceItem<tag_input>> result;
        result.push_back(SequenceItem<tag_input>(1, true));
        result.push_back(SequenceItem<tag_input>(2, true));
        result.push_back(SequenceItem<tag_input>(3, true));
        result.push_back(SequenceItem<tag_input>(4, true));
        result.push_back(SequenceItem<tag_input>(5, true));
        result.push_back(SequenceItem<tag_input>(6, true));
        return result;
    }();

    auto reductor = myReduce<SequenceItem<tag_input>, MoveOnlySequenceItem<tag_reduction>>;

    auto result = BobUIConcurrent::blockingFilteredReduced(
            &pool, input_sequence, MyMoveOnlyFilter<SequenceItem<tag_input>> {}, reductor,
            MoveOnlySequenceItem<tag_reduction>(0, true));

    auto expected_result = MoveOnlySequenceItem<tag_reduction>(9, true);

    QCOMPARE(result, expected_result);*/
}

void tst_BobUIConcurrentFilterMapGenerated::noDefaultConstructorItemMapped()
{
    /* test for
    template<typename typename ResultType, typename Sequence, typename MapFunctor, typename
    ReduceFunctor, typename reductionitemtype> ResultType blockingMappedReduced(BOBUIhreadPool* pool,
    const Sequence & sequence, MapFunctor function, ReduceFunctor reduceFunction, reductionitemtype
    && initialValue, ReduceOptions);

    with
      inputsequence=standard
      inputsequencepassing=lvalue
      inputitemtype=standard
      maptype=same
      mappeditemtype=standard
      reductiontype=different
      reductionitemtype=noconstruct
      mapfunction=functor
      mapfunctionpassing=lvalue
      reductionfunction=function
      reductionfunctionpassing=lvalue
      reductioninitialvaluepassing=lvalue
      reductionoptions=unspecified
    */

    BOBUIhreadPool pool;
    pool.setMaxThreadCount(1);
    auto input_sequence = []() {
        std::vector<SequenceItem<tag_input>> result;
        result.push_back(SequenceItem<tag_input>(1, true));
        result.push_back(SequenceItem<tag_input>(2, true));
        result.push_back(SequenceItem<tag_input>(3, true));
        result.push_back(SequenceItem<tag_input>(4, true));
        result.push_back(SequenceItem<tag_input>(5, true));
        result.push_back(SequenceItem<tag_input>(6, true));
        return result;
    }();

    auto map = MyMap<SequenceItem<tag_input>, SequenceItem<tag_input>> {};
    auto reductor = myReduce<SequenceItem<tag_input>, NoConstructSequenceItem<tag_reduction>>;
    auto initialvalue = NoConstructSequenceItem<tag_reduction>(0, true);

    auto result =
            BobUIConcurrent::blockingMappedReduced(&pool, input_sequence, map, reductor, initialvalue);

    auto expected_result = NoConstructSequenceItem<tag_reduction>(42, true);

    QCOMPARE(result, expected_result);
}

void tst_BobUIConcurrentFilterMapGenerated::noDefaultConstructorItemFiltered()
{
    BOBUIhreadPool pool;
    pool.setMaxThreadCount(1);
    auto input_sequence = []() {
        std::vector<SequenceItem<tag_input>> result;
        result.push_back(SequenceItem<tag_input>(1, true));
        result.push_back(SequenceItem<tag_input>(2, true));
        result.push_back(SequenceItem<tag_input>(3, true));
        result.push_back(SequenceItem<tag_input>(4, true));
        result.push_back(SequenceItem<tag_input>(5, true));
        result.push_back(SequenceItem<tag_input>(6, true));
        return result;
    }();

    auto filter = MyFilter<SequenceItem<tag_input>> {};
    auto reductor = myReduce<SequenceItem<tag_input>, NoConstructSequenceItem<tag_reduction>>;
    auto initialvalue = NoConstructSequenceItem<tag_reduction>(0, true);

    auto result = BobUIConcurrent::blockingFilteredReduced(&pool, input_sequence, filter, reductor,
                                                        initialvalue);

    auto expected_result = NoConstructSequenceItem<tag_reduction>(9, true);

    QCOMPARE(result, expected_result);
}
