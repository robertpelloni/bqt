// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>
#include "widget.h"

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        if (!qstrcmp(argv[i], "-g"))
            QCoreApplication::setAttribute(BobUI::AA_UseOpenGLES);
        else if (!qstrcmp(argv[i], "-s"))
            QCoreApplication::setAttribute(BobUI::AA_UseSoftwareOpenGL);
        else if (!qstrcmp(argv[i], "-d"))
            QCoreApplication::setAttribute(BobUI::AA_UseDesktopOpenGL);
    }

    QApplication app(argc, argv);

    Widget w;
    w.resize(700, 800);
    w.show();

    return app.exec();
}
