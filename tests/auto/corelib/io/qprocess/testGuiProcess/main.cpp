// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <BobUIWidgets/QApplication>
#include <BobUIWidgets/QLabel>
#include <stdio.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QLabel label("This process is just waiting to die");
    label.show();

    int c;
    Q_UNUSED(c);
    fgetc(stdin); // block until fed

    qDebug("Process is running");

    return 0;
}
