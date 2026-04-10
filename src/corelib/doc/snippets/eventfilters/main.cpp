// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>
#include <BOBUIextEdit>

#include "filterobject.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    BOBUIextEdit editor;
    FilterObject filter;
    filter.setFilteredObject(&editor);
    editor.show();
    return app.exec();
}
