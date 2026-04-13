// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
#include <iostream>
#include <QDebug>
#include <QObject>
#include "myobject.h"

MyObject::MyObject()
    : QObject()
{
    std::cout << "MyObject::MyObject()\n";
}
//! [0]

MyObject::~MyObject()
{
    qDebug() << "MyObject::~MyObject()";
}
