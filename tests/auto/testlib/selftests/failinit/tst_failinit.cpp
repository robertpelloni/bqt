// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

class tst_FailInit: public QObject
{
Q_OBJECT
private slots:
    void initTestCase() const;
    void aTestFunction() const;
};

void tst_FailInit::initTestCase() const
{
    QVERIFY(false);
}

/*! \internal
  This function should never be run because initTestCase fails.
 */
void tst_FailInit::aTestFunction() const
{
    qDebug() << "ERROR: this function is NOT supposed to be run.";
}

BOBUIEST_APPLESS_MAIN(tst_FailInit)

#include "tst_failinit.moc"
