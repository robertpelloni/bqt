// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIGui>
#include <BobUIUiTools>

#include "mywidget.h"

//! [0]
QWidget *loadCustomWidget(QWidget *parent)
{
    QUiLoader loader;
    QWidget *myWidget = nullptr;

    QStringList availableWidgets = loader.availableWidgets();

    if (availableWidgets.contains("AnalogClock"))
        myWidget = loader.createWidget("AnalogClock", parent);

    return myWidget;
}
//! [0]

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyWidget widget;
    widget.show();

    QWidget *customWidget = loadCustomWidget(0);
    customWidget->show();
    return app.exec();
}
