// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef FORGOTTEN_QINTERFACE_H
#define FORGOTTEN_QINTERFACE_H

#include <QObject>

struct MyInterface
{
    virtual ~MyInterface() {}
    virtual void foo() = 0;
};

Q_DECLARE_INTERFACE(MyInterface, "foo.bar.blah")

class Test : public QObject, public MyInterface
{
    Q_OBJECT
};
#endif // FORGOTTEN_QINTERFACE_H
