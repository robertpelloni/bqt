// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>

#include "analogclock.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    AnalogClock clock;
    clock.show();
    return app.exec();
}
