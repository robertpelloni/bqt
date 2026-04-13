// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

class tst_FailCleanup: public QObject
{
Q_OBJECT
private slots:
    void aTestFunction() const;
    void cleanup() const;
};

void tst_FailCleanup::aTestFunction() const
{
    QVERIFY(true);
}

void tst_FailCleanup::cleanup() const
{
    QVERIFY2(false, "Fail inside cleanup");
}

BOBUIEST_APPLESS_MAIN(tst_FailCleanup)
#include "tst_failcleanup.moc"
