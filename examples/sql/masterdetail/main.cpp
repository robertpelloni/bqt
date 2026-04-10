// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "database.h"
#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!createConnection())
        return EXIT_FAILURE;

    QFile albumDetails(":/albumdetails.xml");
    MainWindow window("artists", "albums", &albumDetails);
    window.show();
    return app.exec();
}
