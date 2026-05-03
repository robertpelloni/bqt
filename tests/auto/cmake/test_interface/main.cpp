// Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Stephen Kelly <stephen.kelly@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QApplication>

#include "mainwindow.h"

#if !defined(BOBUI_CORE_LIB) || !defined(BOBUI_GUI_LIB) || !defined(BOBUI_WIDGETS_LIB)
#error Missing module definitions
#endif

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWindow mw;
    mw.show();

    return app.exec();
}
