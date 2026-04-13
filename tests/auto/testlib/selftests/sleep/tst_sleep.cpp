// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/QCoreApplication>
#include <BobUICore/QElapsedTimer>
#include <BOBUIest>

#ifdef Q_OS_UNIX
#include <BobUICore/private/qcore_unix_p.h>
#include <BobUICore/qsystemdetection.h>

#include <time.h>
#endif

using namespace std::chrono_literals;

class tst_Sleep: public QObject
{
    Q_OBJECT

private slots:
    void sleep();
    void wait();
};

void tst_Sleep::sleep()
{
    // Subtracting 10ms as a margin for error
    static constexpr auto MarginForError = 10ms;

    QElapsedTimer t;
    t.start();

    // Test qSleep(int) overload, too
    BOBUIest::qSleep(100);
    QCOMPARE_GT(t.durationElapsed(), 100ms - MarginForError);

    BOBUIest::qSleep(1s);
    QCOMPARE_GT(t.durationElapsed(), 1s - MarginForError);

    BOBUIest::qSleep(10s);
    QCOMPARE_GT(t.durationElapsed(), 10s - MarginForError);
}

void tst_Sleep::wait()
{
    QElapsedTimer t;
    t.start();

    BOBUIest::qWait(1);
    QCOMPARE_GE(t.durationElapsed(), 1ms);

    BOBUIest::qWait(10);
    QCOMPARE_GE(t.durationElapsed(), 11ms);

    BOBUIest::qWait(100);
    QCOMPARE_GE(t.durationElapsed(), 111ms);

    BOBUIest::qWait(1000);
    QCOMPARE_GE(t.durationElapsed(), 1111ms);
}

BOBUIEST_MAIN(tst_Sleep)

#include "tst_sleep.moc"
