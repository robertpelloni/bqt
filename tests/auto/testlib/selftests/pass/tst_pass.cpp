// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/QCoreApplication>
#include <BOBUIest>

class tst_Pass: public QObject
{
    Q_OBJECT

private slots:
    void testNumber1() const;
    void testNumber2() const;
    void testNumber3() const;
};

void tst_Pass::testNumber1() const
{
}

void tst_Pass::testNumber2() const
{
}

void tst_Pass::testNumber3() const
{
}

BOBUIEST_MAIN(tst_Pass)

#include "tst_pass.moc"
