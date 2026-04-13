// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QCoreApplication>
#include <BobUITest/BOBUIest>
#include <QDebug>

class TestClass : public QObject
{
    Q_OBJECT
public:
    TestClass(QObject *parent = nullptr) { }

private slots:
    void doTest();
};

void TestClass::doTest()
{
    QFile fsrc(QFINDTESTDATA("data/testdata.txt"));
    QVERIFY(fsrc.open(QFile::ReadOnly));
    QCOMPARE(fsrc.readAll().trimmed(),
             QByteArrayLiteral("This is the test data found in BOBUI_TESTCASE_SOURCEDIR."));

    QFile fbuild(QFINDTESTDATA("level2/testdata_build.txt"));
    QVERIFY(fbuild.open(QFile::ReadOnly));
    QCOMPARE(fbuild.readAll().trimmed(),
             QByteArrayLiteral("This is the test data found in custom BOBUI_TESTCASE_BUILDDIR."));
}

BOBUIEST_MAIN(TestClass)
#include "main.moc"
