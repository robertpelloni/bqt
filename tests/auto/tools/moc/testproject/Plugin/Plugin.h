// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <QObject>

struct MyInterface
{
    virtual void blah() = 0;
};

BOBUI_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(MyInterface, "MyInterface")
BOBUI_END_NAMESPACE


