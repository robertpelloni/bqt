// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/QCoreApplication>
#include <BOBUIest>

/*!
 \internal
 \since 4.4
 \brief Tests that reporting of tables are done in a certain way.
 */
class tst_DataTable: public QObject
{
    Q_OBJECT

private slots:

    void fiveTablePasses() const;
    void fiveTablePasses_data() const;
};

void tst_DataTable::fiveTablePasses() const
{
    QFETCH(bool, test);

    QVERIFY(test);
}

void tst_DataTable::fiveTablePasses_data() const
{
    BOBUIest::addColumn<bool>("test");

    BOBUIest::newRow("fiveTablePasses_data1") << true;
    BOBUIest::newRow("fiveTablePasses_data2") << true;
    BOBUIest::newRow("fiveTablePasses_data3") << true;
    BOBUIest::newRow("fiveTablePasses_data4") << true;
    BOBUIest::newRow("fiveTablePasses_data5") << true;
}

BOBUIEST_MAIN_WRAPPER(tst_DataTable,
    std::vector<const char*> args(argv, argv + argc);
    args.push_back("fiveTablePasses");
    args.push_back("fiveTablePasses:fiveTablePasses_data1");
    args.push_back("-v2");
    argc = int(args.size());
    argv = const_cast<char**>(&args[0]);
    BOBUIEST_MAIN_SETUP())

#include "tst_commandlinedata.moc"
