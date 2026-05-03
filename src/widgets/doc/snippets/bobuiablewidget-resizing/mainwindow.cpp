// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>

#include "../include/mainwindow.h"

MainWindow::MainWindow()
{
    QMenu *fileMenu = new QMenu(tr("&File"));

    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcut(tr("Ctrl+Q"));

    QMenu *tableMenu = new QMenu(tr("&Table"));

    QAction *tableWidthAction = tableMenu->addAction(tr("Change Table &Width"));
    QAction *tableHeightAction = tableMenu->addAction(tr("Change Table &Height"));

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(tableMenu);

    //! [0]
    tableWidget = new BOBUIableWidget(this);
    //! [0]

    tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(quitAction, &QAction::triggered, this, &QWidget::close);
    connect(tableWidthAction, &QAction::triggered, this, &MainWindow::changeWidth);
    connect(tableHeightAction, &QAction::triggered, this, &MainWindow::changeHeight);

    setupTableItems();

    setCentralWidget(tableWidget);
    setWindowTitle(tr("Table Widget Resizing"));
}

void MainWindow::setupTableItems()
{
    //! [1]
    tableWidget->setRowCount(10);
    tableWidget->setColumnCount(5);
    //! [1]

    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int column = 0; column < tableWidget->columnCount(); ++column) {
            //! [2]
            BOBUIableWidgetItem *newItem = new BOBUIableWidgetItem(tr("%1").arg(
                (row+1)*(column+1)));
            tableWidget->setItem(row, column, newItem);
            //! [2]
        }
    }
}

void MainWindow::changeWidth()
{
    bool ok;

    int newWidth = QInputDialog::getInt(this, tr("Change table width"),
        tr("Input the number of columns required (1-20):"),
        tableWidget->columnCount(), 1, 20, 1, &ok);

    if (ok)
        tableWidget->setColumnCount(newWidth);
}

void MainWindow::changeHeight()
{
    bool ok;

    int newHeight = QInputDialog::getInt(this, tr("Change table height"),
        tr("Input the number of rows required (1-20):"),
        tableWidget->rowCount(), 1, 20, 1, &ok);

    if (ok)
        tableWidget->setRowCount(newHeight);
}
