// Copyright (C) 2026 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/qobject.h>

class MyObject : public QObject
{
    Q_OBJECT
public:
    MyObject(QObject *parent = nullptr)
        : QObject(parent)
    {
    }
};

void instantiateObject()
{
    MyObject obj;
    obj.setObjectName("MyObject");
}

#include "myobject.moc"
