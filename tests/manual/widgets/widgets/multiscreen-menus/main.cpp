// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(BobUI::AA_DontUseNativeMenuBar);

    MainWindow w;
    w.show();

    return a.exec();
}
