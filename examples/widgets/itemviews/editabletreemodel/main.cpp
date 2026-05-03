// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "mainwindow.h"

#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    const auto screenSize = window.screen()->availableSize();
    window.resize({screenSize.width() / 2, screenSize.height() * 2 / 3});
    window.show();
    return QCoreApplication::exec();
}
