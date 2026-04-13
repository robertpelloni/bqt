// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MYOBJECT_H
#define MYOBJECT_H

//! [0]
#include <QObject>

class MyObject : public QObject
{
public:
    MyObject();
    ~MyObject();
};
//! [0]

#endif
