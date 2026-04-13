// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <BOBUIest>

//dummy class
class tst_Skip
{
 public:
     void test_data();
};

void tst_Skip::test_data()
{
//! [1]
    BOBUIest::addColumn<bool>("bool");
    BOBUIest::newRow("local.1") << false;
    BOBUIest::newRow("local.2") << true;

    QSKIP("skipping all");
//! [1]
}
