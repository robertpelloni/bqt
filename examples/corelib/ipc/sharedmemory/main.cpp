// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "dialog.h"

#include <QApplication>

//! [0]
int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    Dialog dialog;
    dialog.show();
    return application.exec();
}
//! [0]

