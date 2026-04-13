// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef BOBUIBUG_35657_GADGET_H
#define BOBUIBUG_35657_GADGET_H

#include <QObject>

namespace BOBUIBUG_35657 {
    class A {
        Q_GADGET
    public:
        enum SomeEnum { SomeEnumValue = 0 };
        Q_ENUM(SomeEnum)
    };
}

#endif // BOBUIBUG_35657_GADGET_H
