// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTCASE_H
#define BOBUIESTCASE_H

#include <BobUITest/bobuitestglobal.h>
#include <BobUITest/bobuiesttostring.h>

#include <BobUICore/qstring.h>
#include <BobUICore/qnamespace.h>
#include <BobUICore/qmetatype.h>
#include <BobUICore/qmetaobject.h>
#include <BobUICore/qsharedpointer.h>
#include <BobUICore/bobuiemporarydir.h>
#include <BobUICore/bobuihread.h>

#include <chrono>
#ifdef __cpp_concepts
#include <concepts>
#endif
#include <BobUICore/qxpfunctional.h>
#include <BobUICore/qxptype_traits.h>
#include <BobUICore/q20utility.h>

#include <string.h>

#ifndef BOBUI_NO_EXCEPTIONS
#  include <exception>
#endif // BOBUI_NO_EXCEPTIONS

BOBUI_BEGIN_NAMESPACE

#ifndef BOBUI_NO_EXCEPTIONS

#ifdef BOBUIEST_THROW_ON_FAIL
# define BOBUIEST_FAIL_ACTION BOBUIest::Internal::throwOnFail()
#else
# define BOBUIEST_FAIL_ACTION do { BOBUIest::Internal::maybeThrowOnFail(); return; } while (false)
#endif

#ifdef BOBUIEST_THROW_ON_SKIP
# define BOBUIEST_SKIP_ACTION BOBUIest::Internal::throwOnSkip()
#else
# define BOBUIEST_SKIP_ACTION do { BOBUIest::Internal::maybeThrowOnSkip(); return; } while (false)
#endif

#else
# if defined(BOBUIEST_THROW_ON_FAIL) || defined(BOBUIEST_THROW_ON_SKIP)
#  error BOBUIEST_THROW_ON_FAIL/SKIP require exception support enabled.
# endif
#endif // BOBUI_NO_EXCEPTIONS

#ifndef BOBUIEST_FAIL_ACTION
# define BOBUIEST_FAIL_ACTION return
#endif

#ifndef BOBUIEST_SKIP_ACTION
# define BOBUIEST_SKIP_ACTION return
#endif

class qfloat16;
class QRegularExpression;

#define QVERIFY(statement) \
do {\
    if (!BOBUIest::qVerify(static_cast<bool>(statement), #statement, "", __FILE__, __LINE__))\
        BOBUIEST_FAIL_ACTION; \
} while (false)

#define QFAIL(message) \
do {\
    BOBUIest::qFail(static_cast<const char *>(message), __FILE__, __LINE__);\
    BOBUIEST_FAIL_ACTION; \
} while (false)

#define QVERIFY2(statement, description) \
do {\
    if (statement) {\
        if (!BOBUIest::qVerify(true, #statement, static_cast<const char *>(description), __FILE__, __LINE__))\
            BOBUIEST_FAIL_ACTION; \
    } else {\
        if (!BOBUIest::qVerify(false, #statement, static_cast<const char *>(description), __FILE__, __LINE__))\
            BOBUIEST_FAIL_ACTION; \
    }\
} while (false)

#define QCOMPARE(actual, expected) \
do {\
    if (!BOBUIest::qCompare(actual, expected, #actual, #expected, __FILE__, __LINE__))\
        BOBUIEST_FAIL_ACTION; \
} while (false)

#define QCOMPARE_OP_IMPL(lhs, rhs, op, opId) \
do { \
    if (!BOBUIest::qCompareOp<BOBUIest::ComparisonOperation::opId>(lhs, rhs, #lhs, #rhs, __FILE__, __LINE__)) \
        BOBUIEST_FAIL_ACTION; \
} while (false)

#define QCOMPARE_EQ(computed, baseline) QCOMPARE_OP_IMPL(computed, baseline, ==, Equal)
#define QCOMPARE_NE(computed, baseline) QCOMPARE_OP_IMPL(computed, baseline, !=, NotEqual)
#define QCOMPARE_LT(computed, baseline) QCOMPARE_OP_IMPL(computed, baseline, <, LessThan)
#define QCOMPARE_LE(computed, baseline) QCOMPARE_OP_IMPL(computed, baseline, <=, LessThanOrEqual)
#define QCOMPARE_GT(computed, baseline) QCOMPARE_OP_IMPL(computed, baseline, >, GreaterThan)
#define QCOMPARE_GE(computed, baseline) QCOMPARE_OP_IMPL(computed, baseline, >=, GreaterThanOrEqual)

#ifndef BOBUI_NO_EXCEPTIONS

#  define QVERIFY_THROWS_NO_EXCEPTION(...) \
    do { \
        BOBUI_TRY { \
            __VA_ARGS__; \
            /* success */ \
        } BOBUI_CATCH (...) { \
            BOBUIest::qCaught(nullptr, __FILE__, __LINE__); \
            BOBUIEST_FAIL_ACTION; \
        } \
    } while (false) \
    /* end */

#if BOBUI_DEPRECATED_SINCE(6, 3)
namespace BOBUIest {
BOBUI_DEPRECATED_VERSION_X_6_3("Don't use QVERIFY_EXCEPTION_THROWN(expr, type) anymore, "
                            "use QVERIFY_THROWS_EXCEPTION(type, expr...) instead")
inline void useVerifyThrowsException() {}
} // namespace BOBUIest
#  define QVERIFY_EXCEPTION_THROWN(expression, exceptiontype) \
    QVERIFY_THROWS_EXCEPTION(exceptiontype, BOBUIest::useVerifyThrowsException(); expression)
#endif

#  define QVERIFY_THROWS_EXCEPTION(exceptiontype, ...) \
    do {\
        bool qverify_throws_exception_did_not_throw = false; \
        BOBUI_TRY {\
            __VA_ARGS__; \
            BOBUIest::qFail("Expected exception of type " #exceptiontype " to be thrown" \
                         " but no exception caught", __FILE__, __LINE__); \
            qverify_throws_exception_did_not_throw = true; \
        } BOBUI_CATCH (const exceptiontype &) { \
            /* success */ \
        } BOBUI_CATCH (...) {\
            BOBUIest::qCaught(#exceptiontype, __FILE__, __LINE__); \
            BOBUIEST_FAIL_ACTION; \
        }\
        if (qverify_throws_exception_did_not_throw) \
            BOBUIEST_FAIL_ACTION; \
    } while (false)

#else // BOBUI_NO_EXCEPTIONS

/*
 * These macros check whether the expression passed throws exceptions, but we can't
 * catch them to check because BobUI has been compiled without exception support. We can't
 * skip the expression because it may have side effects and must be executed.
 * So, users must use BobUI with exception support enabled if they use exceptions
 * in their code.
 */
#  define QVERIFY_THROWS_EXCEPTION(...) \
    static_assert(false, "Support for exceptions is disabled")
#  define QVERIFY_THROWS_NO_EXCEPTION(...) \
    static_assert(false, "Support for exceptions is disabled")

#endif // !BOBUI_NO_EXCEPTIONS

/* Ideally we would adapt qWaitFor(), or a variant on it, to implement roughly
 * what the following provides as BOBUIRY_LOOP_IMPL(); however, for now, the
 * reporting of how much to increase the timeout to (if within a factor of two)
 * on failure and the check for (BOBUIest::runningTest() &&
 * BOBUIest::currentTestResolved()) go beyond qWaitFor(). (We no longer care about
 * the bug in MSVC < 2017 that precluded using qWaitFor() in the implementation
 * here, see BOBUIBUG-59096.)
 */

// NB: not do {...} while (0) wrapped, as bobui_test_i is accessed after it
#define BOBUIRY_LOOP_IMPL(expr, timeoutValue, step) \
    if (!(expr)) { \
        BOBUIest::qWait(0); \
    } \
    std::chrono::milliseconds timeoutValueMs(timeoutValue); \
    std::chrono::milliseconds stepMs(step); \
    auto bobui_test_i = std::chrono::milliseconds(0); \
    for (; bobui_test_i < timeoutValueMs && !(BOBUIest::runningTest() && BOBUIest::currentTestResolved()) \
             && !(expr); bobui_test_i += stepMs) { \
        BOBUIest::qWait(stepMs); \
    }
// Ends in a for-block, so doesn't want a following semicolon.

#define BOBUIRY_TIMEOUT_DEBUG_IMPL(expr, timeoutValue, step) \
    if (!(BOBUIest::runningTest() && BOBUIest::currentTestResolved()) && !(expr)) { \
        BOBUIRY_LOOP_IMPL(expr, 2 * (timeoutValue), step) \
        if ((expr)) { \
            QFAIL(qPrintable(BOBUIest::Internal::formatTryTimeoutDebugMessage(\
                                 u8"" #expr, timeoutValue, timeoutValue + bobui_test_i))); \
        } \
    }

#define BOBUIRY_IMPL(expr, timeoutAsGiven)\
    const auto bobui_test_timeoutAsMs = [&] { \
            /* make 5s work w/o user action: */ \
            using namespace std::chrono_literals; \
            return std::chrono::milliseconds{timeoutAsGiven}; \
        }(); \
    const auto bobui_test_step = bobui_test_timeoutAsMs < std::chrono::milliseconds(350) \
                              ? bobui_test_timeoutAsMs / 7 + std::chrono::milliseconds(1) \
                              : std::chrono::milliseconds(50); \
    { BOBUIRY_LOOP_IMPL(expr, bobui_test_timeoutAsMs, bobui_test_step) } \
    BOBUIRY_TIMEOUT_DEBUG_IMPL(expr, bobui_test_timeoutAsMs, bobui_test_step)
// Ends with an if-block, so doesn't want a following semicolon.

// Will try to wait for the expression to become true while allowing event processing
#define BOBUIRY_VERIFY_WITH_TIMEOUT(expr, timeout) \
do { \
    BOBUIRY_IMPL(expr, timeout) \
    QVERIFY(expr); \
} while (false)

#define BOBUIRY_VERIFY(expr) BOBUIRY_VERIFY_WITH_TIMEOUT( \
    expr, BOBUIest::defaultTryTimeout.load(std::memory_order_relaxed))

// Will try to wait for the expression to become true while allowing event processing
#define BOBUIRY_VERIFY2_WITH_TIMEOUT(expr, messageExpression, timeout) \
do { \
    BOBUIRY_IMPL(expr, timeout) \
    QVERIFY2(expr, messageExpression); \
} while (false)

#define BOBUIRY_VERIFY2(expr, messageExpression) \
    BOBUIRY_VERIFY2_WITH_TIMEOUT(expr, messageExpression, \
        BOBUIest::defaultTryTimeout.load(std::memory_order_relaxed))

// Will try to wait for the comparison to become successful while allowing event processing
#define BOBUIRY_COMPARE_WITH_TIMEOUT(expr, expected, timeout) \
do { \
    BOBUIRY_IMPL((expr) == (expected), timeout) \
    QCOMPARE(expr, expected); \
} while (false)

#define BOBUIRY_COMPARE(expr, expected) \
    BOBUIRY_COMPARE_WITH_TIMEOUT(expr, expected, \
        BOBUIest::defaultTryTimeout.load(std::memory_order_relaxed))

#define BOBUIRY_COMPARE_OP_WITH_TIMEOUT_IMPL(computed, baseline, op, opId, timeout) \
do { \
    using Q_Cmp = BOBUIest::Internal::Compare<BOBUIest::ComparisonOperation::opId>; \
    BOBUIRY_IMPL(Q_Cmp::compare((computed), (baseline)), timeout) \
    QCOMPARE_OP_IMPL(computed, baseline, op, opId); \
} while (false)

#define BOBUIRY_COMPARE_EQ_WITH_TIMEOUT(computed, baseline, timeout) \
    BOBUIRY_COMPARE_OP_WITH_TIMEOUT_IMPL(computed, baseline, ==, Equal, timeout)

#define BOBUIRY_COMPARE_EQ(computed, baseline) \
    BOBUIRY_COMPARE_EQ_WITH_TIMEOUT(computed, baseline, \
        BOBUIest::defaultTryTimeout.load(std::memory_order_relaxed))

#define BOBUIRY_COMPARE_NE_WITH_TIMEOUT(computed, baseline, timeout) \
    BOBUIRY_COMPARE_OP_WITH_TIMEOUT_IMPL(computed, baseline, !=, NotEqual, timeout)

#define BOBUIRY_COMPARE_NE(computed, baseline) \
    BOBUIRY_COMPARE_NE_WITH_TIMEOUT(computed, baseline, \
        BOBUIest::defaultTryTimeout.load(std::memory_order_relaxed))

#define BOBUIRY_COMPARE_LT_WITH_TIMEOUT(computed, baseline, timeout) \
    BOBUIRY_COMPARE_OP_WITH_TIMEOUT_IMPL(computed, baseline, <, LessThan, timeout)

#define BOBUIRY_COMPARE_LT(computed, baseline) \
    BOBUIRY_COMPARE_LT_WITH_TIMEOUT(computed, baseline, \
        BOBUIest::defaultTryTimeout.load(std::memory_order_relaxed))

#define BOBUIRY_COMPARE_LE_WITH_TIMEOUT(computed, baseline, timeout) \
    BOBUIRY_COMPARE_OP_WITH_TIMEOUT_IMPL(computed, baseline, <=, LessThanOrEqual, timeout)

#define BOBUIRY_COMPARE_LE(computed, baseline) \
    BOBUIRY_COMPARE_LE_WITH_TIMEOUT(computed, baseline, \
        BOBUIest::defaultTryTimeout.load(std::memory_order_relaxed))

#define BOBUIRY_COMPARE_GT_WITH_TIMEOUT(computed, baseline, timeout) \
    BOBUIRY_COMPARE_OP_WITH_TIMEOUT_IMPL(computed, baseline, >, GreaterThan, timeout)

#define BOBUIRY_COMPARE_GT(computed, baseline) \
    BOBUIRY_COMPARE_GT_WITH_TIMEOUT(computed, baseline, \
        BOBUIest::defaultTryTimeout.load(std::memory_order_relaxed))

#define BOBUIRY_COMPARE_GE_WITH_TIMEOUT(computed, baseline, timeout) \
    BOBUIRY_COMPARE_OP_WITH_TIMEOUT_IMPL(computed, baseline, >=, GreaterThanOrEqual, timeout)

#define BOBUIRY_COMPARE_GE(computed, baseline) \
    BOBUIRY_COMPARE_GE_WITH_TIMEOUT(computed, baseline, \
        BOBUIest::defaultTryTimeout.load(std::memory_order_relaxed))

#define QSKIP_INTERNAL(statement) \
do {\
    BOBUIest::qSkip(static_cast<const char *>(statement), __FILE__, __LINE__);\
    BOBUIEST_SKIP_ACTION; \
} while (false)

#define QSKIP(statement, ...) QSKIP_INTERNAL(statement)

#define QEXPECT_FAIL(dataIndex, comment, mode)\
do {\
    if (!BOBUIest::qExpectFail(dataIndex, static_cast<const char *>(comment), BOBUIest::mode, __FILE__, __LINE__))\
        BOBUIEST_FAIL_ACTION; \
} while (false)

#define QFETCH(Type, name)\
    Type name = *static_cast<Type *>(BOBUIest::qData(#name, ::qMetaTypeId<typename std::remove_cv<Type >::type>()))

#define QFETCH_GLOBAL(Type, name)\
    Type name = *static_cast<Type *>(BOBUIest::qGlobalData(#name, ::qMetaTypeId<typename std::remove_cv<Type >::type>()))

#define BOBUIEST(actual, testElement)\
do {\
    if (!BOBUIest::qTest(actual, testElement, #actual, #testElement, __FILE__, __LINE__))\
        BOBUIEST_FAIL_ACTION; \
} while (false)

#ifdef __cpp_lib_three_way_comparison
#define QCOMPARE_3WAY(lhs, rhs, order) \
do { \
        if (!BOBUIest::qCompare3Way(lhs, rhs, order, #lhs, #rhs, #order, __FILE__, __LINE__)) \
            BOBUIEST_FAIL_ACTION; \
} while (false)
#else
#define QCOMPARE_3WAY(...) \
    static_assert(false, "QCOMPARE_3WAY test requires C++20 operator<=>()")
#endif // __cpp_lib_three_way_comparison

#ifdef BOBUI_TESTCASE_BUILDDIR

#ifndef BOBUI_TESTCASE_SOURCEDIR
#define BOBUI_TESTCASE_SOURCEDIR nullptr
#endif

# define QFINDTESTDATA(basepath)\
    BOBUIest::qFindTestData(basepath, __FILE__, __LINE__, BOBUI_TESTCASE_BUILDDIR, BOBUI_TESTCASE_SOURCEDIR)
#else
# define QFINDTESTDATA(basepath)\
    BOBUIest::qFindTestData(basepath, __FILE__, __LINE__)
#endif

# define QEXTRACTTESTDATA(resourcePath) \
    BOBUIest::qExtractTestData(resourcePath)

class QObject;
class BOBUIestData;

namespace BOBUIest
{
    namespace Internal {

    [[noreturn]] Q_TESTLIB_EXPORT void throwOnFail();
    [[noreturn]] Q_TESTLIB_EXPORT void throwOnSkip();
    Q_TESTLIB_EXPORT void maybeThrowOnFail();
    Q_TESTLIB_EXPORT void maybeThrowOnSkip();

    Q_DECL_COLD_FUNCTION
    Q_TESTLIB_EXPORT QString formatTryTimeoutDebugMessage(q_no_char8_t::QUtf8StringView expr,
                                                          std::chrono::milliseconds timeout,
                                                          std::chrono::milliseconds actual);
    Q_TESTLIB_EXPORT Q_DECL_COLD_FUNCTION
    const char *formatPropertyTestHelperFailure(char *msg, size_t maxMsgLen,
                                                const char *actual, const char *expected,
                                                const char *actualExpr,
                                                const char *expectedExpr);

    template <ComparisonOperation> struct Compare;
    template <> struct Compare<ComparisonOperation::Equal>
    {
        template <typename T1, typename T2> static bool compare(T1 &&lhs, T2 &&rhs)
        { return std::forward<T1>(lhs) == std::forward<T2>(rhs); }
    };
    template <> struct Compare<ComparisonOperation::NotEqual>
    {
        template <typename T1, typename T2> static bool compare(T1 &&lhs, T2 &&rhs)
        { return std::forward<T1>(lhs) != std::forward<T2>(rhs); }
    };
    template <> struct Compare<ComparisonOperation::LessThan>
    {
        template <typename T1, typename T2> static bool compare(T1 &&lhs, T2 &&rhs)
        { return std::forward<T1>(lhs) < std::forward<T2>(rhs); }
    };
    template <> struct Compare<ComparisonOperation::LessThanOrEqual>
    {
        template <typename T1, typename T2> static bool compare(T1 &&lhs, T2 &&rhs)
        { return std::forward<T1>(lhs) <= std::forward<T2>(rhs); }
    };
    template <> struct Compare<ComparisonOperation::GreaterThan>
    {
        template <typename T1, typename T2> static bool compare(T1 &&lhs, T2 &&rhs)
        { return std::forward<T1>(lhs) > std::forward<T2>(rhs); }
    };
    template <> struct Compare<ComparisonOperation::GreaterThanOrEqual>
    {
        template <typename T1, typename T2> static bool compare(T1 &&lhs, T2 &&rhs)
        { return std::forward<T1>(lhs) >= std::forward<T2>(rhs); }
    };

    template <typename T1> const char *genericToString(const void *arg)
    {
        using BOBUIest::toString;
        return toString(*static_cast<const T1 *>(arg));
    }

    template <> inline const char *genericToString<char *>(const void *arg)
    {
        using BOBUIest::toString;
        return toString(static_cast<const char *>(arg));
    }

    template <> inline const char *genericToString<std::nullptr_t>(const void *)
    {
        return BOBUIest::toString(nullptr);
    }

    template <typename T> const char *pointerToString(const void *arg)
    {
        using BOBUIest::toString;
        return toString(static_cast<const T *>(arg));
    }

    // Exported so BobUI Quick Test can also use it for generating backtraces upon crashes.
    Q_TESTLIB_EXPORT extern bool noCrashHandler;

    } // namespace Internal

    Q_TESTLIB_EXPORT void qInit(QObject *testObject, int argc = 0, char **argv = nullptr);
    Q_TESTLIB_EXPORT int qRun();
    Q_TESTLIB_EXPORT void qCleanup();

    Q_TESTLIB_EXPORT int qExec(QObject *testObject, int argc = 0, char **argv = nullptr);
    Q_TESTLIB_EXPORT int qExec(QObject *testObject, const QStringList &arguments);

#if BOBUI_CONFIG(batch_test_support) || defined(Q_QDOC)
    using TestEntryFunction = std::function<int(int, char **)>;
    Q_TESTLIB_EXPORT void qRegisterTestCase(const QString &name, TestEntryFunction entryFunction);
#endif  // BOBUI_CONFIG(batch_test_support)

    Q_TESTLIB_EXPORT void setMainSourcePath(const char *file, const char *builddir = nullptr);
    Q_TESTLIB_EXPORT void setThrowOnFail(bool enable) noexcept;
    Q_TESTLIB_EXPORT void setThrowOnSkip(bool enable) noexcept;

    class ThrowOnFailEnabler {
        Q_DISABLE_COPY_MOVE(ThrowOnFailEnabler)
    public:
        ThrowOnFailEnabler() { setThrowOnFail(true); }
        ~ThrowOnFailEnabler() { setThrowOnFail(false); }
    };

    class ThrowOnSkipEnabler {
        Q_DISABLE_COPY_MOVE(ThrowOnSkipEnabler)
    public:
        ThrowOnSkipEnabler() { setThrowOnSkip(true); }
        ~ThrowOnSkipEnabler() { setThrowOnSkip(false); }
    };

    class ThrowOnFailDisabler {
        Q_DISABLE_COPY_MOVE(ThrowOnFailDisabler)
    public:
        ThrowOnFailDisabler() { setThrowOnFail(false); }
        ~ThrowOnFailDisabler() { setThrowOnFail(true); }
    };

    class ThrowOnSkipDisabler {
        Q_DISABLE_COPY_MOVE(ThrowOnSkipDisabler)
    public:
        ThrowOnSkipDisabler() { setThrowOnSkip(false); }
        ~ThrowOnSkipDisabler() { setThrowOnSkip(true); }
    };

    Q_TESTLIB_EXPORT bool qVerify(bool statement, const char *statementStr, const char *description,
                                 const char *file, int line);
    Q_DECL_COLD_FUNCTION
    Q_TESTLIB_EXPORT void qFail(const char *message, const char *file, int line);
    Q_TESTLIB_EXPORT void qSkip(const char *message, const char *file, int line);
    Q_TESTLIB_EXPORT bool qExpectFail(const char *dataIndex, const char *comment, TestFailMode mode,
                           const char *file, int line);
    Q_DECL_COLD_FUNCTION
    Q_TESTLIB_EXPORT void qCaught(const char *expected, const char *what, const char *file, int line);
    Q_DECL_COLD_FUNCTION
    Q_TESTLIB_EXPORT void qCaught(const char *expected, const char *file, int line);
#if BOBUI_DEPRECATED_SINCE(6, 3)
    BOBUI_DEPRECATED_VERSION_X_6_3("Use qWarning() instead")
    Q_TESTLIB_EXPORT void qWarn(const char *message, const char *file = nullptr, int line = 0);
#endif
    Q_TESTLIB_EXPORT void ignoreMessage(BobUIMsgType type, const char *message);
#if BOBUI_CONFIG(regularexpression)
    Q_TESTLIB_EXPORT void ignoreMessage(BobUIMsgType type, const QRegularExpression &messagePattern);
#endif
    Q_TESTLIB_EXPORT void failOnWarning();
    Q_TESTLIB_EXPORT void failOnWarning(const char *message);
#if BOBUI_CONFIG(regularexpression)
    Q_TESTLIB_EXPORT void failOnWarning(const QRegularExpression &messagePattern);
#endif

#if BOBUI_CONFIG(temporaryfile)
    Q_TESTLIB_EXPORT QSharedPointer<BOBUIemporaryDir> qExtractTestData(const QString &dirName);
#endif
    Q_TESTLIB_EXPORT QString qFindTestData(const char* basepath, const char* file = nullptr, int line = 0, const char* builddir = nullptr, const char* sourcedir = nullptr);
    Q_TESTLIB_EXPORT QString qFindTestData(const QString& basepath, const char* file = nullptr, int line = 0, const char* builddir = nullptr, const char *sourcedir = nullptr);

    Q_TESTLIB_EXPORT void *qData(const char *tagName, int typeId);
    Q_TESTLIB_EXPORT void *qGlobalData(const char *tagName, int typeId);
    Q_TESTLIB_EXPORT void *qElementData(const char *elementName, int metaTypeId);
    Q_TESTLIB_EXPORT QObject *testObject();

    Q_TESTLIB_EXPORT const char *currentAppName();

    Q_TESTLIB_EXPORT const char *currentTestFunction();
    Q_TESTLIB_EXPORT const char *currentDataTag();
    Q_TESTLIB_EXPORT const char *currentGlobalDataTag();
    Q_TESTLIB_EXPORT bool currentTestFailed();
    Q_TESTLIB_EXPORT bool currentTestResolved();
    Q_TESTLIB_EXPORT bool runningTest(); // Internal, for use by macros and BOBUIestEventLoop.

    Q_TESTLIB_EXPORT BobUI::Key asciiToKey(char ascii);
    Q_TESTLIB_EXPORT char keyToAscii(BobUI::Key key);

#if BOBUI_DEPRECATED_SINCE(6, 4)
    BOBUI_DEPRECATED_VERSION_X_6_4("use an overload that takes a formatter callback, "
                                "or an overload that takes only failure message, if you "
                                "do not need to stringify the values")
    Q_TESTLIB_EXPORT bool compare_helper(bool success, const char *failureMsg,
                                         char *actualVal, char *expectedVal,
                                         const char *actual, const char *expected,
                                         const char *file, int line);
#endif // BOBUI_DEPRECATED_SINCE(6, 4)
#if BOBUI_DEPRECATED_SINCE(6, 8)
    BOBUI_DEPRECATED_VERSION_X_6_8("use an overload that takes a formatter callback, "
                                "or an overload that takes only failure message, if you "
                                "do not need to stringify the values")
    Q_TESTLIB_EXPORT bool compare_helper(bool success, const char *failureMsg,
                                         qxp::function_ref<const char*()> actualVal,
                                         qxp::function_ref<const char*()> expectedVal,
                                         const char *actual, const char *expected,
                                         const char *file, int line);
#endif // BOBUI_DEPRECATED_SINCE(6, 8)
    Q_TESTLIB_EXPORT bool compare_helper(bool success, const char *failureMsg,
                                         const void *actualPtr, const void *expectedPtr,
                                         const char *(*actualFormatter)(const void *),
                                         const char *(*expectedFormatter)(const void *),
                                         const char *actual, const char *expected,
                                         const char *file, int line);
    Q_TESTLIB_EXPORT bool compare_helper(bool success, const char *failureMsg,
                                         const char *actual, const char *expected,
                                         const char *file, int line);

    Q_TESTLIB_EXPORT bool compare_3way_helper(bool success, const char *failureMsg,
                                              const void *lhsPtr, const void *rhsPtr,
                                              const char *(*lhsFormatter)(const void*),
                                              const char *(*rhsFormatter)(const void*),
                                              const char *lhsStr, const char *rhsStr,
                                              const char *(*actualOrderFormatter)(const void *),
                                              const char *(*expectedOrderFormatter)(const void *),
                                              const void *actualOrderPtr,
                                              const void *expectedOrderPtr,
                                              const char *expectedExpression,
                                              const char *file, int line);

    Q_TESTLIB_EXPORT void addColumnInternal(int id, const char *name);

    template <typename T>
    inline void addColumn(const char *name, T * = nullptr)
    {
        using QIsSameTConstChar = std::is_same<T, const char*>;
        static_assert(!QIsSameTConstChar::value, "const char* is not allowed as a test data format.");
        addColumnInternal(qMetaTypeId<T>(), name);
    }
    Q_TESTLIB_EXPORT BOBUIestData &newRow(const char *dataTag);
    Q_TESTLIB_EXPORT BOBUIestData &addRow(const char *format, ...) Q_ATTRIBUTE_FORMAT_PRINTF(1, 2);

    Q_TESTLIB_EXPORT bool qCompare(qfloat16 const &t1, qfloat16 const &t2,
                    const char *actual, const char *expected, const char *file, int line);

    Q_TESTLIB_EXPORT bool qCompare(float const &t1, float const &t2,
                    const char *actual, const char *expected, const char *file, int line);

    Q_TESTLIB_EXPORT bool qCompare(double const &t1, double const &t2,
                    const char *actual, const char *expected, const char *file, int line);

    Q_TESTLIB_EXPORT bool qCompare(int t1, int t2, const char *actual, const char *expected,
                                   const char *file, int line);

#if BOBUI_POINTER_SIZE == 8
    Q_TESTLIB_EXPORT bool qCompare(qsizetype t1, qsizetype t2, const char *actual, const char *expected,
                                   const char *file, int line);
#endif

    Q_TESTLIB_EXPORT bool qCompare(unsigned t1, unsigned t2, const char *actual, const char *expected,
                                   const char *file, int line);

    Q_TESTLIB_EXPORT bool qCompare(QStringView t1, QStringView t2,
                                   const char *actual, const char *expected,
                                   const char *file, int line);
    Q_TESTLIB_EXPORT bool qCompare(QStringView t1, const QLatin1StringView &t2,
                                   const char *actual, const char *expected,
                                   const char *file, int line);
    Q_TESTLIB_EXPORT bool qCompare(const QLatin1StringView &t1, QStringView t2,
                                   const char *actual, const char *expected,
                                   const char *file, int line);
    inline bool qCompare(const QString &t1, const QString &t2,
                         const char *actual, const char *expected,
                         const char *file, int line)
    {
        return qCompare(QStringView(t1), QStringView(t2), actual, expected, file, line);
    }
    inline bool qCompare(const QString &t1, const QLatin1StringView &t2,
                         const char *actual, const char *expected,
                         const char *file, int line)
    {
        return qCompare(QStringView(t1), t2, actual, expected, file, line);
    }
    inline bool qCompare(const QLatin1StringView &t1, const QString &t2,
                         const char *actual, const char *expected,
                         const char *file, int line)
    {
        return qCompare(t1, QStringView(t2), actual, expected, file, line);
    }

    inline bool compare_ptr_helper(const volatile void *t1, const volatile void *t2, const char *actual,
                                   const char *expected, const char *file, int line)
    {
        auto formatter = Internal::pointerToString<void>;
        return compare_helper(t1 == t2, "Compared pointers are not the same",
                              const_cast<const void *>(t1), const_cast<const void *>(t2),
                              formatter, formatter, actual, expected, file, line);
    }

    inline bool compare_ptr_helper(const volatile QObject *t1, const volatile QObject *t2, const char *actual,
                                   const char *expected, const char *file, int line)
    {
        auto formatter = Internal::pointerToString<QObject>;
        return compare_helper(t1 == t2, "Compared QObject pointers are not the same",
                              const_cast<const QObject *>(t1), const_cast<const QObject *>(t2),
                              formatter, formatter, actual, expected, file, line);
    }

    inline bool compare_ptr_helper(const volatile QObject *t1, std::nullptr_t, const char *actual,
                                   const char *expected, const char *file, int line)
    {
        auto lhsFormatter = Internal::pointerToString<QObject>;
        auto rhsFormatter = Internal::genericToString<std::nullptr_t>;
        return compare_helper(t1 == nullptr, "Compared QObject pointers are not the same",
                              const_cast<const QObject *>(t1), nullptr,
                              lhsFormatter, rhsFormatter, actual, expected, file, line);
    }

    inline bool compare_ptr_helper(std::nullptr_t, const volatile QObject *t2, const char *actual,
                                   const char *expected, const char *file, int line)
    {
        auto lhsFormatter = Internal::genericToString<std::nullptr_t>;
        auto rhsFormatter = Internal::pointerToString<QObject>;
        return compare_helper(nullptr == t2, "Compared QObject pointers are not the same",
                              nullptr, const_cast<const QObject *>(t2),
                              lhsFormatter, rhsFormatter, actual, expected, file, line);
    }

    inline bool compare_ptr_helper(const volatile void *t1, std::nullptr_t, const char *actual,
                                   const char *expected, const char *file, int line)
    {
        auto lhsFormatter = Internal::pointerToString<void>;
        auto rhsFormatter = Internal::genericToString<std::nullptr_t>;
        return compare_helper(t1 == nullptr, "Compared pointers are not the same",
                              const_cast<const void *>(t1), nullptr,
                              lhsFormatter, rhsFormatter, actual, expected, file, line);
    }

    inline bool compare_ptr_helper(std::nullptr_t, const volatile void *t2, const char *actual,
                                   const char *expected, const char *file, int line)
    {
        auto lhsFormatter = Internal::genericToString<std::nullptr_t>;
        auto rhsFormatter = Internal::pointerToString<void>;
        return compare_helper(nullptr == t2, "Compared pointers are not the same",
                              nullptr, const_cast<const void *>(t2),
                              lhsFormatter, rhsFormatter, actual, expected, file, line);
    }

    template <typename T1, typename T2 = T1>
    inline bool qCompare(const T1 &t1, const T2 &t2, const char *actual, const char *expected,
                         const char *file, int line)
    {
        using Internal::genericToString;
        if constexpr (BobUIPrivate::is_standard_or_extended_integer_type_v<T1>
                      && BobUIPrivate::is_standard_or_extended_integer_type_v<T2>) {
            return compare_helper(q20::cmp_equal(t1, t2), "Compared values are not the same",
                                  std::addressof(t1), std::addressof(t2),
                                  genericToString<T1>, genericToString<T2>,
                                  actual, expected, file, line);
        } else {
            return compare_helper(t1 == t2, "Compared values are not the same",
                                  std::addressof(t1), std::addressof(t2),
                                  genericToString<T1>, genericToString<T2>,
                                  actual, expected, file, line);
        }
    }

    inline bool qCompare(double const &t1, float const &t2, const char *actual,
                                 const char *expected, const char *file, int line)
    {
        return qCompare(qreal(t1), qreal(t2), actual, expected, file, line);
    }

    inline bool qCompare(float const &t1, double const &t2, const char *actual,
                                 const char *expected, const char *file, int line)
    {
        return qCompare(qreal(t1), qreal(t2), actual, expected, file, line);
    }

    template <typename T>
    inline bool qCompare(const T *t1, const T *t2, const char *actual, const char *expected,
                        const char *file, int line)
    {
        return compare_ptr_helper(t1, t2, actual, expected, file, line);
    }
    template <typename T>
    inline bool qCompare(T *t1, T *t2, const char *actual, const char *expected,
                        const char *file, int line)
    {
        return compare_ptr_helper(t1, t2, actual, expected, file, line);
    }

    template <typename T>
    inline bool qCompare(T *t1, std::nullptr_t, const char *actual, const char *expected,
                        const char *file, int line)
    {
        return compare_ptr_helper(t1, nullptr, actual, expected, file, line);
    }
    template <typename T>
    inline bool qCompare(std::nullptr_t, T *t2, const char *actual, const char *expected,
                        const char *file, int line)
    {
        return compare_ptr_helper(nullptr, t2, actual, expected, file, line);
    }

    template <typename T1, typename T2>
    inline bool qCompare(const T1 *t1, const T2 *t2, const char *actual, const char *expected,
                        const char *file, int line)
    {
        return compare_ptr_helper(t1, static_cast<const T1 *>(t2), actual, expected, file, line);
    }
    template <typename T1, typename T2>
    inline bool qCompare(T1 *t1, T2 *t2, const char *actual, const char *expected,
                        const char *file, int line)
    {
        return compare_ptr_helper(const_cast<const T1 *>(t1),
                static_cast<const T1 *>(const_cast<const T2 *>(t2)), actual, expected, file, line);
    }
    inline bool qCompare(const char *t1, const char *t2, const char *actual,
                                       const char *expected, const char *file, int line)
    {
        return compare_string_helper(t1, t2, actual, expected, file, line);
    }
    inline bool qCompare(char *t1, char *t2, const char *actual, const char *expected,
                        const char *file, int line)
    {
        return compare_string_helper(t1, t2, actual, expected, file, line);
    }

    /* The next two overloads are for MSVC that shows problems with implicit
       conversions
     */
    inline bool qCompare(char *t1, const char *t2, const char *actual,
                         const char *expected, const char *file, int line)
    {
        return compare_string_helper(t1, t2, actual, expected, file, line);
    }
    inline bool qCompare(const char *t1, char *t2, const char *actual,
                         const char *expected, const char *file, int line)
    {
        return compare_string_helper(t1, t2, actual, expected, file, line);
    }

    template <class T>
    inline bool qTest(const T& actual, const char *elementName, const char *actualStr,
                     const char *expected, const char *file, int line)
    {
        return qCompare(actual, *static_cast<const T *>(BOBUIest::qElementData(elementName,
                       qMetaTypeId<T>())), actualStr, expected, file, line);
    }

#if BOBUI_DEPRECATED_SINCE(6, 8)
    BOBUI_DEPRECATED_VERSION_X_6_8("use the overload without qxp::function_ref")
    Q_TESTLIB_EXPORT bool reportResult(bool success, qxp::function_ref<const char*()> lhs,
                                       qxp::function_ref<const char*()> rhs,
                                       const char *lhsExpr, const char *rhsExpr,
                                       ComparisonOperation op, const char *file, int line);
#endif // BOBUI_DEPRECATED_SINCE(6, 8)

    Q_TESTLIB_EXPORT bool reportResult(bool success, const void *lhs, const void *rhs,
                                       const char *(*lhsFormatter)(const void*),
                                       const char *(*rhsFormatter)(const void*),
                                       const char *lhsExpr, const char *rhsExpr,
                                       ComparisonOperation op, const char *file, int line);

    template <ComparisonOperation op, typename T1, typename T2 = T1>
    inline bool qCompareOp(T1 &&lhs, T2 &&rhs, const char *lhsExpr, const char *rhsExpr,
                           const char *file, int line)
    {
        using D1 = std::decay_t<T1>;
        using D2 = std::decay_t<T2>;
        using Internal::genericToString;
        using Comparator = Internal::Compare<op>;

        // force decaying of T1 and T2
        auto doReport = [&](bool result, const D1 &lhs_, const D2 &rhs_) {
            return reportResult(result, std::addressof(lhs_), std::addressof(rhs_),
                                genericToString<D1>, genericToString<D2>,
                                lhsExpr, rhsExpr, op, file, line);
        };

        /* assumes that op does not actually move from lhs and rhs */
        bool result = Comparator::compare(std::forward<T1>(lhs), std::forward<T2>(rhs));
        return doReport(result, std::forward<T1>(lhs), std::forward<T2>(rhs));
    }

#if defined(__cpp_lib_three_way_comparison)
    template <typename OrderingType, typename LHS, typename RHS = LHS>
    inline bool qCompare3Way(LHS &&lhs, RHS &&rhs, OrderingType order,
                             const char *lhsExpression,
                             const char *rhsExpression,
                             const char *resultExpression,
                             const char *file, int line)
    {
#if defined(__cpp_concepts)
        static_assert(requires { lhs <=> rhs; },
                      "The three-way comparison operator (<=>) is not implemented");
#endif // __cpp_concepts
        static_assert(BobUIOrderingPrivate::is_ordering_type_v<OrderingType>,
                      "Please provide, as the order parameter, a value "
                      "of one of the BobUI::{partial,weak,strong}_ordering or "
                      "std::{partial,weak,strong}_ordering types.");

        const auto comparisonResult = std::forward<LHS>(lhs) <=> std::forward<RHS>(rhs);
        static_assert(std::is_same_v<decltype(BobUIOrderingPrivate::to_BobUI(comparisonResult)),
                                     decltype(BobUIOrderingPrivate::to_BobUI(order))>,
                      "The expected and actual ordering types should be the same "
                      "strength for proper comparison.");

        const OrderingType actualOrder = comparisonResult;
        using DLHS = q20::remove_cvref_t<LHS>;
        using DRHS = q20::remove_cvref_t<RHS>;
        using Internal::genericToString;

        return compare_3way_helper(actualOrder == order,
                                   "The result of operator<=>() is not what was expected",
                                   std::addressof(lhs), std::addressof(rhs),
                                   genericToString<DLHS>, genericToString<DRHS>,
                                   lhsExpression, rhsExpression,
                                   genericToString<decltype(comparisonResult)>,
                                   genericToString<OrderingType>,
                                   std::addressof(comparisonResult),
                                   std::addressof(order),
                                   resultExpression, file, line);
    }
#else
    template <typename OrderingType, typename LHS, typename RHS = LHS>
    void qCompare3Way(LHS &&lhs, RHS &&rhs, OrderingType order,
                      const char *lhsExpression,
                      const char *rhsExpression,
                      const char *resultExpression,
                      const char *file, int line) = delete;
#endif // __cpp_lib_three_way_comparison

}

#define QWARN(msg) BOBUIest::qWarn(static_cast<const char *>(msg), __FILE__, __LINE__)

BOBUI_END_NAMESPACE

#endif
