// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/QCoreApplication>
#include <BOBUIest>

class tst_Skip: public QObject
{
    Q_OBJECT

private slots:
    void test_data();
    void test();

    void emptytest_data();
    void emptytest();

    void singleSkip_data();
    void singleSkip();
};


void tst_Skip::test_data()
{
    BOBUIest::addColumn<bool>("booll");
    BOBUIest::newRow("local 1") << false;
    BOBUIest::newRow("local 2") << true;

    QSKIP("skipping all");
}

void tst_Skip::test()
{
    QFAIL("this line should never be reached, since we skip in the _data function");
}

void tst_Skip::emptytest_data()
{
    QSKIP("skipping all");
}

void tst_Skip::emptytest()
{
    QFAIL("this line should never be reached, since we skip in the _data function");
}

void tst_Skip::singleSkip_data()
{
    BOBUIest::addColumn<bool>("booll");
    BOBUIest::newRow("local 1") << false;
    BOBUIest::newRow("local 2") << true;
}

void tst_Skip::singleSkip()
{
    QFETCH(bool, booll);
    if (!booll)
        QSKIP("skipping one");
    qDebug("this line should only be reached once (%s)", booll ? "true" : "false");
}

BOBUIEST_MAIN(tst_Skip)

#include "tst_skip.moc"
