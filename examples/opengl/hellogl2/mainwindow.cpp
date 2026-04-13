// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "mainwindow.h"
#include "window.h"
#include <QApplication>
#include <QKeySequence>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>

MainWindow::MainWindow()
{
#ifndef Q_OS_WASM
    QMenu *menuWindow = menuBar()->addMenu(tr("&Window"));
    menuWindow->addAction(tr("Add new"), QKeySequence(BobUI::CTRL | BobUI::Key_N),
                          this, &MainWindow::onAddNew);
    menuWindow->addAction(tr("Quit"), QKeySequence(BobUI::CTRL | BobUI::Key_Q),
                          qApp, QApplication::closeAllWindows);
#endif
    onAddNew();
}

void MainWindow::onAddNew()
{
    if (!centralWidget())
        setCentralWidget(new Window);
    else
        QMessageBox::information(this, tr("Cannot Add New Window"),
                                 tr("Already occupied. Undock first."));
}
