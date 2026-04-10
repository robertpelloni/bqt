// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>

#include "fademessage.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    FadeMessage widget;
    widget.setWindowTitle(BOBUI_TRANSLATE_NOOP(QGraphicsView, "Popup Message with Effect"));
    widget.setFixedSize(400, 600);
    widget.show();

    return app.exec();
}
