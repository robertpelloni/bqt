// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QApplication>
#include <QMainWindow>
#include "triviswidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow wnd;
    wnd.resize(1280, 800);
    wnd.setCentralWidget(new TriangulationVisualizer);
    wnd.show();

    return app.exec();
}
