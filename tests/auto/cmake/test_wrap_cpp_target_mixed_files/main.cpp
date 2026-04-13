// Copyright (C) 2026 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QObject>
#include "myobject1.h"
#include "myobject2.h"

class MyObject3 : public QObject {
    Q_OBJECT
public:
    MyObject3() = default;
};

int main()
{
    MyObject1 obj1;
    MyObject2 obj2;
    MyObject3 obj3;
    return 0;
}

#include "main.moc"
