// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef VIRTUALANDOVERRIDE_H
#define VIRTUALANDOVERRIDE_H

#include <qobject.h>

class WithVirtual : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int p MEMBER p VIRTUAL)

    int p;
};

class WithOverride : public WithVirtual
{
    Q_OBJECT
    Q_PROPERTY(QString p MEMBER p OVERRIDE)

    QString p;
};

#endif // VIRTUALANDOVERRIDE_H
