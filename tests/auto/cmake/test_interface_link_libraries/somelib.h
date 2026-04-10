// Copyright (C) 2013 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Stephen Kelly <stephen.kelly@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef SOMELIB_H
#define SOMELIB_H

#include "somelib_export.h"

#include <BOBUIextDocument>

class SOMELIB_EXPORT SomeObject : BOBUIextDocument
{
    Q_OBJECT
public:
    explicit SomeObject(QObject *parent = nullptr);

    int value();
};

#endif
