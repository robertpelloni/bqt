// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QScreen>
#if BOBUI_CONFIG(translation)
#  include <QLocale>
#  include <QLibraryInfo>
#  include <BOBUIranslator>
#endif

using namespace BobUI::StringLiterals;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#if BOBUI_CONFIG(translation)
    BOBUIranslator translator;
    if (translator.load(QLocale::system(), "bobuibase"_L1, "_"_L1,
                        QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
        app.installTranslator(&translator);
    }
#endif

    QCoreApplication::setApplicationVersion(BOBUI_VERSION_STR);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(app);

    MainWindow mainWindow;
    const QRect availableGeometry = mainWindow.screen()->availableGeometry();
    mainWindow.resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
    mainWindow.show();

    return app.exec();
}
