// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "chatdialog.h"

#include <BobUIWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ChatDialog dialog;
    dialog.show();
    return app.exec();
}
