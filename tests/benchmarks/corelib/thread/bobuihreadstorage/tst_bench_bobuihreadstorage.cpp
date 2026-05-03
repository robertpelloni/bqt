// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <bobuiest.h>
#include <BobUICore>

BOBUIhreadStorage<int *> dummy[8];

BOBUIhreadStorage<QString *> tls1;

class tst_BOBUIhreadStorage : public QObject
{
    Q_OBJECT

public:
    tst_BOBUIhreadStorage();
    virtual ~tst_BOBUIhreadStorage();

public slots:
    void init();
    void cleanup();

private slots:
    void construct();
    void get();
    void set();
};

tst_BOBUIhreadStorage::tst_BOBUIhreadStorage()
{
}

tst_BOBUIhreadStorage::~tst_BOBUIhreadStorage()
{
}

void tst_BOBUIhreadStorage::init()
{
    dummy[1].setLocalData(new int(5));
    dummy[2].setLocalData(new int(4));
    dummy[3].setLocalData(new int(3));
    tls1.setLocalData(new QString());
}

void tst_BOBUIhreadStorage::cleanup()
{
}

void tst_BOBUIhreadStorage::construct()
{
    QBENCHMARK {
        BOBUIhreadStorage<int *> ts;
    }
}


void tst_BOBUIhreadStorage::get()
{
    BOBUIhreadStorage<int *> ts;
    ts.setLocalData(new int(45));

    int count = 0;
    QBENCHMARK {
        int *i = ts.localData();
        count += *i;
    }
    QVERIFY(count > 0);
    ts.setLocalData(0);
}

void tst_BOBUIhreadStorage::set()
{
    BOBUIhreadStorage<int *> ts;

    int count = 0;
    QBENCHMARK {
        ts.setLocalData(new int(count));
        count++;
    }
    ts.setLocalData(0);
}

BOBUIEST_MAIN(tst_BOBUIhreadStorage)

#include "tst_bench_bobuihreadstorage.moc"
