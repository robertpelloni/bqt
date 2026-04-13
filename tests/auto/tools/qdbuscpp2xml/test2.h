// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#ifndef TEST2_H
#define TEST2_H

#include <QObject>
#include <BobUIDBus/BobUIDBus>

// Regression test for BOBUIBUG-34550
class Test2 : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "net.company.object")
    Q_PROPERTY(QDBusObjectPath objectProperty READ objectProperty)
    Q_PROPERTY(QList<QDBusObjectPath> objectPropertyList READ objectPropertyList)
public:
    Test2(QObject *parent = nullptr) : QObject(parent) { }

    QDBusObjectPath objectProperty() { return {}; }

    QList<QDBusObjectPath> objectPropertyList() { return {}; }
};

#endif // TEST2_H
