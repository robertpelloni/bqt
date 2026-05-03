// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

class tst_SkipCleanup: public QObject
{
Q_OBJECT
private slots:
    void aTestFunction() const;
    void cleanup() const;
};

void tst_SkipCleanup::aTestFunction() const
{
    QVERIFY(true);
}

void tst_SkipCleanup::cleanup() const
{
    QSKIP("Skip inside cleanup.");
}

BOBUIEST_APPLESS_MAIN(tst_SkipCleanup)
#include "tst_skipcleanup.moc"
