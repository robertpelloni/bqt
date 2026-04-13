// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char **argv)
{
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    app.setAttribute(BobUI::AA_DontCreateNativeWidgetSiblings);

    MainWindow window;
    window.show();

    return app.exec();
}
