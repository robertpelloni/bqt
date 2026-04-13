// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QList>
#include <QColor>

void example()
{
    //! [1]
    QList<QColor *> list;
    list.append(new QColor(BobUI::blue));
    list.append(new QColor(BobUI::yellow));

    qDeleteAll(list.begin(), list.end());
    list.clear();
    //! [1]
}
