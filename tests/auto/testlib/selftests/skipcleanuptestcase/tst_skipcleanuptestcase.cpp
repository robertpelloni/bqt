// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

class tst_SkipCleanupTestCase: public QObject
{
Q_OBJECT
private slots:
    void aTestFunction() const;
    void cleanupTestCase() const;
};

void tst_SkipCleanupTestCase::aTestFunction() const
{
    QVERIFY(true);
}

void tst_SkipCleanupTestCase::cleanupTestCase() const
{
    QSKIP("Skip inside cleanupTestCase.");
}

BOBUIEST_APPLESS_MAIN(tst_SkipCleanupTestCase)
#include "tst_skipcleanuptestcase.moc"
