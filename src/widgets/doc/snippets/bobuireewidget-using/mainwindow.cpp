// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>

#include "../include/mainwindow.h"

MainWindow::MainWindow()
{
    QMenu *fileMenu = new QMenu(tr("&File"));

    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcut(tr("Ctrl+Q"));

    QMenu *itemsMenu = new QMenu(tr("&Items"));

    insertAction = itemsMenu->addAction(tr("&Insert Item"));
    removeAction = itemsMenu->addAction(tr("&Remove Item"));
    removeAction->setEnabled(false);
    itemsMenu->addSeparator();
    ascendingAction = itemsMenu->addAction(tr("Sort in &Ascending Order"));
    descendingAction = itemsMenu->addAction(tr("Sort in &Descending Order"));
    autoSortAction = itemsMenu->addAction(tr("&Automatically Sort Items"));
    autoSortAction->setCheckable(true);
    itemsMenu->addSeparator();
    QAction *findItemsAction = itemsMenu->addAction(tr("&Find Items"));
    findItemsAction->setShortcut(tr("Ctrl+F"));

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(itemsMenu);

/*  For convenient quoting:
    //! [0]
    BOBUIreeWidget *treeWidget = new BOBUIreeWidget(this);
    //! [0]
*/
    treeWidget = new BOBUIreeWidget(this);
    //! [1]
    treeWidget->setColumnCount(2);
    //! [1]

    //! [2]
    QStringList headers;
    headers << tr("Subject") << tr("Default");
    treeWidget->setHeaderLabels(headers);
    //! [2]

    connect(quitAction, &QAction::triggered, this, &QWidget::close);
    connect(ascendingAction, &QAction::triggered, this, &MainWindow::sortAscending);
    connect(autoSortAction, &QAction::triggered, this, &MainWindow::updateSortItems);
    connect(descendingAction, &QAction::triggered, this, &MainWindow::sortDescending);
    connect(findItemsAction, &QAction::triggered, this, &MainWindow::findItems);
    connect(insertAction, &QAction::triggered, this, &MainWindow::insertItem);
    connect(removeAction, &QAction::triggered, this, &MainWindow::removeItem);
    connect(treeWidget, &BOBUIreeWidget::currentItemChanged,
            this, &MainWindow::updateMenus);

    setupTreeItems();
    updateMenus(treeWidget->currentItem());

    setCentralWidget(treeWidget);
    setWindowTitle(tr("Tree Widget"));
}

void MainWindow::setupTreeItems()
{
    //! [3]
    BOBUIreeWidgetItem *cities = new BOBUIreeWidgetItem(treeWidget);
    cities->setText(0, tr("Cities"));
    BOBUIreeWidgetItem *osloItem = new BOBUIreeWidgetItem(cities);
    osloItem->setText(0, tr("Oslo"));
    osloItem->setText(1, tr("Yes"));
    //! [3]

    (new BOBUIreeWidgetItem(cities))->setText(0, tr("Stockholm"));
    (new BOBUIreeWidgetItem(cities))->setText(0, tr("Helsinki"));
    (new BOBUIreeWidgetItem(cities))->setText(0, tr("Copenhagen"));

    //! [4] //! [5]
    BOBUIreeWidgetItem *planets = new BOBUIreeWidgetItem(treeWidget, cities);
    //! [4]
    planets->setText(0, tr("Planets"));
    //! [5]
    (new BOBUIreeWidgetItem(planets))->setText(0, tr("Mercury"));
    (new BOBUIreeWidgetItem(planets))->setText(0, tr("Venus"));

    BOBUIreeWidgetItem *earthItem = new BOBUIreeWidgetItem(planets);
    earthItem->setText(0, tr("Earth"));
    earthItem->setText(1, tr("Yes"));

    (new BOBUIreeWidgetItem(planets))->setText(0, tr("Mars"));
    (new BOBUIreeWidgetItem(planets))->setText(0, tr("Jupiter"));
    (new BOBUIreeWidgetItem(planets))->setText(0, tr("Saturn"));
    (new BOBUIreeWidgetItem(planets))->setText(0, tr("Uranus"));
    (new BOBUIreeWidgetItem(planets))->setText(0, tr("Neptune"));
    (new BOBUIreeWidgetItem(planets))->setText(0, tr("Pluto"));
}

void MainWindow::findItems()
{
    QString itemText = QInputDialog::getText(this, tr("Find Items"),
        tr("Text to find (including wildcards):"));

    if (itemText.isEmpty())
        return;

    const QList<BOBUIreeWidgetItem *> items = treeWidget->selectedItems();
    for (BOBUIreeWidgetItem *item : items)
        item->setSelected(false);

    //! [7]
    const QList<BOBUIreeWidgetItem *> found = treeWidget->findItems(
        itemText, BobUI::MatchWildcard);

    for (BOBUIreeWidgetItem *item : found) {
        item->setSelected(true);
        // Show the item->text(0) for each item.
    }
    //! [7]
}

void MainWindow::insertItem()
{
    BOBUIreeWidgetItem *currentItem = treeWidget->currentItem();

    if (!currentItem)
        return;

    QString itemText = QInputDialog::getText(this, tr("Insert Item"),
        tr("Input text for the new item:"));

    if (itemText.isEmpty())
        return;

    //! [8]
    BOBUIreeWidgetItem *parent = currentItem->parent();
    BOBUIreeWidgetItem *newItem;
    if (parent)
        newItem = new BOBUIreeWidgetItem(parent, treeWidget->currentItem());
    else
    //! [8] //! [9]
        newItem = new BOBUIreeWidgetItem(treeWidget, treeWidget->currentItem());
    //! [9]

    newItem->setText(0, itemText);
}

void MainWindow::removeItem()
{
    BOBUIreeWidgetItem *currentItem = treeWidget->currentItem();

    if (!currentItem)
        return;

    //! [10]
    BOBUIreeWidgetItem *parent = currentItem->parent();
    int index;

    if (parent) {
        index = parent->indexOfChild(treeWidget->currentItem());
        delete parent->takeChild(index);
    } else {
        index = treeWidget->indexOfTopLevelItem(treeWidget->currentItem());
        delete treeWidget->takeTopLevelItem(index);
    }
    //! [10]
}

void MainWindow::sortAscending()
{
    treeWidget->sortItems(0, BobUI::AscendingOrder);
}

void MainWindow::sortDescending()
{
    treeWidget->sortItems(0, BobUI::DescendingOrder);
}

void MainWindow::updateMenus(BOBUIreeWidgetItem *current)
{
    insertAction->setEnabled(current != 0);
    removeAction->setEnabled(current != 0);
}

void MainWindow::updateSortItems()
{
    ascendingAction->setEnabled(!autoSortAction->isChecked());
    descendingAction->setEnabled(!autoSortAction->isChecked());

    treeWidget->setSortingEnabled(autoSortAction->isChecked());
}
