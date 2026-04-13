// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <QFile>
#include <QDebug>

int main(int, char **)
{
    QFile file(":/test.txt");
    if (!file.open(QFile::ReadOnly))
        return 1;

    QString data = QString::fromUtf8(file.readAll());
    qDebug() << data;
    return 0;
}
