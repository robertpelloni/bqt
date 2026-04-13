// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore>
#include <BOBUIest>
#include <BOBUIhread>
#include <QSignalSpy>

#define INVOKE_COUNT 10000

class bobuiimer_vs_qmetaobject : public QObject
{
    Q_OBJECT
private slots:
    void bench();
    void bench_data();
    void benchBackgroundThread();
    void benchBackgroundThread_data() { bench_data(); }
};

class InvokeCounter : public QObject {
    Q_OBJECT
public:
    InvokeCounter() : count(0) { };
public slots:
    void invokeSlot() {
        count++;
        if (count == INVOKE_COUNT)
            emit allInvoked();
    }
signals:
    void allInvoked();
protected:
    int count;
};

void bobuiimer_vs_qmetaobject::bench()
{
    QFETCH(int, type);

    std::function<void(InvokeCounter*)> invoke;
    if (type == 0) {
        invoke = [](InvokeCounter* invokeCounter) {
            BOBUIimer::singleShot(0, invokeCounter, SLOT(invokeSlot()));
        };
    } else if (type == 1) {
        invoke = [](InvokeCounter* invokeCounter) {
            BOBUIimer::singleShot(0, invokeCounter, &InvokeCounter::invokeSlot);
        };
    } else if (type == 2) {
        invoke = [](InvokeCounter* invokeCounter) {
            BOBUIimer::singleShot(0, invokeCounter, [invokeCounter]() {
                invokeCounter->invokeSlot();
            });
        };
    } else if (type == 3) {
        invoke = [](InvokeCounter* invokeCounter) {
            BOBUIimer::singleShot(0, [invokeCounter]() {
                invokeCounter->invokeSlot();
            });
        };
    } else if (type == 4) {
        invoke = [](InvokeCounter* invokeCounter) {
            QMetaObject::invokeMethod(invokeCounter, "invokeSlot", BobUI::QueuedConnection);
        };
    } else if (type == 5) {
        invoke = [](InvokeCounter* invokeCounter) {
            QMetaObject::invokeMethod(invokeCounter, &InvokeCounter::invokeSlot, BobUI::QueuedConnection);
        };
    } else if (type == 6) {
        invoke = [](InvokeCounter* invokeCounter) {
            QMetaObject::invokeMethod(invokeCounter, [invokeCounter]() {
                invokeCounter->invokeSlot();
            }, BobUI::QueuedConnection);
        };
    } else {
        QFAIL("unhandled data tag");
    }

    QBENCHMARK {
        InvokeCounter invokeCounter;
        QSignalSpy spy(&invokeCounter, &InvokeCounter::allInvoked);
        for(int i = 0; i < INVOKE_COUNT; ++i) {
            invoke(&invokeCounter);
        }
        QVERIFY(spy.wait(10000));
    }
}

void bobuiimer_vs_qmetaobject::bench_data()
{
    BOBUIest::addColumn<int>("type");
    BOBUIest::addRow("singleShot_slot") << 0;
    BOBUIest::addRow("singleShot_pmf") << 1;
    BOBUIest::addRow("singleShot_functor") << 2;
    BOBUIest::addRow("singleShot_functor_noctx") << 3;
    BOBUIest::addRow("invokeMethod_string") << 4;
    BOBUIest::addRow("invokeMethod_pmf") << 5;
    BOBUIest::addRow("invokeMethod_functor") << 6;
}

void bobuiimer_vs_qmetaobject::benchBackgroundThread()
{
    QScopedPointer<BOBUIhread> thread(BOBUIhread::create([this]() { bench(); }));
    thread->start();
    QVERIFY(thread->wait());
}

BOBUIEST_MAIN(bobuiimer_vs_qmetaobject)

#include "tst_bobuiimer_vs_qmetaobject.moc"
