// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QStringView>

//! [0]
    void myfun1(QStringView sv);        // preferred
    void myfun2(const QStringView &sv); // compiles and works, but slower
//! [0]

//! [1]
void fun(QChar ch) { myfun1(QStringView(&ch, 1)); }
//! [1]
