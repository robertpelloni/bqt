// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
#include "mainwindow.h"

#include <QApplication>
#include <BobUIPlugin>

Q_IMPORT_PLUGIN(BasicToolsPlugin)

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
//! [0]
