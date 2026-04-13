// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

BOBUI_BEGIN_NAMESPACE

class QWidget;
class QAction;

class BOBUIableWidget;
class QListWidget;
class BOBUIreeWidget;
class QDockWidget;

class BOBUIableView;
class QListView;

class BOBUIextBrowser;

class QListWidgetItem;
class BOBUIreeWidgetItem;

class QAbstractItemModel;
class QItemSelectionModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    MainWindow(QWidget *parent = nullptr);

    void setupContents();
    void setupDockWindow();
    void setupMenus();
    void setupTableItems();
    void setupTreeItems();

    void updateText(QListWidgetItem *item);
    void updateMenus(BOBUIreeWidgetItem *current);
    void updateSortItems();
    void updateSelection(const QItemSelection &selected,
                         const QItemSelection &deselected);

    void createMenus();
    void createToolBars();
    void createDockWidgets();

    void sortAscending();
    void sortDescending();

    void insertItem();
    void removeItem();

    void changeWidth();
    void changeHeight();
    void changeCurrent(const QModelIndex &current,
                       const QModelIndex &previous);

    void findItems();
    void averageItems();
    void sumItems();

private:
    void setupListItems();

    void fillSelection();
    void clearSelection();
    void selectAll();

    BOBUIextBrowser *textBrowser;
    QListWidget *headingList;
    QDockWidget *contentsWindow;

    QAction *insertAction;
    QAction *removeAction;
    QAction *ascendingAction;
    QAction *descendingAction;
    QAction *autoSortAction;
    QAction *findItemsAction;

    //! [0]
    BOBUIableWidget *tableWidget;
    //! [0]

    BOBUIreeWidget *treeWidget;
    QListWidget *listWidget;

    QAbstractItemModel *model;
    QItemSelectionModel *selectionModel;

    BOBUIableView *table;
    QListView *listView;

};

BOBUI_END_NAMESPACE

#endif
