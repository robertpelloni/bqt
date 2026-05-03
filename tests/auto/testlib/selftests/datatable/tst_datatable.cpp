// Copyright (C) 2016 The BobUI Company Ltd.
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
    void singleTestFunction1() const;
    void singleTestFunction2() const;

    void fiveTablePasses() const;
    void fiveTablePasses_data() const;
    void fiveTableFailures() const;
    void fiveTableFailures_data() const;

    void startsWithFailure() const;
    void startsWithFailure_data() const;

    void endsWithFailure() const;
    void endsWithFailure_data() const;

    void failureInMiddle() const;
    void failureInMiddle_data() const;

    void fiveIsolatedFailures() const;
    void fiveIsolatedFailures_data() const;
};

void tst_DataTable::singleTestFunction1() const
{
    /* Do nothing, just pass. */
}

void tst_DataTable::singleTestFunction2() const
{
    /* Do nothing, just pass. */
}

void tst_DataTable::fiveTableFailures() const
{
    QFETCH(bool, test);

    QVERIFY(test);
}

void tst_DataTable::fiveTableFailures_data() const
{
    BOBUIest::addColumn<bool>("test");

    /* Unconditionally fail. */
    BOBUIest::newRow("fiveTableFailures_data 1") << false;
    BOBUIest::newRow("fiveTableFailures_data 2") << false;
    BOBUIest::newRow("fiveTableFailures_data 3") << false;
    BOBUIest::newRow("fiveTableFailures_data 4") << false;
    BOBUIest::newRow("fiveTableFailures_data 5") << false;
}

void tst_DataTable::startsWithFailure() const
{
    fiveTableFailures();
}

void tst_DataTable::fiveTablePasses() const
{
    fiveTableFailures();
}

void tst_DataTable::fiveTablePasses_data() const
{
    BOBUIest::addColumn<bool>("test");

    BOBUIest::newRow("fiveTablePasses_data 1") << true;
    BOBUIest::newRow("fiveTablePasses_data 2") << true;
    BOBUIest::newRow("fiveTablePasses_data 3") << true;
    BOBUIest::newRow("fiveTablePasses_data 4") << true;
    BOBUIest::newRow("fiveTablePasses_data 5") << true;
}

void tst_DataTable::startsWithFailure_data() const
{
    BOBUIest::addColumn<bool>("test");

    BOBUIest::newRow("startsWithFailure_data 1") << false;
    BOBUIest::newRow("startsWithFailure_data 2") << true;
    BOBUIest::newRow("startsWithFailure_data 3") << true;
    BOBUIest::newRow("startsWithFailure_data 4") << true;
    BOBUIest::newRow("startsWithFailure_data 5") << true;
}

void tst_DataTable::endsWithFailure() const
{
    fiveTableFailures();
}

void tst_DataTable::endsWithFailure_data() const
{
    BOBUIest::addColumn<bool>("test");

    BOBUIest::newRow("endsWithFailure 1") << true;
    BOBUIest::newRow("endsWithFailure 2") << true;
    BOBUIest::newRow("endsWithFailure 3") << true;
    BOBUIest::newRow("endsWithFailure 4") << true;
    BOBUIest::newRow("endsWithFailure 5") << false;
}

void tst_DataTable::failureInMiddle() const
{
    fiveTableFailures();
}

void tst_DataTable::failureInMiddle_data() const
{
    BOBUIest::addColumn<bool>("test");

    BOBUIest::newRow("failureInMiddle_data 1") << true;
    BOBUIest::newRow("failureInMiddle_data 2") << true;
    BOBUIest::newRow("failureInMiddle_data 3") << false;
    BOBUIest::newRow("failureInMiddle_data 4") << true;
    BOBUIest::newRow("failureInMiddle_data 5") << true;
}

void tst_DataTable::fiveIsolatedFailures() const
{
    QFETCH(bool, test);
    QVERIFY(!test);
}

void tst_DataTable::fiveIsolatedFailures_data() const
{
    BOBUIest::addColumn<bool>("test");

    BOBUIest::newRow("fiveIsolatedFailures_data 1") << true;
    BOBUIest::newRow("fiveIsolatedFailures_data 2") << true;
    BOBUIest::newRow("fiveIsolatedFailures_data 3") << true;
    BOBUIest::newRow("fiveIsolatedFailures_data 4") << true;
    BOBUIest::newRow("fiveIsolatedFailures_data 5") << true;
}

BOBUIEST_MAIN(tst_DataTable)

#include "tst_datatable.moc"
