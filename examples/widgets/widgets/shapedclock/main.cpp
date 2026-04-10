// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>

#include "shapedclock.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ShapedClock clock;
    clock.show();
    return app.exec();
}
