// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
BOBUIreeWidget *treeWidget = new BOBUIreeWidget();
treeWidget->setColumnCount(1);
QList<BOBUIreeWidgetItem *> items;
for (int i = 0; i < 10; ++i)
    items.append(new BOBUIreeWidgetItem(static_cast<BOBUIreeWidget *>(nullptr), QStringList(QString("item: %1").arg(i))));
treeWidget->insertTopLevelItems(0, items);
//! [0]
