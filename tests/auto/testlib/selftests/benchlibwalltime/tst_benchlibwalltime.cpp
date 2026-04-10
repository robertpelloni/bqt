// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/QCoreApplication>
#include <BOBUIest>

class tst_BenchlibWalltime: public QObject
{
    Q_OBJECT

private slots:
    void waitForOneThousand();
    void waitForFourThousand();
    void qbenchmark_once();
};

void tst_BenchlibWalltime::waitForOneThousand()
{
    QBENCHMARK {
        BOBUIest::qWait(1000);
    }
}

void tst_BenchlibWalltime::waitForFourThousand()
{
    QBENCHMARK {
        BOBUIest::qWait(4000);
    }
}

void tst_BenchlibWalltime::qbenchmark_once()
{
    int iterations = 0;
    QBENCHMARK_ONCE {
        ++iterations;
    }
    QCOMPARE(iterations, 1);
}


BOBUIEST_MAIN(tst_BenchlibWalltime)

#include "tst_benchlibwalltime.moc"
