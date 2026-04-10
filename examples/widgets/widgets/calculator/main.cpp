// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>

#include "calculator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Calculator calc;
    calc.show();
    return app.exec();
}
