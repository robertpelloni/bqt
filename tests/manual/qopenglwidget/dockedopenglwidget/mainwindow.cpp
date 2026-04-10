// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwidget.h"
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWidget *w1 = new MainWidget();
    centralWidget()->layout()->addWidget(w1);

    MainWidget *w2 = new MainWidget();
    QDockWidget *dock = new QDockWidget("OpenGL Dock", this);
    w2->setFixedSize(300, 300);
    dock->setWidget(w2);
    dock->setFixedSize(300, 300);

    addDockWidget(BobUI::RightDockWidgetArea, dock);
    dock->setFloating(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
