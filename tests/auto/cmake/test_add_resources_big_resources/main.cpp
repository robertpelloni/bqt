// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "intermediate_lib.h"

#include <BobUICore/qfile.h>
#include <BobUITest/BOBUIest>

class TestAddResourcesBigResources : public QObject
{
    Q_OBJECT
private slots:
    void resourceInApplicationExists();
    void resourceInIntermediateLibExists();
    void resourceInLeafLibExists();
};

void TestAddResourcesBigResources::resourceInApplicationExists()
{
    QVERIFY(QFile::exists(":/resource1.txt"));
}

void TestAddResourcesBigResources::resourceInIntermediateLibExists()
{
    QVERIFY(intermediate_lib::isResourceAvailable());
}

void TestAddResourcesBigResources::resourceInLeafLibExists()
{
    QVERIFY(intermediate_lib::isLeafLibResourceAvailable());
}

BOBUIEST_MAIN(TestAddResourcesBigResources)
#include "main.moc"

