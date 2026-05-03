// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QSurfaceFormat>
#include "mainwindow.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );

    QCoreApplication::setApplicationName("BobUI QOpenGLWidget Stereoscopic Rendering Example");
    QCoreApplication::setOrganizationName("BobUIProject");
    QCoreApplication::setApplicationVersion(BOBUI_VERSION_STR);

    //! [1]
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);

    // Enable stereoscopic rendering support
    format.setStereo(true);

    QSurfaceFormat::setDefaultFormat(format);
    //! [1]

    MainWindow mw;
    mw.resize(1280, 720);
    mw.show();
    return a.exec();
}
