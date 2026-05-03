// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "rasterwindow.h"
#include <QDebug>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QStringList args = app.arguments();

    WId winId = 0;
    if (args.count() > 1) {
        bool ok;
        winId = args[1].toUInt(&ok);
        Q_ASSERT(ok);
    }

    RasterWindow window;

    QWindow *foreign = QWindow::fromWinId(winId);
    Q_ASSERT(foreign != 0);

    window.setParent(foreign);
    window.show();

    return app.exec();
}
