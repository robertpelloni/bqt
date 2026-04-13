// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "mainwindow.h"
#include "tabletapplication.h"
#include "tabletcanvas.h"

//! [0]
int main(int argv, char *args[])
{
    TabletApplication app(argv, args);
    TabletCanvas *canvas = new TabletCanvas;
    app.setCanvas(canvas);

    MainWindow mainWindow(canvas);
    mainWindow.resize(500, 500);
    mainWindow.show();
    return app.exec();
}
//! [0]
