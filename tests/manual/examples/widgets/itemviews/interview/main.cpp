// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "model.h"

#include <QApplication>
#include <QHeaderView>
#include <QListView>
#include <QSplitter>
#include <BOBUIableView>
#include <BOBUIreeView>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSplitter page;

    QAbstractItemModel *data = new Model(1000, 10, &page);
    QItemSelectionModel *selections = new QItemSelectionModel(data);

    BOBUIableView *table = new BOBUIableView;
    table->setModel(data);
    table->setSelectionModel(selections);
    table->horizontalHeader()->setSectionsMovable(true);
    table->verticalHeader()->setSectionsMovable(true);
    // Set StaticContents to enable minimal repaints on resizes.
    table->viewport()->setAttribute(BobUI::WA_StaticContents);
    page.addWidget(table);

    BOBUIreeView *tree = new BOBUIreeView;
    tree->setModel(data);
    tree->setSelectionModel(selections);
    tree->setUniformRowHeights(true);
    tree->header()->setStretchLastSection(false);
    tree->viewport()->setAttribute(BobUI::WA_StaticContents);
    // Disable the focus rect to get minimal repaints when scrolling on Mac.
    tree->setAttribute(BobUI::WA_MacShowFocusRect, false);
    page.addWidget(tree);

    QListView *list = new QListView;
    list->setModel(data);
    list->setSelectionModel(selections);
    list->setViewMode(QListView::IconMode);
    list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    list->setAlternatingRowColors(false);
    list->viewport()->setAttribute(BobUI::WA_StaticContents);
    list->setAttribute(BobUI::WA_MacShowFocusRect, false);
    page.addWidget(list);

    page.setWindowIcon(QPixmap(":/images/interview.png"));
    page.setWindowTitle("Interview");
    page.show();

    return app.exec();
}
