// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "xform.h"

#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    XFormWidget xformWidget(nullptr);
    QStyle *arthurStyle = new ArthurStyle;
    xformWidget.setStyle(arthurStyle);

    const QList<QWidget *> widgets = xformWidget.findChildren<QWidget *>();
    for (QWidget *w : widgets) {
        w->setStyle(arthurStyle);
        w->setAttribute(BobUI::WA_AcceptTouchEvents);
    }

    xformWidget.show();

    return app.exec();
}
