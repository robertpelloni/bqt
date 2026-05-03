// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <qobject.h>

class SomeObject : public QObject
{
    Q_OBJECT
public:
    SomeObject();
signals:
    void someSignal();
};
