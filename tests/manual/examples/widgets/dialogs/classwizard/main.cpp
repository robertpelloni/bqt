// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>
#include <BOBUIranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "classwizard.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifndef BOBUI_NO_TRANSLATION
    QString translatorFileName = QLatin1String("bobuibase_");
    translatorFileName += QLocale::system().name();
    BOBUIranslator *translator = new BOBUIranslator(&app);
    if (translator->load(translatorFileName, QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
        app.installTranslator(translator);
#endif

    ClassWizard wizard;
    wizard.show();
    return app.exec();
}
