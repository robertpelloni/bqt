// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

class tst_SkipBlacklisted : public QObject
{
    Q_OBJECT

private slots:
    void pass();
    void blacklisted();
    void blacklistedData();
    void blacklistedData_data();
};

void tst_SkipBlacklisted::pass()
{
    QVERIFY(true);
}

// This test have been blacklisted in skipblacklisted/BLACKLIST
void tst_SkipBlacklisted::blacklisted()
{
    QFAIL("this line should never be reached, since we skip all blacklisted test functions");
}

// blacklisted 1 and blacklisted 2 have been blacklisted in skipblacklisted/BLACKLIST
void tst_SkipBlacklisted::blacklistedData()
{
    QFETCH(int, testdata);
    QCOMPARE(testdata, 2);
}

void tst_SkipBlacklisted::blacklistedData_data()
{
    BOBUIest::addColumn<int>("testdata");

    BOBUIest::newRow("blacklisted 1") << 1;
    BOBUIest::newRow("should pass") << 2;
    BOBUIest::newRow("blacklisted 2") << 3;
}

BOBUIEST_MAIN_WRAPPER(tst_SkipBlacklisted,
    std::vector<const char*> args(argv, argv + argc);
    args.push_back("-skipblacklisted");
    argc = int(args.size());
    argv = const_cast<char**>(&args[0]);
    BOBUIEST_MAIN_SETUP())

#include "tst_skipblacklisted.moc"
