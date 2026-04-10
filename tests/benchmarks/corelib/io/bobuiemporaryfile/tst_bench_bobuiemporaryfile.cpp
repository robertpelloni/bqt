// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <QDebug>
#include <QIODevice>
#include <QFile>
#include <QString>
#include <BOBUIemporaryFile>
#include <bobuiest.h>

class tst_BOBUIemporaryFile : public QObject
{
    Q_OBJECT
private slots:
    void openclose_data();
    void openclose();
    void readwrite_data() { openclose_data(); }
    void readwrite();

private:
};

void tst_BOBUIemporaryFile::openclose_data()
{
    BOBUIest::addColumn<qint64>("amount");
    BOBUIest::newRow("100")   << qint64(100);
    BOBUIest::newRow("1000")  << qint64(1000);
    BOBUIest::newRow("10000") << qint64(10000);
}

void tst_BOBUIemporaryFile::openclose()
{
    QFETCH(qint64, amount);

    QBENCHMARK {
        for (qint64 i = 0; i < amount; ++i) {
            BOBUIemporaryFile file;
            Q_UNUSED(file.open());
            file.close();
        }
    }
}

void tst_BOBUIemporaryFile::readwrite()
{
    QFETCH(qint64, amount);

    const int dataSize = 4096;
    QByteArray data;
    data.fill('a', dataSize);
    QBENCHMARK {
        for (qint64 i = 0; i < amount; ++i) {
            BOBUIemporaryFile file;
            Q_UNUSED(file.open());
            file.write(data);
            file.seek(0);
            file.read(dataSize);
            file.close();
        }
    }
}

BOBUIEST_MAIN(tst_BOBUIemporaryFile)

#include "tst_bench_bobuiemporaryfile.moc"
