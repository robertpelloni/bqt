// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>
#include "window.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Window w;

    w.resize(400, 400);
    w.show();

    return app.exec();
}
