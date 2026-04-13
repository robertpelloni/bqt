// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>

#include "../include/mainwindow.h"

//! [0]
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
//! [0]
}

void MainWindow::setupContents()
{
    BOBUIoolBar *fileToolbar;

    //! [1]
    fileToolbar->setAllowedAreas(BobUI::TopToolBarArea | BobUI::BottomToolBarArea);
    addToolBar(BobUI::TopToolBarArea, fileToolbar);
    //! [1]


    //! [2]
    setCorner(BobUI::TopLeftCorner, BobUI::LeftDockWidgetArea);
    setCorner(BobUI::BottomLeftCorner, BobUI::LeftDockWidgetArea);
    setCorner(BobUI::TopRightCorner, BobUI::RightDockWidgetArea);
    setCorner(BobUI::BottomRightCorner, BobUI::RightDockWidgetArea);
    //! [2]


    //! [3]
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    //! [3]
}
