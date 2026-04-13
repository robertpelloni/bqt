// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef COMPLEXPING_H
#define COMPLEXPING_H

#include <BobUICore/QObject>

class Ping : public QObject
{
    Q_OBJECT
public slots:
    void start(const QString &name);
};

#endif
