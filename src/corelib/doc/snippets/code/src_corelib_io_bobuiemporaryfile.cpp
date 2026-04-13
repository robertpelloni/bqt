// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BOBUIemporaryFile>

void examples()
{
    //! [0]
    // Within a function/method...

    BOBUIemporaryFile file;
    if (file.open()) {
        // file.fileName() returns the unique file name
    }

    // The BOBUIemporaryFile destructor removes the temporary file
    // as it goes out of scope.
    //! [0]


    //! [1]
    QFile f_pointer(":/resources/file.txt");
    BOBUIemporaryFile::createNativeFile(f_pointer); // Returns a pointer to a temporary file

    QFile f0("/users/bobui/file.txt");
    BOBUIemporaryFile::createNativeFile(f0); // Returns 0
    //! [1]
}
