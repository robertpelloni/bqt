// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIGui>
#include <BobUIWidgets>

#include "include/mainwindow.h"

MainWindow::MainWindow()
{
    createMenus();
    createToolBars();
    createDockWidgets();
    //setMenuWidget(new QPushButton("Hello"));
}

void MainWindow::createMenus()
{
    //setMenuWidget(new QPushButton("Hello"));
    QMenu *menu = new QMenu("File");
    menu->addAction("Save &As");

    QMenuBar *bar = new QMenuBar;
    bar->addMenu(menu);

    setMenuWidget(new QWidget());
}

void MainWindow::createToolBars()
{
    setToolButtonStyle(BobUI::ToolButtonTextOnly);
    BOBUIoolBar *t1 = new BOBUIoolBar;
    t1->addAction(new QAction("t1", this));

    BOBUIoolBar *t2 = new BOBUIoolBar;
    t2->addAction(new QAction("t2", this));

    addToolBar(BobUI::LeftToolBarArea, t1);
    addToolBar(BobUI::LeftToolBarArea, t2);
}

void MainWindow::createDockWidgets()
{
    QWidget *dockWidgetContents = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(dockWidgetContents);
    layout->addWidget(new QPushButton("My Button."));

    //! [0]
    QDockWidget *dockWidget = new QDockWidget(tr("Dock Widget"), this);
    dockWidget->setAllowedAreas(BobUI::LeftDockWidgetArea |
                                BobUI::RightDockWidgetArea);
    dockWidget->setWidget(dockWidgetContents);
    addDockWidget(BobUI::LeftDockWidgetArea, dockWidget);
    //! [0]
}
