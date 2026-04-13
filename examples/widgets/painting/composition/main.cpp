// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "composition.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QScopedPointer<QStyle> arthurStyle(new ArthurStyle());
    CompositionWidget compWidget(nullptr);
    compWidget.setStyle(arthurStyle.data());

    const QList<QWidget *> widgets = compWidget.findChildren<QWidget *>();
    for (QWidget *w : widgets)
        w->setStyle(arthurStyle.data());
    compWidget.show();

    return app.exec();
}
