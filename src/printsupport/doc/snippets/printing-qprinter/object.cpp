// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "object.h"

#include <BobUIWidgets>
#include <BobUIPrintSupport/bobuiprintsupportglobal.h>
#if BOBUI_CONFIG(printdialog)
#include <QPrinter>
#endif

Object::Object(QObject *parent)
    : QObject(parent)
{
}

void Object::print()
{
    int numberOfPages = 10;
    int lastPage = numberOfPages - 1;

//! [0]
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName("print.ps");
    QPainter painter;
    painter.begin(&printer);

    for (int page = 0; page < numberOfPages; ++page) {

        // Use the painter to draw on the page.

        if (page != lastPage)
            printer.newPage();
    }

    painter.end();
//! [0]
    qApp->quit();
}
