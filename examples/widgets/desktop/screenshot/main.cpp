// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QScreen>

#include "screenshot.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Screenshot screenshot;
    screenshot.move(screenshot.screen()->availableGeometry().topLeft() + QPoint(20, 20));
    screenshot.show();

    return app.exec();
}
