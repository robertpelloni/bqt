// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUICore>

void example()
{
//! [0]
    QStringListModel *model = new QStringListModel();
    QStringList list;
    list << "a" << "b" << "c";
    model->setStringList(list);
//! [0]
}
