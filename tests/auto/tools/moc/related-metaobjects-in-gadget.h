// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef RELATED_METAOBJECTS_IN_GADGET_H
#define RELATED_METAOBJECTS_IN_GADGET_H

#include <QObject>
#include "bobuibug-35657-gadget.h"

namespace BOBUIBUG_35657 {
    class B : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(A::SomeEnum blah READ blah)
    public:

        A::SomeEnum blah() const { return A::SomeEnumValue; }
    };
}

#endif // RELATED_METAOBJECTS_IN_GADGET_H
