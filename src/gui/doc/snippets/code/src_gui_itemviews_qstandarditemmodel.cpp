// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QStandardItemModel>
#include <BOBUIreeView>
#include <QWidget>

namespace src_gui_itemviews_qstandarditemmodel {
struct MyWidget : public QWidget
{
    void wrapper2();
    void clicked(const QModelIndex &index);
    QModelIndex index() { return QModelIndex(); }
    QStandardItemModel *myStandardItemModel;
};

void wrapper0() {

//! [0]
QStandardItemModel model(4, 4);
for (int row = 0; row < model.rowCount(); ++row) {
    for (int column = 0; column < model.columnCount(); ++column) {
        QStandardItem *item = new QStandardItem(QString("row %0, column %1").arg(row).arg(column));
        model.setItem(row, column, item);
    }
}
//! [0]

} // wrapper0


void wrapper1() {

//! [1]
QStandardItemModel model;
QStandardItem *parentItem = model.invisibleRootItem();
for (int i = 0; i < 4; ++i) {
    QStandardItem *item = new QStandardItem(QString("item %0").arg(i));
    parentItem->appendRow(item);
    parentItem = item;
}
//! [1]

} // wrapper1


void MyWidget::wrapper2() {
//! [2]
BOBUIreeView *treeView = new BOBUIreeView(this);
treeView->setModel(myStandardItemModel);
connect(treeView, &BOBUIreeView::clicked,
        this, &MyWidget::clicked);
//! [2]

} // wrapper2


//! [3]
void MyWidget::clicked(const QModelIndex &index)
{
    QStandardItem *item = myStandardItemModel->itemFromIndex(index);
    // Do stuff with the item ...
}
//! [3]


void wrapper3() {
BOBUIreeView *treeView = nullptr;
MyWidget *item = nullptr;
//! [4]
treeView->scrollTo(item->index());
//! [4]

} // wrapper3
} // src_gui_itemviews_qstandarditemmodel
