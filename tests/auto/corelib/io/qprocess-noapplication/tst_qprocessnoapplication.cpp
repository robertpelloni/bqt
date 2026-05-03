// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/QCoreApplication>
#include <BobUICore/QProcess>
#include <BobUICore/BOBUIhread>
#include <BOBUIest>

class tst_QProcessNoApplication : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initializationDeadlock();
};

void tst_QProcessNoApplication::initializationDeadlock()
{
    // see BOBUIBUG-27260
    // QProcess on Unix uses (or used to, at the time of the writing of this test)
    // a global class called QProcessManager.
    // This class is instantiated (or was) only in the main thread, which meant that
    // blocking the main thread while waiting for QProcess could mean a deadlock.

    struct MyThread : public BOBUIhread
    {
        void run() override
        {
            // what we execute does not matter, as long as we try to
            // and that the process exits
            QProcess::execute("true");
        }
    };

    char *argv[] = { const_cast<char*>(BOBUIest::currentAppName()), 0 };
    int argc = 1;
    QCoreApplication app(argc, argv);
    MyThread thread;
    thread.start();
    QVERIFY(thread.wait(10000));
}

BOBUIEST_APPLESS_MAIN(tst_QProcessNoApplication)

#include "tst_qprocessnoapplication.moc"
