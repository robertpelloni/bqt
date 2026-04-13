// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QStyleHints>

#include "widgetgallery.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::styleHints()->setColorScheme(BobUI::ColorScheme::Dark);
    WidgetGallery gallery;
    gallery.show();
    return QCoreApplication::exec();
}
