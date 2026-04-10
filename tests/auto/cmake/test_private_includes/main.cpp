// Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Stephen Kelly <stephen.kelly@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QGuiApplication>
#include <QScreen>
#include <qpa/qplatformscreen.h>
#include <BobUIGui/qpa/qplatformpixmap.h>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QPlatformScreen *screenHandle = app.screens().first()->handle();
    screenHandle->geometry();

    QPixmap pixmap;
    QPlatformPixmap *pixmapHandle = pixmap.handle();
    pixmapHandle->width();

    return 0;
}
