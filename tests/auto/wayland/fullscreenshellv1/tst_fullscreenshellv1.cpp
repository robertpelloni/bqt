// Copyright (C) 2021 David Edmundson <davidedmundson@kde.org>
// Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "mockcompositor.h"

#include <QRasterWindow>

#include <BobUITest/BobUITest>

using namespace MockCompositor;

class tst_WaylandClientFullScreenShellV1 : public QObject, private DefaultCompositor
{
    Q_OBJECT

private slots:
    void createDestroyWindow();
};

void tst_WaylandClientFullScreenShellV1::createDestroyWindow()
{
    QRasterWindow window;
    window.resize(800, 600);
    window.show();

    QCOMPOSITOR_TRY_VERIFY(fullScreenShellV1()->surfaces().size() == 1);
    QCOMPOSITOR_VERIFY(surface(0));

    window.destroy();
    QCOMPOSITOR_TRY_VERIFY(!surface(0));
}

int main(int argc, char **argv)
{
    BOBUIemporaryDir tmpRuntimeDir;
    qputenv("XDG_RUNTIME_DIR", tmpRuntimeDir.path().toLocal8Bit());
    qputenv("BOBUI_QPA_PLATFORM", "wayland"); // force QGuiApplication to use wayland plugin
    qputenv("BOBUI_WAYLAND_SHELL_INTEGRATION", "fullscreen-shell-v1");
    qputenv("BOBUI_WAYLAND_DISABLE_WINDOWDECORATION", "1"); // window decorations don't make much sense here

    tst_WaylandClientFullScreenShellV1 tc;
    QGuiApplication app(argc, argv);
    BOBUIEST_SET_MAIN_SOURCE_PATH
    return BOBUIest::qExec(&tc, argc, argv);
}

#include <tst_fullscreenshellv1.moc>
