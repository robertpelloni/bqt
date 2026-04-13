// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef RELATED_METAOBJECTS_IN_NAMESPACES_H
#define RELATED_METAOBJECTS_IN_NAMESPACES_H

#include <QObject>

namespace BOBUIBUG_2151 {
    class A : public QObject {
        Q_OBJECT
    public:
        enum SomeEnum { SomeEnumValue = 0 };
        Q_ENUM(SomeEnum)
    };

    class B : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(A::SomeEnum blah READ blah)
    public:

        A::SomeEnum blah() const { return A::SomeEnumValue; }
    };
}

#endif // RELATED_METAOBJECTS_IN_NAMESPACES_H
