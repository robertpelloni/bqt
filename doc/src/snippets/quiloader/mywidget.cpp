// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIGui>
#include <BobUIUiTools>

#include "mywidget.h"

//! [0]
MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
{
    QUiLoader loader;
    QFile file(":/forms/myform.ui");
    file.open(QFile::ReadOnly);
    QWidget *myWidget = loader.load(&file, this);
    file.close();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(myWidget);
    setLayout(layout);
}
//! [0]
