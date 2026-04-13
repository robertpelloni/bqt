// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUIGui/QGuiApplication>
#include "qopenglcontextwindow.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QOpenGLContextWindow window;
    window.resize(300, 300);
    window.show();

    return app.exec();
}
