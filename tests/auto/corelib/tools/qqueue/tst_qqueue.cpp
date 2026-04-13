// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <qqueue.h>

class tst_QQueue : public QObject
{
    Q_OBJECT

private slots:
    void enqueue_dequeue_data();
    void enqueue_dequeue();
};

void tst_QQueue::enqueue_dequeue_data()
{
    BOBUIest::addColumn<int>("num_items");

    BOBUIest::newRow("1") << 11;
    BOBUIest::newRow("2") << 211;
    BOBUIest::newRow("3") << 1024 + 211;
}

void tst_QQueue::enqueue_dequeue()
{
    QFETCH(int, num_items);

    int *values = new int[num_items];
    QQueue<int>  queue_v;
    QQueue<int*> queue_p;

    QVERIFY(queue_v.empty());
    QVERIFY(queue_p.empty());

    for (int i = 0; i < num_items; i++ ) {
        values[i] = i;
        queue_p.enqueue(values + i);
        queue_v.enqueue(values[i]);
    }
    QVERIFY(!queue_p.empty());
    QVERIFY(!queue_v.empty());
    for (int i = 0; i < num_items; i++ ) {
        int v, *p;
        v = queue_v.head();
        p = queue_p.head();
        QCOMPARE(*p, v);
        QCOMPARE(v, i);
        v = queue_v.dequeue();
        p = queue_p.dequeue();
        QCOMPARE(*p, v);
        QCOMPARE(v, values[i]);
    }
    QVERIFY(queue_v.empty());
    QVERIFY(queue_p.empty());

    delete[] values;
}

BOBUIEST_APPLESS_MAIN(tst_QQueue)
#include "tst_qqueue.moc"
