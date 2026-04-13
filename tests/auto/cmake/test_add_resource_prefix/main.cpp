// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUITest/bobuiest.h>
#include <BobUICore/qfile.h>

class TestAddResourcePrefix : public QObject
{
    Q_OBJECT
private slots:
    void resourceInDefaultPathExists();
    void resourceInGivenPathExists();
    void xmlEscaping();
};

void TestAddResourcePrefix::resourceInDefaultPathExists()
{
    QVERIFY(QFile::exists(":/resource_file.txt"));
}

void TestAddResourcePrefix::resourceInGivenPathExists()
{
    QVERIFY(QFile::exists(":/resources/resource_file.txt"));
}


void TestAddResourcePrefix::xmlEscaping()
{
    QVERIFY(QFile::exists(":/&\"'<>/&\"'<>.alias"));
}

BOBUIEST_MAIN(TestAddResourcePrefix)
#include "main.moc"
