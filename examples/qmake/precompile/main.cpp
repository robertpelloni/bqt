// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include "myobject.h"
#include "mydialog.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MyObject obj;
    MyDialog dialog;

    QObject::connect(dialog.aButton, &QPushButton::clicked,
                     &dialog, &MyDialog::close);
    dialog.show();

    return app.exec();
}
//! [0]
