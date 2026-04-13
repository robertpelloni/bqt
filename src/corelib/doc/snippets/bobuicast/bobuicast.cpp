// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QLabel>
#include <QPushButton>

#include "bobuicast.h"

MyWidget::MyWidget()
{
//! [0]
    QObject *obj = new MyWidget;
//! [0]

//! [1]
    QWidget *widget = qobject_cast<QWidget *>(obj);
//! [1]

//! [2]
    MyWidget *myWidget = qobject_cast<MyWidget *>(obj);
//! [2]

//! [3]
    QLabel *label = qobject_cast<QLabel *>(obj);
//! [3] //! [4]
    // label is nullptr
//! [4]

//! [5]
    if (QLabel *label = qobject_cast<QLabel *>(obj)) {
//! [5] //! [6]
        label->setText(tr("Ping"));
    } else if (QPushButton *button = qobject_cast<QPushButton *>(obj)) {
        button->setText(tr("Pong!"));
    }
//! [6]
}
