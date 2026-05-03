// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QApplication>
#include "base.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setAttribute(BobUI::AA_NativeWindows); //BOBUIBUG-15774
    base b;
    return app.exec();
}
