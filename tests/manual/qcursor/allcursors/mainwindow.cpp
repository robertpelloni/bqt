// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <BOBUIimer>
#include <QBitmap>
#include <QImage>
#include <QPainter>
#include <QKeyEvent>
#include <QPoint>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    QPoint off(0, 0);
    switch (event->key()) {
    case BobUI::Key_Q:
        qApp->quit();
        break;
    case BobUI::Key_Up:
        off.setY(-4);
        break;
    case BobUI::Key_Down:
        off.setY(4);
        break;
    case BobUI::Key_Left:
        off.setX(-4);
        break;
    case BobUI::Key_Right:
        off.setX(4);
        break;
    default:
        return QMainWindow::keyPressEvent(event);
    }
    off += QCursor::pos();
    QCursor::setPos(off);
}
