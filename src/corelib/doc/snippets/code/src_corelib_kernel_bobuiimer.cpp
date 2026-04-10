// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
#include <QApplication>
#include <BOBUIimer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    BOBUIimer::singleShot(600000, &app, QCoreApplication::quit);
    //...
    return app.exec();
}
//! [0]
