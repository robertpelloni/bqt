// Copyright (C) 2026 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#pragma once

#include <QObject>

class MyObject1 : public QObject
{
    Q_OBJECT
public:
    explicit MyObject1(QObject *parent = nullptr);
};
