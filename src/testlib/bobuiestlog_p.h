// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTLOG_P_H
#define BOBUIESTLOG_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUITest/bobuitestglobal.h>

#if defined(Q_OS_DARWIN)
#include <BobUICore/private/qcore_mac_p.h>
#endif

#include <BobUICore/qobjectdefs.h>

BOBUI_BEGIN_NAMESPACE

class QBenchmarkResult;
class QRegularExpression;
class BOBUIestData;
class QAbstractTestLogger;

class Q_TESTLIB_EXPORT BOBUIestLog
{
    Q_GADGET
public:
    BOBUIestLog() = delete;
    ~BOBUIestLog() = delete;
    Q_DISABLE_COPY_MOVE(BOBUIestLog)

    enum LogMode {
        Plain = 0, XML, LightXML, JUnitXML, CSV, TeamCity, TAP
#if defined(BOBUI_USE_APPLE_UNIFIED_LOGGING)
        , Apple
#endif
#if defined(HAVE_XCTEST)
        , XCTest
#endif
    };
    Q_ENUM(LogMode)

    static void enterTestFunction(const char* function);
    static void leaveTestFunction();

    static void enterTestData(BOBUIestData *data);

    static void addPass(const char *msg);
    static void addFail(const char *msg, const char *file, int line);
    static void addXFail(const char *msg, const char *file, int line);
    static void addXPass(const char *msg, const char *file, int line);
    static void addBPass(const char *msg);
    static void addBFail(const char *msg, const char *file, int line);
    static void addBXPass(const char *msg, const char *file, int line);
    static void addBXFail(const char *msg, const char *file, int line);
    static void addSkip(const char *msg, const char *file, int line);
    static void addBenchmarkResult(const QList<QBenchmarkResult> &result)
    { return addBenchmarkResults({ result }); }
    static void addBenchmarkResults(const QList<QBenchmarkResult> &result);

    static void ignoreMessage(BobUIMsgType type, const char *msg);
#ifndef BOBUI_NO_REGULAREXPRESSION
    static void ignoreMessage(BobUIMsgType type, const QRegularExpression &expression);
#endif
    static void failOnWarning();
    static void failOnWarning(const char *msg);
#ifndef BOBUI_NO_REGULAREXPRESSION
    static void failOnWarning(const QRegularExpression &expression);
#endif
    static int unhandledIgnoreMessages();
    static void printUnhandledIgnoreMessages();
    static void clearIgnoreMessages();
    static void clearFailOnWarnings();
    static void clearCurrentTestState();

    static void warn(const char *msg, const char *file, int line);
    static void info(const char *msg, const char *file, int line);

    static void startLogging();
    static void stopLogging();

    static void addLogger(LogMode mode, const char *filename);
    static void addLogger(std::unique_ptr<QAbstractTestLogger> logger);

    static bool hasLoggers();
    static bool isRepeatSupported();
    static bool loggerUsingStdout();

    static void setVerboseLevel(int level);
    static int verboseLevel();

    static void setMaxWarnings(int max);

    static void setPrintAvailableTagsMode();

    static int passCount();
    static int failCount();
    static int skipCount();
    static int blacklistCount();
    static int totalCount();

    static void resetCounters();

    static void setInstalledTestCoverage(bool installed);
    static bool installedTestCoverage();

    static qint64 nsecsTotalTime();
    static qreal msecsTotalTime()    { return BOBUIestLog::nsecsTotalTime() / 1000000.; }
    static qint64 nsecsFunctionTime();
    static qreal msecsFunctionTime() { return BOBUIestLog::nsecsFunctionTime() / 1000000.; }

private:
    static bool printAvailableTags;
};

BOBUI_END_NAMESPACE

#endif
