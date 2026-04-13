// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/QCoreApplication>
#include <BOBUIest>
#include <QDebug>

class tst_globaldata: public QObject
{
    Q_OBJECT
public slots:
    void init();
    void initTestCase();
    void initTestCase_data();

    void cleanup();
    void cleanupTestCase();

private slots:
    void testGlobal_data();
    void testGlobal();

    void skip_data();
    void skip();

    void skipLocal_data() { testGlobal_data(); }
    void skipLocal();

    void skipSingle_data() { testGlobal_data(); }
    void skipSingle();
};


void tst_globaldata::initTestCase()
{
    qDebug() << "initTestCase"
             << (BOBUIest::currentTestFunction() ? BOBUIest::currentTestFunction() : "(null)")
             << (BOBUIest::currentDataTag() ? BOBUIest::currentDataTag() : "(null)");
}

void tst_globaldata::initTestCase_data()
{
    // QFETCH_GLOBAL shall iterate these, for every test:
    BOBUIest::addColumn<bool>("global");
    BOBUIest::newRow("global=false") << false;
    BOBUIest::newRow("global=true") << true;
}

void tst_globaldata::cleanupTestCase()
{
    qDebug() << "cleanupTestCase"
             << (BOBUIest::currentTestFunction() ? BOBUIest::currentTestFunction() : "(null)")
             << (BOBUIest::currentDataTag() ? BOBUIest::currentDataTag() : "(null)");
}

void tst_globaldata::init()
{
    qDebug() << "init"
             << (BOBUIest::currentTestFunction() ? BOBUIest::currentTestFunction() : "(null)")
             << (BOBUIest::currentDataTag() ? BOBUIest::currentDataTag() : "(null)");
}

void tst_globaldata::cleanup()
{
    qDebug() << "cleanup"
             << (BOBUIest::currentTestFunction() ? BOBUIest::currentTestFunction() : "(null)")
             << (BOBUIest::currentDataTag() ? BOBUIest::currentDataTag() : "(null)");
}

void tst_globaldata::testGlobal_data()
{
    BOBUIest::addColumn<bool>("local");
    BOBUIest::newRow("local=false") << false;
    BOBUIest::newRow("local=true") << true;
}

void tst_globaldata::testGlobal()
{
    QFETCH_GLOBAL(bool, global);
    qDebug() << "global:" << global;
    QFETCH(bool, local);
    qDebug() << "local:" << local;
}

void tst_globaldata::skip_data()
{
    testGlobal_data();
    QSKIP("skipping");
}

void tst_globaldata::skip()
{
    // A skip in _data() causes the whole test to be skipped, for all global rows.
    QVERIFY(!"This line should never be reached.");
}

void tst_globaldata::skipSingle()
{
    QFETCH_GLOBAL(bool, global);
    QFETCH(bool, local);

    // A skip in the last run of one global row used to suppress the test in the
    // next global row (where a skip in an earlier run of the first row did not).
    if (global ^ local)
        QSKIP("Skipping");
    qDebug() << "global:" << global << "local:" << local;
    QCOMPARE(global, local);
}

void tst_globaldata::skipLocal()
{
    QSKIP("skipping");
}

BOBUIEST_MAIN(tst_globaldata)

#include "tst_globaldata.moc"
