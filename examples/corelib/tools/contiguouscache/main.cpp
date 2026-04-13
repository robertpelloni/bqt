// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "randomlistmodel.h"
#include <QListView>
#include <QApplication>

int main(int c, char **v)
{
    QApplication a(c, v);

    QListView view;
    view.setUniformItemSizes(true);
    view.setModel(new RandomListModel(&view));
    view.show();

    return a.exec();
}
