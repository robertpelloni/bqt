// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0
#include <QApplication>

#include "localewidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LocaleWidget wgt;
    wgt.show();
    return app.exec();
}
