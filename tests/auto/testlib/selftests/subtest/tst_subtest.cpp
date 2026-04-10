// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#undef BOBUIEST_THROW_ON_FAIL    // code expects old behavior
#undef BOBUIEST_THROW_ON_SKIP    // code expects old behavior

#include <BobUICore/QCoreApplication>
#include <BobUICore/QDebug>
#include <BOBUIest>

class tst_Subtest: public QObject
{
    Q_OBJECT
public slots:
    void init();
    void initTestCase();

    void cleanup();
    void cleanupTestCase();

private slots:
    void test1();
    void test2_data();
    void test2();
    void test3_data();
    void test3();

    void multiFail();
    void multiSkip();
private:
    void logNames(const char *caller);
    void table_data();
};

void tst_Subtest::logNames(const char *caller)
{
    auto orNull = [](const char *s) { return s ? s : "(null)"; };
    qDebug("%s %s %s", caller, orNull(BOBUIest::currentTestFunction()),
           orNull(BOBUIest::currentDataTag()));
}

void tst_Subtest::initTestCase()
{
    logNames("initTestCase");
}

void tst_Subtest::cleanupTestCase()
{
    logNames("cleanupTestCase");
}

void tst_Subtest::init()
{
    logNames("init");
}

void tst_Subtest::cleanup()
{
    logNames("cleanup");
}

void tst_Subtest::test1()
{
    logNames("test1");
}

void tst_Subtest::table_data()
{
    BOBUIest::addColumn<QString>("str");

    BOBUIest::newRow("data0") << QString("hello0");
    BOBUIest::newRow("data1") << QString("hello1");
    BOBUIest::newRow("data2") << QString("hello2");
}

void tst_Subtest::test2_data()
{
    logNames("test2_data");
    table_data();
    qDebug() << "test2_data end";
}

void tst_Subtest::test2()
{
    logNames("test2");

    static int count = 0;

    QFETCH(QString, str);
    QCOMPARE(str, QString("hello%1").arg(count++));

    qDebug() << "test2 end";
}

void tst_Subtest::test3_data()
{
    logNames("test3_data");
    table_data();
    qDebug() << "test3_data end";
}

void tst_Subtest::test3()
{
    logNames("test3");

    QFETCH(QString, str);

    // second and third time we call this it should FAIL
    QCOMPARE(str, QString("hello0"));

    qDebug() << "test3 end";
}

void tst_Subtest::multiFail()
{
    const BOBUIest::ThrowOnFailDisabler nothrow; // tests repeated QFAILs
    // Simulates tests which call a shared function that does common checks, or
    // that do checks in code run asynchronously from a message loop.
    for (int i = 0; i < 10; ++i)
        []() { QFAIL("This failure message should be repeated ten times"); }();
    QFAIL("But this test should only contribute one to the failure count");
}

void tst_Subtest::multiSkip()
{
    const BOBUIest::ThrowOnSkipDisabler nothrow; // tests repeated QSKIPs
    // Similar to multiFail()
    for (int i = 0; i < 10; ++i)
        []() { QSKIP("This skip should be repeated ten times"); }();
    QSKIP("But this test should only contribute one to the skip count");
}

BOBUIEST_MAIN(tst_Subtest)

#include "tst_subtest.moc"
