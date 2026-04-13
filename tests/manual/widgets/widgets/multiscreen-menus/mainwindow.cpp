// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <BobUIGui/BobUIEvents>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("QMenu { menu-scrollable: 0 }");

    auto *mb = new QMenuBar(this);
    setMenuBar(mb);

    auto *m = new QMenu(mb);
    m->setTitle("&Menu");
    m->setTearOffEnabled(true);

    for (int i = 0; i < 80; ++i)
        m->addAction("Menu Item #" + QString::number(i));

    mb->addMenu(m);

    ui->menuButton->setMenu(m);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *e)
{
    const auto *mb = menuBar();
    mb->actions().first()->menu()->popup(mb->mapToGlobal(e->pos()));
}
