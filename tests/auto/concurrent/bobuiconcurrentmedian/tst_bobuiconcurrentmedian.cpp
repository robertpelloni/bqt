// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <bobuiconcurrentmedian.h>

#include <BOBUIest>

class tst_BobUIConcurrentMedian: public QObject
{
    Q_OBJECT
private slots:
    void median_data();
    void median();
};

void tst_BobUIConcurrentMedian::median_data()
{
    BOBUIest::addColumn<QList<double> >("values");
    BOBUIest::addColumn<double>("expectedMedian");

    BOBUIest::newRow("size=1")
        << (QList<double>() << 1.0)
        << 0.0; // six 0.0 in front of the actual value

    BOBUIest::newRow("size=2")
        << (QList<double>() << 3.0 << 2.0)
        << 0.0; // five 0.0 in front of the actual value

    BOBUIest::newRow("size=3")
        << (QList<double>() << 3.0 << 1.0 << 2.0)
        << 0.0; // four 0.0 in front of the actual value

    BOBUIest::newRow("size=4")
        << (QList<double>() << 3.0 << 1.0 << 2.0 << 4.0)
        << 1.0; // three 0.0 in front of the first actual value, pick 1.0

    BOBUIest::newRow("size=5")
        << (QList<double>() << 3.0 << 1.0 << 2.0 << 3.0 << 1.0)
        << 1.0; // two 0.0 in front of the first actual value, pick 1.0

    BOBUIest::newRow("size=6")
        << (QList<double>() << 3.0 << 1.0 << 2.0 << 3.0 << 1.0 << 2.0)
        << 2.0; // one 0.0 in front of the first actual value, pick 2.0

    BOBUIest::newRow("size=7")
        << QList<double> { 207089.0, 202585.0, 180067.0, 157549.0, 211592.0, 216096.0, 207089.0 }
        << 207089.0;
}

void tst_BobUIConcurrentMedian::median()
{
    QFETCH(const QList<double> , values);
    QFETCH(double, expectedMedian);

    BobUIConcurrent::Median m;
    for (double value : values)
        m.addValue(value);
    QCOMPARE(m.median(), expectedMedian);
}

BOBUIEST_MAIN(tst_BobUIConcurrentMedian)
#include "tst_bobuiconcurrentmedian.moc"
