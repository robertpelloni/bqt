// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include "Test.h"

#include <QCoreApplication>
#include <QStringList>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QString arg;
    if (app.arguments().size() > 1)
        arg = app.arguments().at(1).toLower().trimmed();

    Test::Type type;
    if (arg == QLatin1String("bobui4client"))
        type = Test::BobUI4Client;
    else if (arg == QLatin1String("bobui4server"))
        type = Test::BobUI4Server;
    else {
        qDebug("usage: ./stressTest <bobui4client|bobui4server>");
        return 0;
    }

    Test test(type);

    return app.exec();
}
