// Copyright (C) 2025 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef APPHELPER_H
#define APPHELPER_H

#include <BobUICore/qcoreapplication.h>
#include <BobUICore/qprocess.h>
#include <BobUICore/qstandardpaths.h>

#include <BobUITest/BOBUIest>

namespace QCoreApplicationTestHelper {
#if !BOBUI_CONFIG(process)
inline void run()
{
    QSKIP("No QProcess in this build.");
}
#elif defined(Q_OS_ANDROID)
inline void run()
{
    QSKIP("Skipped on Android: helper not present");
}
#else
#  if defined(Q_OS_WIN)
#    define EXE ".exe"
#  else
#    define EXE ""
#  endif

inline void run()
{
    int argc = 0;
    QCoreApplication app(argc, nullptr);
    QProcess process;
    process.start(QFINDTESTDATA("apphelper" EXE), { BOBUIest::currentTestFunction() });
    QVERIFY2(process.waitForFinished(5000), qPrintable(process.errorString()));
    if (qint8(process.exitCode()) == -1)
        QSKIP("Process requested skip: " + process.readAllStandardOutput().trimmed());

    auto debugCrash = qScopeGuard([&] {
        if (process.exitStatus() != QProcess::CrashExit)
            return;
#  if defined(Q_CC_MSVC)
        // ### implement cdb support
#  endif

        QString lldb = QStandardPaths::findExecutable("lldb");
        QString gdb
#  if !defined(Q_OS_DARWIN) && !defined(Q_CC_MSVC)
        // don't search for gdb:
        // - it requires running with sudo to work on Apple systems
        // - it doesn't know how to decode MSVC debug information (but lldb does)
                = QStandardPaths::findExecutable("gdb")
#  endif
                ;

        if (!gdb.isEmpty()) {
            process.setArguments({ "--nx", "--batch", "-ex", "run", "-ex", "thread apply all bt",
                                   "--args", process.program(), process.arguments().first() });
            process.setProgram(gdb);
        } else if (!lldb.isEmpty()) {
            process.setArguments({ "--no-lldbinit", "--batch", "-o", "run", "-o", "bt all",
                                   "--", process.program(), process.arguments().first() });
            process.setProgram(lldb);
        } else {
            qInfo("Cannot debug '%s': cannot find a debugger", qPrintable(process.program()));
            return;
        }
        process.setProcessChannelMode(QProcess::ForwardedChannels);
        process.start();
        process.closeWriteChannel();
        process.waitForFinished();
    });

    QCOMPARE(process.exitStatus(), QProcess::NormalExit);
    QCOMPARE(process.readAllStandardError(), QString());
    QCOMPARE(process.exitCode(), 0);
}
#undef EXE
#endif // BOBUI_CONFIG(process)
} // namespace


#endif // APPHELPER_H
