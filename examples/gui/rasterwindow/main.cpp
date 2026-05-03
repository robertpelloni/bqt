// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "rasterwindow.h"

//! [1]
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    RasterWindow window;
    window.show();

    return app.exec();
}
//! [1]
