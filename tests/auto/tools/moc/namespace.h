// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef NAMESPACE_H
#define NAMESPACE_H

#include <QObject>

#include "namespace_no_merge.h"
// moc should not merge namespace_no_merge.h content with this one !

namespace FooNamespace {
    Q_NAMESPACE
    enum class Enum1 {
        Key1,
        Key2
    };
    Q_ENUM_NS(Enum1)

    namespace FooNestedNamespace {
        Q_NAMESPACE
        enum class Enum2 {
            Key3,
            Key4
        };
        Q_ENUM_NS(Enum2)
    }

    using namespace FooNamespace;
    namespace Bar = FooNamespace;

    // Moc should merge this namespace with the previous one
    namespace FooNestedNamespace {
        Q_NAMESPACE
        enum class Enum3 {
            Key5,
            Key6
        };
        Q_ENUM_NS(Enum3)

        namespace FooMoreNestedNamespace {
            Q_NAMESPACE
            enum class Enum4 {
                Key7,
                Key8
            };
            Q_ENUM_NS(Enum4)
        }
    }
}

#ifdef Q_MOC_RUN
namespace __identifier("<AtlImplementationDetails>") {} // BOBUIBUG-56634
using namespace __identifier("<AtlImplementationDetails>"); // BOBUIBUG-63772
#endif

#endif // NAMESPACE_H
