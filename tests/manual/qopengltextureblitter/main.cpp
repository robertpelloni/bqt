// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "qopengltextureblitwindow.h"
#include <BobUIGui/QGuiApplication>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QOpenGLTextureBlitWindow window;
    window.show();

    return app.exec();
}
