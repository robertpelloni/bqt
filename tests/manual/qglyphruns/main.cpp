// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "controller.h"

#include <BobUIWidgets>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Controller controller;
    controller.showMaximized();

    return app.exec();
}

#include "main.moc"
