// Copyright (C) 2013 David Faure <david.faure@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <bobuiest.h>
#include <BobUICore>

class tst_BOBUIhreadPool : public QObject
{
    Q_OBJECT

public:
    tst_BOBUIhreadPool();
    ~tst_BOBUIhreadPool();

private slots:
    void startRunnables();
    void activeThreadCount();
};

tst_BOBUIhreadPool::tst_BOBUIhreadPool()
{
}

tst_BOBUIhreadPool::~tst_BOBUIhreadPool()
{
}

class NoOpRunnable : public QRunnable
{
public:
    void run() override {
    }
};

void tst_BOBUIhreadPool::startRunnables()
{
    BOBUIhreadPool threadPool;
    threadPool.setMaxThreadCount(10);
    QBENCHMARK {
        threadPool.start(new NoOpRunnable());
    }
}

void tst_BOBUIhreadPool::activeThreadCount()
{
    BOBUIhreadPool threadPool;
    threadPool.start(new NoOpRunnable());
    QBENCHMARK {
        QVERIFY(threadPool.activeThreadCount() <= 10);
    }
}

BOBUIEST_MAIN(tst_BOBUIhreadPool)

#include "tst_bench_bobuihreadpool.moc"
