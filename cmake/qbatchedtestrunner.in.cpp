// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QList>
#include <QString>
#include <BobUITest/private/bobuiestcase_p.h>

int main(int argc, char **argv)
{
    if (argc == 1) {
        printf("%s\n", BOBUIest::qGetTestCaseNames().join(
            QStringLiteral(" ")).toStdString().c_str());
        return 0;
    }

    const auto entryFunction = BOBUIest::qGetTestCaseEntryFunction(QString::fromUtf8(argv[1]));
    return entryFunction ? entryFunction(argc - 1, argv + 1) : -1;
}
