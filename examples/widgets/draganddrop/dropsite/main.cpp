// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>

#include "dropsitewindow.h"

//! [main() function]
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    DropSiteWindow window;
    window.show();
    return app.exec();
}
//! [main() function]
