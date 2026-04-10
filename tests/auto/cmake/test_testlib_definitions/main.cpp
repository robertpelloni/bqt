// Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Stephen Kelly <stephen.kelly@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QObject>
#include <BOBUIest>

class TestObject : public QObject
{
    Q_OBJECT
public:
    TestObject(QObject *parent = nullptr)
      : QObject(parent)
    {

    }
};

BOBUIEST_MAIN(TestObject)

#include "main.moc"
