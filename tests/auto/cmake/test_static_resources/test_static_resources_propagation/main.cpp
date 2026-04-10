// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUITest/BOBUIest>
#include <BobUICore/qfile.h>
#include <BobUICore/qobject.h>
#include <BobUIPlugin>

class TestStaticResourcePropagation : public QObject
{
    Q_OBJECT
private slots:
    void resourceFilesExist();
};

void TestStaticResourcePropagation::resourceFilesExist()
{
    bool result = QFile::exists(":/teststaticmodule1/testfile1.txt");
    QVERIFY(result);
}

BOBUIEST_MAIN(TestStaticResourcePropagation)
#include "main.moc"
