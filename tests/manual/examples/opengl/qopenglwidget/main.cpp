// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QMainWindow>
#include <QColorSpace>
#include <QSurfaceFormat>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "mainwindow.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );

    QCoreApplication::setApplicationName("BobUI QOpenGLWidget Example");
    QCoreApplication::setOrganizationName("BobUIProject");
    QCoreApplication::setApplicationVersion(BOBUI_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption multipleSampleOption("multisample", "Multisampling");
    parser.addOption(multipleSampleOption);
    QCommandLineOption srgbOption("srgb", "Use sRGB Color Space");
    parser.addOption(srgbOption);
    parser.process(a);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    if (parser.isSet(srgbOption))
        format.setColorSpace(QColorSpace::SRgb);
    if (parser.isSet(multipleSampleOption))
        format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow mw;
    mw.resize(1280, 720);
    mw.show();
    return a.exec();
}
