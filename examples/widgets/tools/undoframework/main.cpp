// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>

#include "mainwindow.h"

//! [0]
int main(int argv, char *args[])
{
    QApplication app(argv, args);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
//! [0]
