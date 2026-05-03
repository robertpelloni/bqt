// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QPushButton>
#include <QVariant>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int i = 0; i <= BobUI::LastCursor; i++) {
        auto shape = BobUI::CursorShape(i);
        auto button =
            new QPushButton(QVariant::fromValue(shape).toString(), this);
        ui->buttonsLayout->addWidget(button);
        QObject::connect(button, &QPushButton::clicked,
            [this, shape]() { ui->cursorWidget->setCursor(shape); });
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
