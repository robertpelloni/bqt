// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef OLDSTYLE_CASTS_H
#define OLDSTYLE_CASTS_H
#include <BobUICore/qobject.h>

class OldStyleCast: public QObject
{
    Q_OBJECT
public:


signals:

public slots:
    inline void foo() {}
    inline int bar(int, int*, const int *, volatile int *, const int * volatile *) { return 0; }
    inline void slot(int, QObject * const) {}
};

#endif // OLDSTYLE_CASTS_H
