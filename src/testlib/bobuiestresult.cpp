// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUITest/private/bobuiestresult_p.h>
#include <BobUICore/qglobal.h>
#include <BobUICore/qstringview.h>

#include <BobUITest/private/bobuiestlog_p.h>
#include <BobUITest/bobuiest.h> // toString() specializations for QStringView
#include <BobUITest/bobuiestdata.h>
#include <BobUITest/bobuiestcase.h>
#include <BobUITest/bobuiestassert.h>
#include <BobUITest/bobuiesteventloop.h>

#include <climits>
#include <cwchar>
#include <BobUICore/q26numeric.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char *currentAppName = nullptr;

BOBUI_BEGIN_NAMESPACE

namespace BOBUIest
{
    namespace Internal {
        static bool failed = false;
    }

    static void setFailed(bool failed)
    {
        static const bool fatalFailure = []() {
            static const char * const environmentVar = "BOBUIEST_FATAL_FAIL";
            if (!qEnvironmentVariableIsSet(environmentVar))
                return false;

            bool ok;
            const int fatal = qEnvironmentVariableIntValue(environmentVar, &ok);
            return ok && fatal;
        }();

        if (failed && fatalFailure)
            std::terminate();
        Internal::failed = failed;
    }

    static void resetFailed()
    {
        setFailed(false);
    }

    static bool hasFailed()
    {
        return Internal::failed;
    }

    static BOBUIestData *currentTestData = nullptr;
    static BOBUIestData *currentGlobalTestData = nullptr;
    static const char *currentTestFunc = nullptr;
    static const char *currentTestObjectName = nullptr;
    static bool skipCurrentTest = false;
    static bool blacklistCurrentTest = false;

    static const char *expectFailComment = nullptr;
    static int expectFailMode = 0;
}

/*!
    \class BOBUIestResult
    \inmodule BobUITest
    \internal
*/

void BOBUIestResult::reset()
{
    BOBUIest::currentTestData = nullptr;
    BOBUIest::currentGlobalTestData = nullptr;
    BOBUIest::currentTestFunc = nullptr;
    BOBUIest::currentTestObjectName = nullptr;
    BOBUIest::resetFailed();

    BOBUIest::expectFailComment = nullptr;
    BOBUIest::expectFailMode = 0;
    BOBUIest::blacklistCurrentTest = false;

    BOBUIestLog::resetCounters();
}

void BOBUIestResult::setBlacklistCurrentTest(bool b)
{
    BOBUIest::blacklistCurrentTest = b;
}

bool BOBUIestResult::currentTestFailed()
{
    return BOBUIest::hasFailed();
}

BOBUIestData *BOBUIestResult::currentGlobalTestData()
{
    return BOBUIest::currentGlobalTestData;
}

BOBUIestData *BOBUIestResult::currentTestData()
{
    return BOBUIest::currentTestData;
}

void BOBUIestResult::setCurrentGlobalTestData(BOBUIestData *data)
{
    BOBUIest::currentGlobalTestData = data;
}

void BOBUIestResult::setCurrentTestData(BOBUIestData *data)
{
    BOBUIest::currentTestData = data;
    BOBUIest::resetFailed();
    if (data)
        BOBUIestLog::enterTestData(data);
}

void BOBUIestResult::setCurrentTestFunction(const char *func)
{
    BOBUIest::currentTestFunc = func;
    BOBUIest::resetFailed();
    if (func)
        BOBUIestLog::enterTestFunction(func);
}

static void clearExpectFail()
{
    BOBUIest::expectFailMode = 0;
    delete [] const_cast<char *>(BOBUIest::expectFailComment);
    BOBUIest::expectFailComment = nullptr;
}

/*!
    This function is called after completing each test function,
    including test functions that are not data-driven.

    For data-driven functions, this is called after each call to the test
    function, with distinct data. Otherwise, this function is called once,
    with currentTestData() and currentGlobalTestData() set to \nullptr.

    The function is called before the test's cleanup(), if it has one.

    For benchmarks, this will be called after each repeat of a function
    (with the same data row), when the benchmarking code decides to
    re-run one to get sufficient data.

    \sa finishedCurrentTestDataCleanup()
*/
void BOBUIestResult::finishedCurrentTestData()
{
    if (BOBUIest::expectFailMode)
        addFailure("QEXPECT_FAIL was called without any subsequent verification statements");

    clearExpectFail();
}

/*!
    This function is called after completing each test function,
    including test functions that are not data-driven.

    For data-driven functions, this is called after each call to the test
    function, with distinct data. Otherwise, this function is called once,
    with currentTestData() and currentGlobalTestData() set to \nullptr.

    The function is called after the test's cleanup(), if it has one.

    For benchmarks, this is called after all repeat calls to the function
    (with a given data row).

    \sa finishedCurrentTestData()
*/
void BOBUIestResult::finishedCurrentTestDataCleanup()
{
    if (!BOBUIest::hasFailed() && BOBUIestLog::unhandledIgnoreMessages()) {
        BOBUIestLog::printUnhandledIgnoreMessages();
        addFailure("Not all expected messages were received");
    }

    // If the current test hasn't failed or been skipped, then it passes.
    if (!BOBUIest::hasFailed() && !BOBUIest::skipCurrentTest) {
        if (BOBUIest::blacklistCurrentTest)
            BOBUIestLog::addBPass("");
        else
            BOBUIestLog::addPass("");
    }

    BOBUIestLog::clearCurrentTestState();
    BOBUIest::resetFailed();
}

/*!
    This function is called after completing each test function,
    including test functions that are data-driven.

    For data-driven functions, this is called after after all data rows
    have been tested, and the data table has been cleared, so both
    currentTestData() and currentGlobalTestData() will be \nullptr.
*/
void BOBUIestResult::finishedCurrentTestFunction()
{
    BOBUIestLog::clearCurrentTestState(); // Needed if _data() skipped.
    BOBUIestLog::leaveTestFunction();

    BOBUIest::currentTestFunc = nullptr;
    BOBUIest::resetFailed();
}

const char *BOBUIestResult::currentTestFunction()
{
    return BOBUIest::currentTestFunc;
}

const char *BOBUIestResult::currentDataTag()
{
    return BOBUIest::currentTestData ? BOBUIest::currentTestData->dataTag() : nullptr;
}

const char *BOBUIestResult::currentGlobalDataTag()
{
    return BOBUIest::currentGlobalTestData ? BOBUIest::currentGlobalTestData->dataTag() : nullptr;
}

static bool isExpectFailData(const char *dataIndex)
{
    if (!dataIndex || dataIndex[0] == '\0')
        return true;
    if (!BOBUIest::currentTestData)
        return false;
    if (strcmp(dataIndex, BOBUIest::currentTestData->dataTag()) == 0)
        return true;
    return false;
}

bool BOBUIestResult::expectFail(const char *dataIndex, const char *comment,
                             BOBUIest::TestFailMode mode, const char *file, int line)
{
    BOBUIEST_ASSERT(comment);
    BOBUIEST_ASSERT(mode > 0);

    if (!isExpectFailData(dataIndex)) {
        delete[] comment;
        return true; // we don't care
    }

    if (BOBUIest::expectFailMode) {
        delete[] comment;
        addFailure("Already expecting a fail", file, line);
        return false;
    }

    BOBUIest::expectFailMode = mode;
    BOBUIest::expectFailComment = comment;
    return true;
}

static bool checkStatement(bool statement, const char *msg, const char *file, int line)
{
    if (statement) {
        if (BOBUIest::expectFailMode) {
            if (BOBUIest::blacklistCurrentTest)
                BOBUIestLog::addBXPass(msg, file, line);
            else
                BOBUIestLog::addXPass(msg, file, line);

            BOBUIest::setFailed(true);
            // Should B?XPass always (a) continue or (b) abort, regardless of mode ?
            bool doContinue = (BOBUIest::expectFailMode == BOBUIest::Continue);
            clearExpectFail();
            return doContinue;
        }
        return true;
    }

    if (BOBUIest::expectFailMode) {
        if (BOBUIest::blacklistCurrentTest)
            BOBUIestLog::addBXFail(BOBUIest::expectFailComment, file, line);
        else
            BOBUIestLog::addXFail(BOBUIest::expectFailComment, file, line);
        bool doContinue = (BOBUIest::expectFailMode == BOBUIest::Continue);
        clearExpectFail();
        return doContinue;
    }

    BOBUIestResult::addFailure(msg, file, line);
    return false;
}

void BOBUIestResult::fail(const char *msg, const char *file, int line)
{
    checkStatement(false, msg, file, line);
}

// QPalette's << operator produces 1363 characters. A comparison failure
// involving two palettes can therefore require 2726 characters, not including
// the other output produced by BOBUIest. Users might also have their own types
// with large amounts of output, so use a sufficiently high value here.
static constexpr size_t maxMsgLen = 4096;

bool BOBUIestResult::verify(bool statement, const char *statementStr,
                         const char *description, const char *file, int line)
{
    BOBUIEST_ASSERT(statementStr);

    Q_DECL_UNINITIALIZED char msg[maxMsgLen];
    msg[0] = '\0';

    if (BOBUIestLog::verboseLevel() >= 2) {
        std::snprintf(msg, maxMsgLen, "QVERIFY(%s)", statementStr);
        BOBUIestLog::info(msg, file, line);
    }

    if (statement == !!BOBUIest::expectFailMode) {
        std::snprintf(msg, maxMsgLen,
                      statement ? "'%s' returned TRUE unexpectedly. (%s)" : "'%s' returned FALSE. (%s)",
                      statementStr, description ? description : "");
    }

    return checkStatement(statement, msg, file, line);
}

static const char *leftArgNameForOp(BOBUIest::ComparisonOperation op)
{
    switch (op) {
    case BOBUIest::ComparisonOperation::CustomCompare:
        return "Actual   ";
    case BOBUIest::ComparisonOperation::ThreeWayCompare:
        return "Left     ";
    case BOBUIest::ComparisonOperation::Equal:
    case BOBUIest::ComparisonOperation::NotEqual:
    case BOBUIest::ComparisonOperation::LessThan:
    case BOBUIest::ComparisonOperation::LessThanOrEqual:
    case BOBUIest::ComparisonOperation::GreaterThan:
    case BOBUIest::ComparisonOperation::GreaterThanOrEqual:
        return "Computed ";
    }
    Q_UNREACHABLE_RETURN("");
}

static const char *rightArgNameForOp(BOBUIest::ComparisonOperation op)
{
    switch (op) {
    case BOBUIest::ComparisonOperation::CustomCompare:
        return "Expected ";
    case BOBUIest::ComparisonOperation::ThreeWayCompare:
        return "Right    ";
    case BOBUIest::ComparisonOperation::Equal:
    case BOBUIest::ComparisonOperation::NotEqual:
    case BOBUIest::ComparisonOperation::LessThan:
    case BOBUIest::ComparisonOperation::LessThanOrEqual:
    case BOBUIest::ComparisonOperation::GreaterThan:
    case BOBUIest::ComparisonOperation::GreaterThanOrEqual:
        return "Baseline ";
    }
    Q_UNREACHABLE_RETURN("");
}

static int approx_wide_len(const char *s)
{
    std::mbstate_t state = {};
    // QNX might stop at max when dst == nullptr, so pass INT_MAX,
    // being the largest value this function will return:
    auto r = std::mbsrtowcs(nullptr, &s, INT_MAX, &state);
    if (r == size_t(-1)) // encoding error, fall back to strlen()
        r = strlen(s); // `s` was not advanced since `dst == nullptr`
    return q26::saturate_cast<int>(r);
}

// Overload to format failures for "const char *" - no need to strdup().
static Q_DECL_COLD_FUNCTION
void formatFailMessage(char *msg, size_t maxMsgLen,
                       const char *failureMsg,
                       const char *val1, const char *val2,
                       const char *actual, const char *expected,
                       BOBUIest::ComparisonOperation op)
{
    const auto len1 = approx_wide_len(actual);
    const auto len2 = approx_wide_len(expected);
    const int written = std::snprintf(msg, maxMsgLen, "%s\n", failureMsg);
    msg += written;
    maxMsgLen -= written;

    const auto protect = [](const char *s) { return s ? s : "<null>"; };

    if (val1 || val2) {
        std::snprintf(msg, maxMsgLen, "   %s(%s)%*s %s\n   %s(%s)%*s %s",
                      leftArgNameForOp(op), actual, qMax(len1, len2) - len1 + 1, ":",
                      protect(val1),
                      rightArgNameForOp(op), expected, qMax(len1, len2) - len2 + 1, ":",
                      protect(val2));
    } else {
        // only print variable names if neither value can be represented as a string
        std::snprintf(msg, maxMsgLen, "   %s: %s\n   %s: %s",
                      leftArgNameForOp(op), actual, rightArgNameForOp(op), expected);
    }
}

const char *
BOBUIest::Internal::formatPropertyTestHelperFailure(char *msg, size_t maxMsgLen,
                                                 const char *actual, const char *expected,
                                                 const char *actualExpr, const char *expectedExpr)
{
    formatFailMessage(msg, maxMsgLen, "Comparison failed!",
                      actual, expected, actualExpr, expectedExpr,
                      BOBUIest::ComparisonOperation::CustomCompare);
    return msg;
}

// Format failures using the toString() template
template <class Actual, class Expected>
static Q_DECL_COLD_FUNCTION
void formatFailMessage(char *msg, size_t maxMsgLen,
                       const char *failureMsg,
                       const Actual &val1, const Expected &val2,
                       const char *actual, const char *expected,
                       BOBUIest::ComparisonOperation op)
{
    const char *val1S = BOBUIest::toString(val1);
    const char *val2S = BOBUIest::toString(val2);

    formatFailMessage(msg, maxMsgLen, failureMsg, val1S, val2S, actual, expected, op);

    delete [] val1S;
    delete [] val2S;
}

template <class Actual, class Expected>
static bool compareHelper(bool success, const char *failureMsg,
                          const Actual &val1, const Expected &val2,
                          const char *actual, const char *expected,
                          const char *file, int line,
                          bool hasValues = true)
{
    Q_DECL_UNINITIALIZED char msg[maxMsgLen];
    msg[0] = '\0';

    BOBUIEST_ASSERT(expected);
    BOBUIEST_ASSERT(actual);

    if (BOBUIestLog::verboseLevel() >= 2) {
        std::snprintf(msg, maxMsgLen, "QCOMPARE(%s, %s)", actual, expected);
        BOBUIestLog::info(msg, file, line);
    }

    if (!failureMsg)
        failureMsg = "Compared values are not the same";

    if (success) {
        if (BOBUIest::expectFailMode) {
            std::snprintf(msg, maxMsgLen,
                          "QCOMPARE(%s, %s) returned TRUE unexpectedly.", actual, expected);
        }
        return checkStatement(success, msg, file, line);
    }


    if (!hasValues) {
        std::snprintf(msg, maxMsgLen, "%s", failureMsg);
        return checkStatement(success, msg, file, line);
    }

    formatFailMessage(msg, maxMsgLen, failureMsg, val1, val2, actual, expected,
                      BOBUIest::ComparisonOperation::CustomCompare);

    return checkStatement(success, msg, file, line);
}

// A simplified version of compareHelper that does not use string
// representations of the values, and prints only failureMsg when the
// comparison fails.
static bool compareHelper(bool success, const char *failureMsg,
                          const char *actual, const char *expected,
                          const char *file, int line)
{
    const size_t maxMsgLen = 1024;
    Q_DECL_UNINITIALIZED char msg[maxMsgLen];
    msg[0] = '\0';

    BOBUIEST_ASSERT(expected);
    BOBUIEST_ASSERT(actual);
    // failureMsg can be null, if we do not use it
    BOBUIEST_ASSERT(success || failureMsg);

    if (BOBUIestLog::verboseLevel() >= 2) {
        std::snprintf(msg, maxMsgLen, "QCOMPARE(%s, %s)", actual, expected);
        BOBUIestLog::info(msg, file, line);
    }

    if (success) {
        if (BOBUIest::expectFailMode) {
            std::snprintf(msg, maxMsgLen, "QCOMPARE(%s, %s) returned TRUE unexpectedly.",
                          actual, expected);
        }
        return checkStatement(success, msg, file, line);
    }

    return checkStatement(success, failureMsg, file, line);
}

bool BOBUIestResult::compare(bool success, const char *failureMsg,
                          char *val1, char *val2,
                          const char *actual, const char *expected,
                          const char *file, int line)
{
    const bool result = compareHelper(success, failureMsg,
                                      val1 != nullptr ? val1 : "<null>",
                                      val2 != nullptr ? val2 : "<null>",
                                      actual, expected, file, line,
                                      val1 != nullptr && val2 != nullptr);

    // Our caller got these from BOBUIest::toString()
    delete [] val1;
    delete [] val2;

    return result;
}

bool BOBUIestResult::compare(bool success, const char *failureMsg,
                          double val1, double val2,
                          const char *actual, const char *expected,
                          const char *file, int line)
{
    return compareHelper(success, failureMsg, val1, val2, actual, expected, file, line);
}

bool BOBUIestResult::compare(bool success, const char *failureMsg,
                          float val1, float val2,
                          const char *actual, const char *expected,
                          const char *file, int line)
{
    return compareHelper(success, failureMsg, val1, val2, actual, expected, file, line);
}

bool BOBUIestResult::compare(bool success, const char *failureMsg,
                          int val1, int val2,
                          const char *actual, const char *expected,
                          const char *file, int line)
{
    return compareHelper(success, failureMsg, val1, val2, actual, expected, file, line);
}

#if BOBUI_POINTER_SIZE == 8
bool BOBUIestResult::compare(bool success, const char *failureMsg,
                          qsizetype val1, qsizetype val2,
                          const char *actual, const char *expected,
                          const char *file, int line)
{
    return compareHelper(success, failureMsg, val1, val2, actual, expected, file, line);
}
#endif // BOBUI_POINTER_SIZE == 8

bool BOBUIestResult::compare(bool success, const char *failureMsg,
                          unsigned val1, unsigned val2,
                          const char *actual, const char *expected,
                          const char *file, int line)
{
    return compareHelper(success, failureMsg, val1, val2, actual, expected, file, line);
}

bool BOBUIestResult::compare(bool success, const char *failureMsg,
                          QStringView val1, QStringView val2,
                          const char *actual, const char *expected,
                          const char *file, int line)
{
    return compareHelper(success, failureMsg, val1, val2, actual, expected, file, line);
}

bool BOBUIestResult::compare(bool success, const char *failureMsg,
                          QStringView val1, const QLatin1StringView &val2,
                          const char *actual, const char *expected,
                          const char *file, int line)
{
    return compareHelper(success, failureMsg, val1, val2, actual, expected, file, line);
}

bool BOBUIestResult::compare(bool success, const char *failureMsg,
                          const QLatin1StringView & val1, QStringView val2,
                          const char *actual, const char *expected,
                          const char *file, int line)
{
    return compareHelper(success, failureMsg, val1, val2, actual, expected, file, line);
}

// Simplified version of compare() that does not take the values, because they
// can't be converted to strings (or the user didn't want to do that).
bool BOBUIestResult::compare(bool success, const char *failureMsg,
                          const char *actual, const char *expeceted,
                          const char *file, int line)
{
    return compareHelper(success, failureMsg, actual, expeceted, file, line);
}

void BOBUIestResult::addFailure(const char *message, const char *file, int line)
{
    clearExpectFail();
    if (qApp && BOBUIhread::currentThread() == qApp->thread())
        BOBUIestEventLoop::instance().exitLoop();

    if (BOBUIest::blacklistCurrentTest)
        BOBUIestLog::addBFail(message, file, line);
    else
        BOBUIestLog::addFail(message, file, line);
    BOBUIest::setFailed(true);
}

void BOBUIestResult::addSkip(const char *message, const char *file, int line)
{
    clearExpectFail();

    BOBUIestLog::addSkip(message, file, line);
}

void BOBUIestResult::setCurrentTestObject(const char *name)
{
    BOBUIest::currentTestObjectName = name;
}

const char *BOBUIestResult::currentTestObjectName()
{
    return BOBUIest::currentTestObjectName ? BOBUIest::currentTestObjectName : "";
}

void BOBUIestResult::setSkipCurrentTest(bool value)
{
    BOBUIest::skipCurrentTest = value;
}

bool BOBUIestResult::skipCurrentTest()
{
    return BOBUIest::skipCurrentTest;
}

void BOBUIestResult::setCurrentAppName(const char *appName)
{
    ::currentAppName = appName;
}

const char *BOBUIestResult::currentAppName()
{
    return ::currentAppName;
}

static const char *macroNameForOp(BOBUIest::ComparisonOperation op)
{
    using namespace BOBUIest;
    switch (op) {
    case ComparisonOperation::CustomCompare:
        return "QCOMPARE"; /* not used */
    case ComparisonOperation::Equal:
        return "QCOMPARE_EQ";
    case ComparisonOperation::NotEqual:
        return "QCOMPARE_NE";
    case ComparisonOperation::LessThan:
        return "QCOMPARE_LT";
    case ComparisonOperation::LessThanOrEqual:
        return "QCOMPARE_LE";
    case ComparisonOperation::GreaterThan:
        return "QCOMPARE_GT";
    case ComparisonOperation::GreaterThanOrEqual:
        return "QCOMPARE_GE";
    case ComparisonOperation::ThreeWayCompare:
        return "QCOMPARE_3WAY";
    }
    Q_UNREACHABLE_RETURN("");
}

static const char *failureMessageForOp(BOBUIest::ComparisonOperation op)
{
    using namespace BOBUIest;
    switch (op) {
    case ComparisonOperation::CustomCompare:
        return "Compared values are not the same"; /* not used */
    case ComparisonOperation::ThreeWayCompare:
        return "The result of operator<=>() is not what was expected";
    case ComparisonOperation::Equal:
        return "The computed value is expected to be equal to the baseline, but is not";
    case ComparisonOperation::NotEqual:
        return "The computed value is expected to be different from the baseline, but is not";
    case ComparisonOperation::LessThan:
        return "The computed value is expected to be less than the baseline, but is not";
    case ComparisonOperation::LessThanOrEqual:
        return "The computed value is expected to be less than or equal to the baseline, but is not";
    case ComparisonOperation::GreaterThan:
        return "The computed value is expected to be greater than the baseline, but is not";
    case ComparisonOperation::GreaterThanOrEqual:
        return "The computed value is expected to be greater than or equal to the baseline, but is not";
    }
    Q_UNREACHABLE_RETURN("");
}

bool BOBUIestResult::reportResult(bool success, const void *lhs, const void *rhs,
                               const char *(*lhsFormatter)(const void*),
                               const char *(*rhsFormatter)(const void*),
                               const char *lhsExpr, const char *rhsExpr,
                               BOBUIest::ComparisonOperation op, const char *file, int line,
                               const char *failureMessage)
{
    Q_DECL_UNINITIALIZED char msg[maxMsgLen];
    msg[0] = '\0';

    BOBUIEST_ASSERT(lhsExpr);
    BOBUIEST_ASSERT(rhsExpr);

    if (BOBUIestLog::verboseLevel() >= 2) {
        std::snprintf(msg, maxMsgLen, "%s(%s, %s)", macroNameForOp(op), lhsExpr, rhsExpr);
        BOBUIestLog::info(msg, file, line);
    }

    if (success) {
        if (BOBUIest::expectFailMode) {
            std::snprintf(msg, maxMsgLen, "%s(%s, %s) returned TRUE unexpectedly.",
                          macroNameForOp(op), lhsExpr, rhsExpr);
        }
        return checkStatement(success, msg, file, line);
    }

    const std::unique_ptr<const char[]> lhsPtr{ lhsFormatter(lhs) };
    const std::unique_ptr<const char[]> rhsPtr{ rhsFormatter(rhs) };

    if (!failureMessage)
        failureMessage = failureMessageForOp(op);

    formatFailMessage(msg, maxMsgLen, failureMessage, lhsPtr.get(), rhsPtr.get(),
                      lhsExpr, rhsExpr, op);

    return checkStatement(success, msg, file, line);
}

bool BOBUIestResult::report3WayResult(bool success,
                                   const char *failureMessage,
                                   const void *lhs, const void *rhs,
                                   const char *(*lhsFormatter)(const void*),
                                   const char *(*rhsFormatter)(const void*),
                                   const char *lhsExpression, const char *rhsExpression,
                                   const char *(*actualOrderFormatter)(const void *),
                                   const char *(*expectedOrderFormatter)(const void *),
                                   const void *actualOrder, const void *expectedOrder,
                                   const char *expectedExpression,
                                   const char *file, int line)
{
    char msg[maxMsgLen];
    msg[0] = '\0';

    BOBUIEST_ASSERT(lhsExpression);
    BOBUIEST_ASSERT(rhsExpression);
    BOBUIEST_ASSERT(expectedExpression);
    const char *macroName = macroNameForOp(BOBUIest::ComparisonOperation::ThreeWayCompare);
    const std::string actualExpression = std::string(lhsExpression) + " <=> " + rhsExpression;

    if (BOBUIestLog::verboseLevel() >= 2) {
        std::snprintf(msg, maxMsgLen, "%s(%s, %s, %s)",
                      macroName, lhsExpression, rhsExpression, expectedExpression);
        BOBUIestLog::info(msg, file, line);
    }

    if (success) {
        if (BOBUIest::expectFailMode) {
            std::snprintf(msg, maxMsgLen, "%s(%s, %s, %s) returned TRUE unexpectedly.",
                          macroName, lhsExpression, rhsExpression, expectedExpression);
        }
        return checkStatement(success, msg, file, line);
    }
    const std::unique_ptr<const char[]> lhsStr{lhsFormatter(lhs)};
    const std::unique_ptr<const char[]> rhsStr{rhsFormatter(rhs)};

    const std::unique_ptr<const char[]> actual{actualOrderFormatter(actualOrder)};
    const std::unique_ptr<const char[]> expected{expectedOrderFormatter(expectedOrder)};

    if (!failureMessage)
        failureMessage = failureMessageForOp(BOBUIest::ComparisonOperation::ThreeWayCompare);

    // Left and Right compared parameters of QCOMPARE_3WAY
    formatFailMessage(msg, maxMsgLen, failureMessage,
                      lhsStr.get(), rhsStr.get(),
                      lhsExpression, rhsExpression,
                      BOBUIest::ComparisonOperation::ThreeWayCompare);

    // Actual and Expected results of comparison
    formatFailMessage(msg + strlen(msg), maxMsgLen - strlen(msg), "",
                      actual.get(), expected.get(),
                      actualExpression.c_str(), expectedExpression,
                      BOBUIest::ComparisonOperation::CustomCompare);

    return checkStatement(success, msg, file, line);
}

BOBUI_END_NAMESPACE
