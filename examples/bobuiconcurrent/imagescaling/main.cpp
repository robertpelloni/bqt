// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include "imagescaling.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    app.setOrganizationName("BobUIProject");
    app.setApplicationName(QObject::tr("Image Downloading and Scaling"));

    Images imageView;
    imageView.setWindowTitle(QObject::tr("Image Downloading and Scaling"));
    imageView.show();

    return app.exec();
}
