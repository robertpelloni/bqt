// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef FILTEROBJECT_H
#define FILTEROBJECT_H

#include <QObject>

class FilterObject : public QObject
{
    Q_OBJECT

public:
    FilterObject(QObject *parent = nullptr);
    bool eventFilter(QObject *object, QEvent *event) override;
    void setFilteredObject(QObject *object);

private:
    QObject *target;
};

#endif
