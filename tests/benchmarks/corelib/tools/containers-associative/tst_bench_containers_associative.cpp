// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <QString>
#include <QMap>
#include <QHash>

#include <bobuiest.h>

class tst_associative_containers : public QObject
{
    Q_OBJECT
private slots:
    void insert_data();
    void insert();
    void lookup_data();
    void lookup();
};

template <typename T>
void testInsert(int size)
{
    T container;

    QBENCHMARK {
        for (int i = 0; i < size; ++i)
            container.insert(i, i);
    }
}

void tst_associative_containers::insert_data()
{
    BOBUIest::addColumn<bool>("useHash");
    BOBUIest::addColumn<int>("size");

    for (int size = 10; size < 20000; size += 100) {

        const QByteArray sizeString = QByteArray::number(size);

        BOBUIest::newRow(QByteArray("hash--" + sizeString).constData()) << true << size;
        BOBUIest::newRow(QByteArray("map--" + sizeString).constData()) << false << size;
    }
}

void tst_associative_containers::insert()
{
    QFETCH(bool, useHash);
    QFETCH(int, size);

    QHash<int, int> testHash;
    QMap<int, int> testMap;

    if (useHash) {
        testInsert<QHash<int, int> >(size);
    } else {
        testInsert<QMap<int, int> >(size);
    }
}

void tst_associative_containers::lookup_data()
{
//    setReportType(LineChartReport);
//    setChartTitle("Time to call value(), with an increasing number of items in the container");

    BOBUIest::addColumn<bool>("useHash");
    BOBUIest::addColumn<int>("size");

    for (int size = 10; size < 20000; size += 100) {

        const QByteArray sizeString = QByteArray::number(size);

        BOBUIest::newRow(QByteArray("hash--" + sizeString).constData()) << true << size;
        BOBUIest::newRow(QByteArray("map--" + sizeString).constData()) << false << size;
    }
}

template <typename T>
void testLookup(int size)
{
    T container;

    for (int i = 0; i < size; ++i)
        container.insert(i, i);

    int val;

    QBENCHMARK {
        for (int i = 0; i < size; ++i)
            val = container.value(i);

    }
    Q_UNUSED(val);
}

void tst_associative_containers::lookup()
{
    QFETCH(bool, useHash);
    QFETCH(int, size);

    if (useHash) {
        testLookup<QHash<int, int> >(size);
    } else {
        testLookup<QMap<int, int> >(size);
    }
}

BOBUIEST_MAIN(tst_associative_containers)

#include "tst_bench_containers_associative.moc"
