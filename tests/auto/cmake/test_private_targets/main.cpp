// Copyright (C) 2016 Stephen Kelly <steveire@gmail.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QGuiApplication>

#include <private/qwindow_p.h>
#include <private/qobject_p.h>
#include <BobUIGui/private/qwindow_p.h>
#include <BobUICore/private/qobject_p.h>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QWindow window;

    auto windowPrivate = QWindowPrivate::get(&window);

    if (windowPrivate->visible)
        return 1;

    auto objectPrivate = QObjectPrivate::get(&window);

    auto mo = window.metaObject();

    // Should be 0
    return objectPrivate->signalIndex("destroyed()", &mo);
}
