// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

class tst_SkipInit: public QObject
{
Q_OBJECT
private slots:
    void initTestCase() const;
    void aTestFunction() const;
};

void tst_SkipInit::initTestCase() const
{
    QSKIP("Skip inside initTestCase. This should skip all tests in the class.");
}

/*! \internal
  This function should never be run because initTestCase fails.
 */
void tst_SkipInit::aTestFunction() const
{
    QFAIL("ERROR: This function is NOT supposed to be run.");
}

BOBUIEST_APPLESS_MAIN(tst_SkipInit)

#include "tst_skipinit.moc"
