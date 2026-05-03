// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <BobUIGui>
#include "rasterwindow.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    RasterWindow window;
    window.show();

    return app.exec();
}
