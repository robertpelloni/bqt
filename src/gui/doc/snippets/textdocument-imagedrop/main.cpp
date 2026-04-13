// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include "textedit.h"
#include <QApplication>

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    TextEdit textEdit;
    textEdit.show();

    return app.exec();
}
