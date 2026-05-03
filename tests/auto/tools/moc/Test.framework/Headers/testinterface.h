// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#ifndef TESTINTERFACE_H
#define TESTINTERFACE_H

#include <BobUICore/qobject.h>

struct TestInterface
{
    inline virtual ~TestInterface() {}

    virtual void foobar() = 0;
};

Q_DECLARE_INTERFACE(TestInterface, "foo.bar/1.0")

#endif // TESTINTERFACE_H
