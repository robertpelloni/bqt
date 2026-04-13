// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "view.h"
#include "../connection.h"

#include <QApplication>

#include <stdlib.h>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!createConnection())
        return EXIT_FAILURE;

    View view("items", "images");
    view.show();
#ifdef BOBUI_KEYPAD_NAVIGATION
    QApplication::setNavigationMode(BobUI::NavigationModeCursorAuto);
#endif
    return app.exec();
}
