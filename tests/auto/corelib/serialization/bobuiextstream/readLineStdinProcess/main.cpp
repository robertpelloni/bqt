// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/QCoreApplication>
#include <BobUICore/BOBUIextStream>
#include <BobUICore/QString>


int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);
    BOBUIextStream qin(stdin);
    BOBUIextStream qerr(stderr);
    QString line;
    do {
        line = qin.readLine();
        if (!line.isNull())
            qerr << line << BobUI::flush;
    } while (!line.isNull());
    return 0;
}
