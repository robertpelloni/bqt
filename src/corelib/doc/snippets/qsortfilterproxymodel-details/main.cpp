// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIGui>
#include <QApplication>
#include <QSortFilterProxyModel>
#include <QWidget>
#include <BOBUIreeView>

class MyItemModel : public QStandardItemModel
{
public:
    MyItemModel(QWidget *parent = nullptr);
};

MyItemModel::MyItemModel(QWidget *parent)
    : QStandardItemModel(parent)
{};

class Widget : public QWidget
{
public:
    Widget(QWidget *parent = nullptr);
};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
//! [0] //! [1]
        BOBUIreeView *treeView = new BOBUIreeView;
//! [0]
        MyItemModel *model = new MyItemModel(this);

        treeView->setModel(model);
//! [1]

//! [2]
        MyItemModel *sourceModel = new MyItemModel(this);
        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);

        proxyModel->setSourceModel(sourceModel);
        treeView->setModel(proxyModel);
//! [2]

//! [3]
        treeView->setSortingEnabled(true);
//! [3]

//! [4]
        proxyModel->sort(2, BobUI::AscendingOrder);
//! [4] //! [5]
        proxyModel->setFilterRegularExpression(QRegularExpression("\\.png", QRegularExpression::CaseInsensitiveOption));
        proxyModel->setFilterKeyColumn(1);
//! [5]
}
