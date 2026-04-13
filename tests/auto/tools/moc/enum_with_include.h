// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef ENUM_WITH_INCLUDE_H
#define ENUM_WITH_INCLUDE_H
#include <BobUICore/QObject>

class Foo : public QObject {
    enum en {
       #include <enum_inc.h>
    };

    enum class en2 {
       #include <enum_inc.h>
       reference = 42
    };
    Q_OBJECT
};
#endif
