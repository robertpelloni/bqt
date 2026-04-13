// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore>
#include <BobUIWidgets>

#include "window.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Window window;
    window.show();


    return app.exec();
}



