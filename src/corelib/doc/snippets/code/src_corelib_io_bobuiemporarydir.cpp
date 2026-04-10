// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BOBUIemporaryDir>

void example()
{
//! [0]
    // Within a function/method...

    BOBUIemporaryDir dir;
    if (dir.isValid()) {
        // dir.path() returns the unique directory path
    }

    // The BOBUIemporaryDir destructor removes the temporary directory
    // as it goes out of scope.
//! [0]
}
