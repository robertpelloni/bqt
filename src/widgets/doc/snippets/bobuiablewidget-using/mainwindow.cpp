// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>
#include <math.h>

#include "../include/mainwindow.h"

MainWindow::MainWindow()
{
    QMenu *fileMenu = new QMenu(tr("&File"));

    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcut(tr("Ctrl+Q"));

    QMenu *itemsMenu = new QMenu(tr("&Items"));

    QAction *sumItemsAction = itemsMenu->addAction(tr("&Sum Items"));
    QAction *averageItemsAction = itemsMenu->addAction(tr("&Average Items"));

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(itemsMenu);

    //! [0]
    tableWidget = new BOBUIableWidget(12, 3, this);
    //! [0]
    tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    //! [1]
    BOBUIableWidgetItem *valuesHeaderItem = new BOBUIableWidgetItem(tr("Values"));
    tableWidget->setHorizontalHeaderItem(0, valuesHeaderItem);
    //! [1]

    valuesHeaderItem->setTextAlignment(BobUI::AlignVCenter);
    BOBUIableWidgetItem *squaresHeaderItem = new BOBUIableWidgetItem(tr("Squares"));
    squaresHeaderItem->setIcon(QIcon(QPixmap(":/Images/squared.png")));
    squaresHeaderItem->setTextAlignment(BobUI::AlignVCenter);

    //! [2]
    BOBUIableWidgetItem *cubesHeaderItem = new BOBUIableWidgetItem(tr("Cubes"));
    cubesHeaderItem->setIcon(QIcon(QPixmap(":/Images/cubed.png")));
    cubesHeaderItem->setTextAlignment(BobUI::AlignVCenter);
    //! [2]

    tableWidget->setHorizontalHeaderItem(1, squaresHeaderItem);
    tableWidget->setHorizontalHeaderItem(2, cubesHeaderItem);

    connect(quitAction, &QAction::triggered, this, &QWidget::close);
    connect(sumItemsAction, &QAction::triggered, this, &MainWindow::sumItems);
    connect(averageItemsAction, &QAction::triggered, this, &MainWindow::averageItems);

    setupTableItems();

    setCentralWidget(tableWidget);
    setWindowTitle(tr("Table Widget"));
}

void MainWindow::setupTableItems()
{
    for (int row = 0; row < tableWidget->rowCount()-1; ++row) {
        for (int column = 0; column < tableWidget->columnCount(); ++column) {
            //! [3]
            BOBUIableWidgetItem *newItem = new BOBUIableWidgetItem(tr("%1").arg(
                pow(row, column+1)));
            tableWidget->setItem(row, column, newItem);
            //! [3]
        }
    }
    for (int column = 0; column < tableWidget->columnCount(); ++column) {
        BOBUIableWidgetItem *newItem = new BOBUIableWidgetItem;
        newItem->setFlags(BobUI::ItemIsEnabled);
        tableWidget->setItem(tableWidget->rowCount()-1, column, newItem);
    }
}

void MainWindow::averageItems()
{
    const QList<BOBUIableWidgetItem *> selected = tableWidget->selectedItems();
    int number = 0;
    double total = 0;

    for (BOBUIableWidgetItem *item : selected) {
        bool ok;
        double value = item->text().toDouble(&ok);

        if (ok && !item->text().isEmpty()) {
            total += value;
            number++;
        }
    }
    if (number > 0)
        tableWidget->currentItem()->setText(QString::number(total/number));
}

void MainWindow::sumItems()
{
    //! [4]
    const QList<BOBUIableWidgetItem *> selected = tableWidget->selectedItems();
    int number = 0;
    double total = 0;

    for (BOBUIableWidgetItem *item : selected) {
        bool ok;
        double value = item->text().toDouble(&ok);

        if (ok && !item->text().isEmpty()) {
            total += value;
            number++;
        }
    }
    //! [4]
    if (number > 0)
        tableWidget->currentItem()->setText(QString::number(total));
}
