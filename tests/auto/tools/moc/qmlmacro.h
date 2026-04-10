// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#ifndef QMlMACRO_H
#define QMlMACRO_H

#include <QObject>
#include <QByteArray>

struct QmlMacro : QObject
{
    Q_OBJECT
    Q_CLASSINFO("QML.Element", "auto")

signals:
    void f(const QByteArray &b);
};
#endif
