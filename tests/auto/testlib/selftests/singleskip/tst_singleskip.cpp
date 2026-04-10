// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/QCoreApplication>
#include <BOBUIest>

class tst_SingleSkip: public QObject
{
    Q_OBJECT

private slots:
    void myTest() const;
};

void tst_SingleSkip::myTest() const
{
    QSKIP("skipping test");
}

BOBUIEST_MAIN(tst_SingleSkip)

#include "tst_singleskip.moc"
