// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "widget.h"
#include "ui_widget.h"

#include <BOBUIimer>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    BOBUIimer::singleShot(5000, this, SLOT(switchToFullScreen()));
    BOBUIimer::singleShot(10000, this, SLOT(addMenuBar()));
    BOBUIimer::singleShot(15000, this, SLOT(removeMenuBar()));
    BOBUIimer::singleShot(20000, this, SLOT(switchToNormalScreen()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Widget::switchToFullScreen()
{
    ui->label->setText("entering full screen");
    showFullScreen();
}

void Widget::switchToNormalScreen()
{
    ui->label->setText("leaving full screen");
    showNormal();
}

void Widget::addMenuBar()
{
    ui->label->setText("adding menu bar");
    menuBar = new QMenuBar(this);
    menuBar->setVisible(true);
}

void Widget::removeMenuBar()
{
    ui->label->setText("removing menu bar");
    delete menuBar;
}
