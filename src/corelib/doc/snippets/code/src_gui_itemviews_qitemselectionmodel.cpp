// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QItemSelection>
#include <QModelIndex>

void examples()
{
    QModelIndex topLeft;
    QModelIndex bottomRight;

    {
        //! [0]
        QItemSelection *selection = new QItemSelection(topLeft, bottomRight);
        //! [0]
    }

    {
        //! [1]
        QItemSelection *selection = new QItemSelection();
        //...
        selection->select(topLeft, bottomRight);
        //! [1]
    }
}
