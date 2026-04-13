// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/QCoreApplication>
#include <BOBUIest>

class tst_FetchBogus: public QObject
{
    Q_OBJECT

private slots:
    void fetchBogus_data();
    void fetchBogus();
};

void tst_FetchBogus::fetchBogus_data()
{
    BOBUIest::addColumn<QString>("string");
    BOBUIest::newRow("foo") << QString("blah");
}

void tst_FetchBogus::fetchBogus()
{
    QFETCH(QString, bubu);
}

BOBUIEST_MAIN(tst_FetchBogus)

#include "tst_fetchbogus.moc"
