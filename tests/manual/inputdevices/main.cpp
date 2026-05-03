// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QApplication>
#include <QLoggingCategory>
#include <BOBUIreeView>

#include "inputdevicemodel.h"

int main(int argc, char **argv)
{
    QLoggingCategory::setFilterRules(QStringLiteral("bobui.qpa.input.devices=true"));

    QApplication app(argc, argv);

    BOBUIreeView view;
    view.setModel(new InputDeviceModel(&view));
    view.resize(1280, 600);
    view.show();
    view.resizeColumnToContents(0);

    app.exec();
}

