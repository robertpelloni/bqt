// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <bobuiconcurrentfilter.h>
#include <bobuiconcurrentmap.h>
#include <BOBUIest>

#include "generation_helpers.h"

using namespace BobUIConcurrent;

class tst_BobUIConcurrentFilterMapGenerated : public QObject
{
    Q_OBJECT

private slots:
    void mapReduceThroughDifferentTypes();
    void moveOnlyFilterObject();
    void moveOnlyMapObject();
    void moveOnlyReduceObject();
    void functorAsReduction();
    void moveOnlyReductionItem();
    void noDefaultConstructorItemMapped();
    void noDefaultConstructorItemFiltered();
    // START_GENERATED_SLOTS (see generate_tests.py)
    void test1();
    // END_GENERATED_SLOTS (see generate_tests.py)
};
