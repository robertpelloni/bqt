// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>


#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Controller controller;
    controller.show();
    return app.exec();
}
