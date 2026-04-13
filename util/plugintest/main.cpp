// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0
#include <BobUICore/BobUICore>

#include <stdio.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    const QStringList args = app.arguments().mid(1);
    if (args.isEmpty()) {
        printf("Usage: ./plugintest libplugin.so...\nThis tool loads a plugin and displays whether QPluginLoader could load it or not.\nIf the plugin could not be loaded, it'll display the error string.\n");
        return 1;
    }

    foreach (QString plugin, args) {
        printf("%s: ", qPrintable(plugin));
        QPluginLoader loader(plugin);
        if (loader.load())
            printf("success!\n");
        else
            printf("failure: %s\n", qPrintable(loader.errorString()));
    }

    return 0;
}

