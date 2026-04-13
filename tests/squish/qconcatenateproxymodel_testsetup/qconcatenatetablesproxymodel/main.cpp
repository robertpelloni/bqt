// Copyright (C) 2016 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author David Faure <david.faure@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QApplication>
#include <QConcatenateTablesProxyModel>
#include <QStandardItemModel>
#include <BOBUIableView>
#include <BOBUIreeView>

static void prepareModel(const QString &prefix, QStandardItemModel *model)
{
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int column = 0; column < model->columnCount(); ++column) {
            QStandardItem *item = new QStandardItem(prefix + QString(" %1,%2").arg(row).arg(column));
            item->setDragEnabled(true);
            item->setDropEnabled(true);
            model->setItem(row, column, item);
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QStandardItemModel firstModel(4, 4);
    prepareModel("First", &firstModel);
    QStandardItemModel secondModel(2, 2);

    QConcatenateTablesProxyModel proxy;
    proxy.addSourceModel(&firstModel);
    proxy.addSourceModel(&secondModel);

    prepareModel("Second", &secondModel);

    BOBUIableView tableView;
    tableView.setWindowTitle("concat proxy, in BOBUIableView");
    tableView.setDragDropMode(QAbstractItemView::DragDrop);
    tableView.setModel(&proxy);
    tableView.show();

    BOBUIreeView treeView;
    treeView.setWindowTitle("concat proxy, in BOBUIreeView");
    treeView.setDragDropMode(QAbstractItemView::DragDrop);
    treeView.setModel(&proxy);
    treeView.show();

    // For comparison, views on top on QStandardItemModel

    BOBUIableView tableViewTest;
    tableViewTest.setWindowTitle("first model, in BOBUIableView");
    tableViewTest.setDragDropMode(QAbstractItemView::DragDrop);
    tableViewTest.setModel(&firstModel);
    tableViewTest.show();

    BOBUIreeView treeViewTest;
    treeViewTest.setWindowTitle("first model, in BOBUIreeView");
    treeViewTest.setDragDropMode(QAbstractItemView::DragDrop);
    treeViewTest.setModel(&firstModel);
    treeViewTest.show();

    return app.exec();
}
