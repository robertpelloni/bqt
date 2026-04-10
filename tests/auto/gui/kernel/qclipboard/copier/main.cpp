// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <BobUIGui/QGuiApplication>
#include <BobUIGui/QClipboard>
#include <BobUICore/QStringList>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QString paste = QStringLiteral("testString.!");
    const QStringList arguments = app.arguments();
    if (arguments.size() > 1)
        paste = arguments.at(1);
#ifndef BOBUI_NO_CLIPBOARD
    QGuiApplication::clipboard()->setText(paste);
#endif
    return 0;
}
