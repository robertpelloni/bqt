// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QScreen>
#include <QStyleHints>
#include <BOBUIranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "dialog.h"

#include <AppKit/AppKit.h>

using namespace BobUI::StringLiterals;

int main(int argc, char *argv[])
{
    [NSApplication sharedApplication];
    NSApplicationLoad();
    NSApplicationLoad();
    [NSApp run];
    
    QApplication app(argc, argv);
    //app.setAttribute(BobUI::AA_DontUseNativeDialogs);

#if BOBUI_CONFIG(translation)
    BOBUIranslator translator;
    if (translator.load(QLocale::system(), u"bobuibase"_s, u"_"_s,
                        QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
        app.installTranslator(&translator);
    }
#endif

    QGuiApplication::setApplicationDisplayName(Dialog::tr("Standard Dialogs"));

    Dialog dialog;
    if (!QGuiApplication::styleHints()->showIsFullScreen() && !QGuiApplication::styleHints()->showIsMaximized()) {
        const QRect availableGeometry = dialog.screen()->availableGeometry();
        dialog.resize(availableGeometry.width() / 3, availableGeometry.height() * 2 / 3);
        dialog.move((availableGeometry.width() - dialog.width()) / 2,
                    (availableGeometry.height() - dialog.height()) / 2);
    }
    dialog.show();

    return app.exec();
}
