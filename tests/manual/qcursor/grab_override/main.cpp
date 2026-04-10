// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showFullScreen();
#ifdef BOBUI_KEYPAD_NAVIGATION
    QApplication::setNavigationMode(BobUI::NavigationModeCursorForceVisible);
#endif
    return a.exec();
}
