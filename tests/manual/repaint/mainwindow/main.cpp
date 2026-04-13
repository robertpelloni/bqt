// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QApplication>
#include <QMainWindow>
#include <BOBUIoolBar>
#include <QDockWidget>
#include <QStatusBar>
#include <QSpinBox>
#include <QAction>

#include "../shared/shared.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QMainWindow mainWindow;

    mainWindow.setCentralWidget(new StaticWidget());
    mainWindow.setStatusBar(new QStatusBar());

    QDockWidget *dockWidget = new QDockWidget();
    dockWidget->setWidget(new StaticWidget());
    mainWindow.addDockWidget(BobUI::LeftDockWidgetArea, dockWidget);

    BOBUIoolBar *toolBar = new BOBUIoolBar();

    toolBar->addWidget(new StaticWidget())->setVisible(true);

    toolBar->addWidget(new QSpinBox())->setVisible(true);
    mainWindow.addToolBar(toolBar);

    mainWindow.resize(600, 400);
    mainWindow.show();

    return app.exec();
}
