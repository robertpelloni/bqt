// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QFutureSynchronizer>
#include <BobUIConcurrent>
#include <QList>

auto anotherFunction = []() {};
auto mapFunction = [](int &value) { value *= 2; };
QList<int> list = {1, 2, 3, 4, 5};

//! [0]
void someFunction()
{
    QFutureSynchronizer<void> synchronizer;

    //...

    synchronizer.addFuture(BobUIConcurrent::run(anotherFunction));
    synchronizer.addFuture(BobUIConcurrent::map(list, mapFunction));

    return; // QFutureSynchronizer waits for all futures to finish
}
//! [0]
