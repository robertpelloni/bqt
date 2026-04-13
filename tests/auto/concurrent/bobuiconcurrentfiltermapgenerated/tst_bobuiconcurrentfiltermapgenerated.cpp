// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <bobuiconcurrentfilter.h>
#include <bobuiconcurrentmap.h>
#include <QCoreApplication>
#include <QList>
#include <BOBUIest>

#include "../testhelper_functions.h"
#include "generation_helpers.h"

#include "tst_bobuiconcurrentfiltermapgenerated.h"

using namespace BobUIConcurrent;

// START_GENERATED_IMPLEMENTATIONS (see generate_tests.py)

void tst_BobUIConcurrentFilterMapGenerated::test1()
{
    /* test for
    template<typename Sequence, typename KeepFunctor>
    void blockingFilter(BOBUIhreadPool* pool, Sequence & sequence, KeepFunctor filterFunction);

    with
      inputsequence=standard
      inputitemtype=standard
      filterfunction=functor
      filterfunctionpassing=lvalue
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

    BobUIConcurrent::blockingFilter(&pool, input_sequence, filter);

    auto expected_result = []() {
        std::vector<SequenceItem<tag_input>> result;
        result.push_back(SequenceItem<tag_input>(1, true));
        result.push_back(SequenceItem<tag_input>(3, true));
        result.push_back(SequenceItem<tag_input>(5, true));
        return result;
    }();

    QCOMPARE(input_sequence, expected_result);
}
// END_GENERATED_IMPLEMENTATION (see generate_tests.py)

BOBUIEST_MAIN(tst_BobUIConcurrentFilterMapGenerated)
