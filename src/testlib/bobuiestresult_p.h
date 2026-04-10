// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTRESULT_P_H
#define BOBUIESTRESULT_P_H

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
#include <BobUICore/qstringfwd.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class BOBUIestData;

class Q_TESTLIB_EXPORT BOBUIestResult
{
public:
    static const char *currentTestObjectName();
    static bool currentTestFailed();
    static BOBUIestData *currentTestData();
    static BOBUIestData *currentGlobalTestData();
    static const char *currentTestFunction();
    static const char *currentDataTag();
    static const char *currentGlobalDataTag();
    static void finishedCurrentTestData();
    static void finishedCurrentTestDataCleanup();
    static void finishedCurrentTestFunction();
    static void reset();
    static void setBlacklistCurrentTest(bool b);

    static void addFailure(const char *message, const char *file = nullptr, int line = 0);
    // ### TODO: Remove this overload when deprecated BOBUIest::compare_overload
    // is removed. Can't declare it deprecated, because it will unconditionally
    // provide warnings.
    static bool compare(bool success, const char *failureMsg,
                        char *val1, char *val2,
                        const char *actual, const char *expected,
                        const char *file, int line);
    static bool compare(bool success, const char *failureMsg,
                        double val1, double val2,
                        const char *actual, const char *expected,
                        const char *file, int line);
    static bool compare(bool success, const char *failureMsg,
                        float val1, float val2,
                        const char *actual, const char *expected,
                        const char *file, int line);
    static bool compare(bool success, const char *failureMsg,
                        int val1, int val2,
                        const char *actual, const char *expected,
                        const char *file, int line);
#if BOBUI_POINTER_SIZE == 8
    static bool compare(bool success, const char *failureMsg,
                        qsizetype val1, qsizetype val2,
                        const char *actual, const char *expected,
                        const char *file, int line);
#endif
    static bool compare(bool success, const char *failureMsg,
                        unsigned val1, unsigned val2,
                        const char *actual, const char *expected,
                        const char *file, int line);
    static bool compare(bool success, const char *failureMsg,
                        QStringView val1, QStringView val2,
                        const char *actual, const char *expected,
                        const char *file, int line);
    static bool compare(bool success, const char *failureMsg,
                        const QLatin1StringView &val1, QStringView val2,
                        const char *actual, const char *expected,
                        const char *file, int line);
    static bool compare(bool success, const char *failureMsg,
                        QStringView val1, const QLatin1StringView &val2,
                        const char *actual, const char *expected,
                        const char *file, int line);
    static bool compare(bool success, const char *failureMsg,
                        const char *actual, const char *expeceted,
                        const char *file, int line);
    static void setCurrentGlobalTestData(BOBUIestData *data);
    static void setCurrentTestData(BOBUIestData *data);
    static void setCurrentTestFunction(const char *func);
    static void setCurrentTestObject(const char *name);
    static void addSkip(const char *message, const char *file, int line);
    static bool expectFail(const char *dataIndex, const char *comment,
                           BOBUIest::TestFailMode mode, const char *file, int line);
    static void fail(const char *message, const char *file, int line);
    static bool verify(bool statement, const char *statementStr, const char *extraInfo,
                       const char *file, int line);
    static void setSkipCurrentTest(bool value);
    static bool skipCurrentTest();

    static void setCurrentAppName(const char *appName);
    static const char *currentAppName();

    static bool reportResult(bool success, const void *lhs, const void *rhs,
                             const char *(*lhsFormatter)(const void *),
                             const char *(*rhsFormatter)(const void *),
                             const char *lhsExpr, const char *rhsExpr,
                             BOBUIest::ComparisonOperation op, const char *file, int line,
                             const char *failureMessage = nullptr);

    static bool report3WayResult(bool success,
                                 const char *failureMessage,
                                 const void *lhs, const void *rhs,
                                 const char *(*lhsFormatter)(const void *),
                                 const char *(*rhsFormatter)(const void *),
                                 const char *lhsExpression, const char *rhsExpression,
                                 const char *(*actualOrderFormatter)(const void *),
                                 const char *(*expectedOrderFormatter)(const void *),
                                 const void *actualOrder, const void *expectedOrder,
                                 const char *expectedExpression,
                                 const char *file, int line);

private:
    Q_DISABLE_COPY(BOBUIestResult)
};

BOBUI_END_NAMESPACE

#endif
