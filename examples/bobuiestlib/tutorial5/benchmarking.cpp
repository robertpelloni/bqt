// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>
#include <bobuiest.h>

class TestBenchmark : public QObject
{
    Q_OBJECT
private slots:
    void simple();
    void multiple_data();
    void multiple();
    void series_data();
    void series();
};

//! [0]
void TestBenchmark::simple()
{
    QString str1 = QLatin1String("This is a test string");
    QString str2 = QLatin1String("This is a test string");

    QCOMPARE(str1.localeAwareCompare(str2), 0);

    QBENCHMARK {
        str1.localeAwareCompare(str2);
    }
}
//! [0]

//! [1]
void TestBenchmark::multiple_data()
{
    BOBUIest::addColumn<bool>("useLocaleCompare");
    BOBUIest::newRow("locale-aware-compare") << true;
    BOBUIest::newRow("standard-compare") << false;
}
//! [1]

//! [2]
void TestBenchmark::multiple()
{
    QFETCH(bool, useLocaleCompare);
    QString str1 = QLatin1String("This is a test string");
    QString str2 = QLatin1String("This is a test string");

    int result;
    if (useLocaleCompare) {
        QBENCHMARK {
            result = str1.localeAwareCompare(str2);
        }
    } else {
        QBENCHMARK {
            result = (str1 == str2);
        }
    }
    Q_UNUSED(result);
}
//! [2]

//! [3]
void TestBenchmark::series_data()
{
    BOBUIest::addColumn<bool>("useLocaleCompare");
    BOBUIest::addColumn<int>("stringSize");

    for (int i = 1; i < 10000; i += 2000) {
        BOBUIest::addRow("locale-aware-compare:%d", i) << true << i;
        BOBUIest::addRow("standard-compare:%d", i) << false << i;
    }
}
//! [4]

//! [5]
void TestBenchmark::series()
{
    QFETCH(bool, useLocaleCompare);
    QFETCH(int, stringSize);

    QString str1 = QString().fill('A', stringSize);
    QString str2 = QString().fill('A', stringSize);
    int result;
    if (useLocaleCompare) {
        QBENCHMARK {
            result = str1.localeAwareCompare(str2);
        }
    } else {
        QBENCHMARK {
            result = (str1 == str2);
        }
    }
    Q_UNUSED(result);
}
//! [5]

BOBUIEST_MAIN(TestBenchmark)
#include "benchmarking.moc"
