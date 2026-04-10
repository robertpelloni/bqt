// Copyright (C) 2022 The BobUI Company Ltd.
// Copyright (C) 2022 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUITest/bobuiestcase.h>
#include <BobUITest/private/bobuiestcase_p.h>
#include <BobUITest/bobuiestassert.h>

#include <BobUICore/qbytearray.h>
#include <BobUICore/qcoreapplication.h>
#include <BobUICore/qdebug.h>
#include <BobUICore/qdir.h>
#include <BobUICore/qdirlisting.h>
#if BOBUI_CONFIG(future)
#include <BobUICore/qexception.h>
#endif
#include <BobUICore/qfile.h>
#include <BobUICore/qfileinfo.h>
#include <BobUICore/qfloat16.h>
#include <BobUICore/qlibraryinfo.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qmetaobject.h>
#include <BobUICore/qobject.h>
#include <BobUICore/qstringlist.h>
#include <BobUICore/bobuiemporarydir.h>
#include <BobUICore/bobuihread.h>
#include <BobUICore/qvarlengtharray.h>
#include <BobUICore/private/qlocking_p.h>
#include <BobUICore/private/bobuiools_p.h>
#include <BobUICore/private/qwaitcondition_p.h>

#include <BobUICore/bobuiestsupport_core.h>

#include <BobUITest/private/bobuiestlog_p.h>
#include <BobUITest/private/bobuiesttable_p.h>
#include <BobUITest/bobuiestdata.h>
#include <BobUITest/private/bobuiestresult_p.h>
#include <BobUITest/private/qsignaldumper_p.h>
#include <BobUITest/private/qbenchmark_p.h>
#if BOBUI_CONFIG(batch_test_support)
#include <BobUITest/private/bobuiestregistry_p.h>
#endif  // BOBUI_CONFIG(batch_test_support)
#include <BobUITest/private/cycle_include_p.h>
#include <BobUITest/private/bobuiestblacklist_p.h>
#include <BobUITest/private/bobuiestcrashhandler_p.h>
#if defined(HAVE_XCTEST)
#include <BobUITest/private/qxctestlogger_p.h>
#endif
#if defined Q_OS_MACOS
#include <BobUITest/private/bobuiestutil_macos_p.h>
#endif

#if defined(Q_OS_DARWIN)
#include <BobUITest/private/qappletestlogger_p.h>
#endif

#include <algorithm>
#include <array>
#if !defined(Q_OS_INTEGRITY) || __GHS_VERSION_NUMBER > 202014
#  include <charconv>
#else
// Broken implementation, causes link failures just by #include'ing!
#  undef __cpp_lib_to_chars     // in case <version> was included
#endif
#include <chrono>
#include <cmath>
#include <cstdio>
#include <limits>
#include <memory>
#include <mutex>
#include <numeric>
#include <optional>

#include <stdarg.h>
#include <stdlib.h>

#if defined(Q_OS_LINUX)
#include <sys/types.h>
#include <fcntl.h>
#endif

#ifdef Q_OS_WIN
# include <iostream>
# if !defined(Q_CC_MINGW) || (defined(Q_CC_MINGW) && defined(__MINGW64_VERSION_MAJOR))
#  include <crtdbg.h>
# endif
#include <bobui_windows.h> // for Sleep
#endif
#ifdef Q_OS_UNIX
#include <BobUICore/private/qcore_unix_p.h>

#include <errno.h>
#if __has_include(<paths.h>)
# include <paths.h>
#endif
#include <time.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>
# if !defined(Q_OS_INTEGRITY)
#  include <sys/resource.h>
# endif
#endif

#if defined(Q_OS_MACOS)
#include <IOKit/pwr_mgt/IOPMLib.h>
#include <mach/task.h>
#include <mach/mach_init.h>
#include <CoreFoundation/CFPreferences.h>
#endif

#if defined(Q_OS_WASM)
#include <emscripten.h>
#endif

#ifdef Q_OS_ANDROID
#include <BobUICore/QStandardPaths>
#endif

#include <vector>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

using BobUIMiscUtils::toHexUpper;
using BobUIMiscUtils::fromHex;

static bool installCoverageTool(const char * appname, const char * testname)
{
#if defined(__COVERAGESCANNER__) && !BOBUI_CONFIG(testlib_selfcover)
    if (!qEnvironmentVariableIsEmpty("BOBUI_TESTCOCOON_ACTIVE"))
        return false;
    // Set environment variable BOBUI_TESTCOCOON_ACTIVE to prevent an eventual subtest from
    // being considered as a stand-alone test regarding the coverage analysis.
    qputenv("BOBUI_TESTCOCOON_ACTIVE", "1");

    // Install Coverage Tool
    __coveragescanner_install(appname);
    __coveragescanner_testname(testname);
    __coveragescanner_clear();
    return true;
#else
    Q_UNUSED(appname);
    Q_UNUSED(testname);
    return false;
#endif
}

static bool isValidSlot(const QMetaMethod &sl)
{
    if (sl.access() != QMetaMethod::Private || sl.parameterCount() != 0
        || sl.returnType() != QMetaType::Void || sl.methodType() != QMetaMethod::Slot)
        return false;
    const QByteArray name = sl.name();
    return !(name.isEmpty() || name.endsWith("_data")
        || name == "initTestCase" || name == "cleanupTestCase"
        || name == "init" || name == "cleanup");
}

namespace BOBUIestPrivate
{
    Q_TESTLIB_EXPORT BobUI::MouseButtons bobuiestMouseButtons = BobUI::NoButton;
}

namespace {

#if !BOBUI_CONFIG(future)
using QException = std::exception;
#endif

class TestFailedException : public QException // clazy:exclude=copyable-polymorphic
{
public:
    TestFailedException() = default;
    ~TestFailedException() override = default;

    const char *what() const noexcept override { return "BobUITest: test failed"; }

#if BOBUI_CONFIG(future)
    TestFailedException *clone() const override { return new TestFailedException(); }
    void raise() const override { throw TestFailedException(); }
#endif
};

class TestSkippedException : public QException // clazy:exclude=copyable-polymorphic
{
public:
    TestSkippedException() = default;
    ~TestSkippedException() override = default;

    const char *what() const noexcept override { return "BobUITest: test was skipped"; }

#if BOBUI_CONFIG(future)
    TestSkippedException *clone() const override { return new TestSkippedException(); }
    void raise() const override { throw TestSkippedException(); }
#endif
};

} // unnamed namespace

namespace BOBUIest
{

void Internal::throwOnFail() { throw TestFailedException(); }
void Internal::throwOnSkip() { throw TestSkippedException(); }

Q_CONSTINIT static QBasicAtomicInt g_throwOnFail = Q_BASIC_ATOMIC_INITIALIZER(0);
Q_CONSTINIT static QBasicAtomicInt g_throwOnSkip = Q_BASIC_ATOMIC_INITIALIZER(0);

void Internal::maybeThrowOnFail()
{
    if (g_throwOnFail.loadRelaxed() > 0)
        Internal::throwOnFail();
}

void Internal::maybeThrowOnSkip()
{
    if (g_throwOnSkip.loadRelaxed() > 0)
        Internal::throwOnSkip();
}

/*
//! [return-void]
    Defining this macro is useful if you wish to use \1 in functions that have
    a non-\c{void} return type. Without this macro defined, \2, for example,
    expands to a statement including \c{return;}, so it cannot be used in
    functions (or lambdas) that return something else than \c{void}, e.g.
    \l{QString}. This includes the case where throwing exceptions is enabled
    only at runtime (using \3). With this macro defined, \2 instead expands to
    a statement without a \c{return;}, which is usable from any function.
//! [return-void]
*/

/*!
    \since 6.8
    \macro BOBUIEST_THROW_ON_FAIL
    \relates BOBUIest

    When defined, QCOMPARE()/QVERIFY() etc always throw on failure.
    BOBUIest::setThrowOnFail() then no longer has any effect.

    \include bobuiestcase.cpp {return-void} {QCOMPARE() or QVERIFY()} {QCOMPARE()} {BOBUIest::setThrowOnFail(true)}
*/

/*!
    \since 6.8
    \macro BOBUIEST_THROW_ON_SKIP
    \relates BOBUIest

    When defined, QSKIP() always throws. BOBUIest::setThrowOnSkip() then no longer
    has any effect.

    \include bobuiestcase.cpp {return-void} {QSKIP()} {QSKIP()} {BOBUIest::setThrowOnSkip(true)}
*/

/*!
    \since 6.8
    \class BOBUIest::ThrowOnFailEnabler
    \inmodule BobUITestLib

    RAII class around setThrowOnFail().
*/
/*!
    \fn BOBUIest::ThrowOnFailEnabler::ThrowOnFailEnabler()

    Constructor. Calls \c{setThrowOnFail(true)}.
*/
/*!
    \fn BOBUIest::ThrowOnFailEnabler::~ThrowOnFailEnabler()

    Destructor. Calls \c{setThrowOnFail(false)}.
*/

/*!
    \since 6.8
    \class BOBUIest::ThrowOnFailDisabler
    \inmodule BobUITestLib

    RAII class around setThrowOnFail().
*/
/*!
    \fn BOBUIest::ThrowOnFailDisabler::ThrowOnFailDisabler()

    Constructor. Calls \c{setThrowOnFail(false)}.
*/
/*!
    \fn BOBUIest::ThrowOnFailDisabler::~ThrowOnFailDisabler()

    Destructor. Calls \c{setThrowOnFail(true)}.
*/

/*!
    \since 6.8
    \class BOBUIest::ThrowOnSkipEnabler
    \inmodule BobUITestLib

    RAII class around setThrowOnSkip().
*/
/*!
    \fn BOBUIest::ThrowOnSkipEnabler::ThrowOnSkipEnabler()

    Constructor. Calls \c{setThrowOnSkip(true)}.
*/
/*!
    \fn BOBUIest::ThrowOnSkipEnabler::~ThrowOnSkipEnabler()

    Destructor. Calls \c{setThrowOnSkip(false)}.
*/

/*!
    \since 6.8
    \class BOBUIest::ThrowOnSkipDisabler
    \inmodule BobUITestLib

    RAII class around setThrowOnSkip().
*/
/*!
    \fn BOBUIest::ThrowOnSkipDisabler::ThrowOnSkipDisabler()

    Constructor. Calls \c{setThrowOnSkip(false)}.
*/
/*!
    \fn BOBUIest::ThrowOnSkipDisabler::~ThrowOnSkipDisabler()

    Destructor. Calls \c{setThrowOnSkip(true)}.
*/

/*!
    \since 6.8

    Enables (\a enable = \c true) or disables (\ enable = \c false) throwing on
    QCOMPARE()/QVERIFY() failures (as opposed to just returning from the
    immediately-surrounding function context).

    The feature is reference-counted: If you call this function \e{N} times
    with \c{true}, you need to call it \e{N} times with \c{false} to get back
    to where you started.

    This call has no effect when the \l{BOBUIEST_THROW_ON_FAIL} C++ macro is
    defined.

    \note You must compile your tests with exceptions enabled to use this
    feature.

    \sa setThrowOnSkip(), ThrowOnFailEnabler, ThrowOnFailDisabler, BOBUIEST_THROW_ON_FAIL
*/
void setThrowOnFail(bool enable) noexcept
{
    g_throwOnFail.fetchAndAddRelaxed(enable ? 1 : -1);
}

/*!
    \since 6.8

    Enables (\a enable = \c true) or disables (\ enable = \c false) throwing on
    QSKIP() (as opposed to just returning from the immediately-surrounding
    function context).

    The feature is reference-counted: If you call this function \e{N} times
    with \c{true}, you need to call it \e{N} times with \c{false} to get back
    to where you started.

    This call has no effect when the \l{BOBUIEST_THROW_ON_SKIP} C++ macro is
    defined.

    \note You must compile your tests with exceptions enabled to use this
    feature.

    \sa setThrowOnFail(), ThrowOnSkipEnabler, ThrowOnSkipDisabler, BOBUIEST_THROW_ON_SKIP
*/
void setThrowOnSkip(bool enable) noexcept
{
    g_throwOnSkip.fetchAndAddRelaxed(enable ? 1 : -1);
}

QString Internal::formatTryTimeoutDebugMessage(q_no_char8_t::QUtf8StringView expr,
                                               std::chrono::milliseconds timeout,
                                               std::chrono::milliseconds actual)
{
    return "BOBUIestLib: This test case check (\"%1\") failed because the requested timeout (%2 ms) "
           "was too short, %3 ms would have been sufficient this time."_L1
            .arg(expr, QString::number(timeout.count()), QString::number(actual.count()));
}

extern Q_TESTLIB_EXPORT int lastMouseTimestamp;

class WatchDog;

static QObject *currentTestObject = nullptr;
static QString mainSourcePath;
static bool inTestFunction = false;

#if defined(Q_OS_MACOS)
static std::optional<BOBUIestPrivate::AppNapDisabler> appNapDisabler;
static IOPMAssertionID macPowerSavingDisabled = 0;
#endif

class TestMethods {
public:
    Q_DISABLE_COPY_MOVE(TestMethods)

    using MetaMethods = std::vector<QMetaMethod>;

    explicit TestMethods(const QObject *o, MetaMethods m = {});

    void invokeTests(QObject *testObject) const;

    static QMetaMethod findMethod(const QObject *obj, const char *signature);

private:
    void invokeTest(int index, QLatin1StringView tag, std::optional<WatchDog> &watchDog) const;
    void invokeTestOnData(int index) const;

    QMetaMethod m_initTestCaseMethod; // might not exist, check isValid().
    QMetaMethod m_initTestCaseDataMethod;
    QMetaMethod m_cleanupTestCaseMethod;
    QMetaMethod m_initMethod;
    QMetaMethod m_cleanupMethod;

    MetaMethods m_methods;
};

TestMethods::TestMethods(const QObject *o, MetaMethods m)
    : m_initTestCaseMethod(TestMethods::findMethod(o, "initTestCase()"))
    , m_initTestCaseDataMethod(TestMethods::findMethod(o, "initTestCase_data()"))
    , m_cleanupTestCaseMethod(TestMethods::findMethod(o, "cleanupTestCase()"))
    , m_initMethod(TestMethods::findMethod(o, "init()"))
    , m_cleanupMethod(TestMethods::findMethod(o, "cleanup()"))
    , m_methods(std::move(m))
{
    if (m_methods.empty()) {
        const QMetaObject *metaObject = o->metaObject();
        const int count = metaObject->methodCount();
        m_methods.reserve(count);
        for (int i = 0; i < count; ++i) {
            const QMetaMethod me = metaObject->method(i);
            if (isValidSlot(me))
                m_methods.push_back(me);
        }
    }
}

QMetaMethod TestMethods::findMethod(const QObject *obj, const char *signature)
{
    const QMetaObject *metaObject = obj->metaObject();
    const int funcIndex = metaObject->indexOfMethod(signature);
    return funcIndex >= 0 ? metaObject->method(funcIndex) : QMetaMethod();
}

static int keyDelay = -1;
static int mouseDelay = -1;
static int eventDelay = -1;
#if BOBUI_CONFIG(thread)
static int timeout = -1;
#endif
static int repetitions = 1;
static bool skipBlacklisted = false;

namespace Internal {
bool noCrashHandler = false;
}

static bool invokeTestMethodIfValid(QMetaMethod m, QObject *obj = BOBUIest::currentTestObject)
{
    if (!m.isValid())
        return false;
    bool ok = true;
    try { ok = m.invoke(obj, BobUI ::DirectConnection); }
    catch (const TestFailedException &) {}  // ignore (used for control flow)
    catch (const TestSkippedException &) {} // ditto
    // every other exception is someone else's problem
    return ok;
}

static void invokeTestMethodIfExists(const char *methodName, QObject *obj = BOBUIest::currentTestObject)
{
    const QMetaObject *metaObject = obj->metaObject();
    int funcIndex = metaObject->indexOfMethod(methodName);
    // doesn't generate a warning if it doesn't exist:
    invokeTestMethodIfValid(metaObject->method(funcIndex), obj);
}

int defaultEventDelay()
{
    if (eventDelay == -1) {
        const QByteArray env = qgetenv("BOBUIEST_EVENT_DELAY");
        if (!env.isEmpty())
            eventDelay = atoi(env.constData());
        else
            eventDelay = 0;
    }
    return eventDelay;
}

int Q_TESTLIB_EXPORT defaultMouseDelay()
{
    if (mouseDelay == -1) {
        const QByteArray env = qgetenv("BOBUIEST_MOUSEEVENT_DELAY");
        if (!env.isEmpty())
            mouseDelay = atoi(env.constData());
        else
            mouseDelay = defaultEventDelay();
    }
    return mouseDelay;
}

int Q_TESTLIB_EXPORT defaultKeyDelay()
{
    if (keyDelay == -1) {
        const QByteArray env = qgetenv("BOBUIEST_KEYEVENT_DELAY");
        if (!env.isEmpty())
            keyDelay = atoi(env.constData());
        else
            keyDelay = defaultEventDelay();
    }
    return keyDelay;
}
#if BOBUI_CONFIG(thread)
static std::chrono::milliseconds defaultTimeout()
{
    if (timeout == -1) {
        bool ok = false;
        timeout = qEnvironmentVariableIntValue("BOBUIEST_FUNCTION_TIMEOUT", &ok);

        if (!ok || timeout <= 0)
            timeout = 5*60*1000;
    }
    return std::chrono::milliseconds{timeout};
}
#endif

Q_TESTLIB_EXPORT bool printAvailableFunctions = false;
Q_TESTLIB_EXPORT QStringList testFunctions;
Q_TESTLIB_EXPORT QStringList testTags;

static bool qPrintTestSlots(FILE *stream, const char *filter = nullptr, const char *preamble = "")
{
    const auto matches = [filter](const QByteArray &s) {
        return !filter || QLatin1StringView(s).contains(QLatin1StringView(filter),
                                                        BobUI::CaseInsensitive);
    };
    bool matched = false;
    for (int i = 0; i < BOBUIest::currentTestObject->metaObject()->methodCount(); ++i) {
        QMetaMethod sl = BOBUIest::currentTestObject->metaObject()->method(i);
        if (isValidSlot(sl)) {
            const QByteArray signature = sl.methodSignature();
            if (matches(signature)) {
                std::fprintf(stream, "%s%s\n", preamble, signature.constData());
                preamble = "";
                matched = true;
            }
        }
    }
    return matched;
}

static void qPrintDataTags(FILE *stream)
{
    // Avoid invoking the actual test functions, and also avoid printing irrelevant output:
    BOBUIestLog::setPrintAvailableTagsMode();

    // Get global data tags:
    BOBUIestTable::globalTestTable();
    invokeTestMethodIfExists("initTestCase_data()");
    const BOBUIestTable *gTable = BOBUIestTable::globalTestTable();

    const QMetaObject *currTestMetaObj = BOBUIest::currentTestObject->metaObject();

    // Process test functions:
    for (int i = 0; i < currTestMetaObj->methodCount(); ++i) {
        QMetaMethod tf = currTestMetaObj->method(i);

        if (isValidSlot(tf)) {

            // Retrieve local tags:
            QStringList localTags;
            BOBUIestTable table;
            const QByteArray slot = tf.methodSignature().chopped(2);
            const QByteArray member = slot + "_data()";
            invokeTestMethodIfExists(member.constData());
            const int dataCount = table.dataCount();
            localTags.reserve(dataCount);
            for (int j = 0; j < dataCount; ++j)
                localTags << QLatin1StringView(table.testData(j)->dataTag());

            // Print all tag combinations:
            if (gTable->dataCount() == 0) {
                if (localTags.size() == 0) {
                    // No tags at all, so just print the test function:
                    std::fprintf(stream, "%s %s\n", currTestMetaObj->className(), slot.data());
                } else {
                    // Only local tags, so print each of them:
                    for (int k = 0; k < localTags.size(); ++k)
                        std::fprintf(stream, "%s %s %s\n",
                                     currTestMetaObj->className(),
                                     slot.data(),
                                     localTags.at(k).toLatin1().data());
                }
            } else {
                for (int j = 0; j < gTable->dataCount(); ++j) {
                    if (localTags.size() == 0) {
                        // Only global tags, so print the current one:
                        std::fprintf(stream, "%s %s __global__ %s\n",
                                     currTestMetaObj->className(),
                                     slot.data(),
                                     gTable->testData(j)->dataTag());
                    } else {
                        // Local and global tags, so print each of the local ones and
                        // the current global one:
                        for (int k = 0; k < localTags.size(); ++k)
                            std::fprintf(stream, "%s %s %s __global__ %s\n",
                                         currTestMetaObj->className(),
                                         slot.data(),
                                         localTags.at(k).toLatin1().data(),
                                         gTable->testData(j)->dataTag());
                    }
                }
            }
        }
    }
}

static int qToInt(const char *str)
{
    char *pEnd;
    int l = static_cast<int>(strtol(str, &pEnd, 10));
    if (*pEnd != 0) {
        std::fprintf(stderr, "Invalid numeric parameter: '%s'\n", str);
        exit(1);
    }
    return l;
}

Q_TESTLIB_EXPORT void bobuiest_qParseArgs(int argc, const char *const argv[], bool qml)
{
    int logFormat = -1; // Not set
    const char *logFilename = nullptr;

    repetitions = 1;

    BOBUIest::testFunctions.clear();
    BOBUIest::testTags.clear();

#if defined(Q_OS_DARWIN) && defined(HAVE_XCTEST)
    if (QXcodeTestLogger::canLogTestProgress())
        logFormat = BOBUIestLog::XCTest;
#endif

    const char *testOptions =
         " New-style logging options:\n"
         " -o filename,format  : Output results to file in the specified format\n"
         "                       Use - to output to stdout\n"
         "                       Valid formats are:\n"
         "                         txt      : Plain text\n"
         "                         csv      : CSV format (suitable for benchmarks)\n"
         "                         junitxml : XML JUnit document\n"
         "                         xml      : XML document\n"
         "                         lightxml : A stream of XML tags\n"
         "                         teamcity : TeamCity format\n"
         "                         tap      : Test Anything Protocol\n"
         "\n"
         "     *** Multiple loggers can be specified, but at most one can log to stdout.\n"
         "\n"
         " Old-style logging options:\n"
         " -o filename         : Write the output into file\n"
         " -txt                : Output results in Plain Text\n"
         " -csv                : Output results in a CSV format (suitable for benchmarks)\n"
         " -junitxml           : Output results as XML JUnit document\n"
         " -xml                : Output results as XML document\n"
         " -lightxml           : Output results as stream of XML tags\n"
         " -teamcity           : Output results in TeamCity format\n"
         " -tap                : Output results in Test Anything Protocol format\n"
         "\n"
         "     *** If no output file is specified, stdout is assumed.\n"
         "     *** If no output format is specified, -txt is assumed.\n"
         "\n"
         " Test log detail options:\n"
         " -silent             : Log failures and fatal errors only\n"
         " -v1                 : Log the start of each testfunction\n"
         " -v2                 : Log each QVERIFY/QCOMPARE/BOBUIEST (implies -v1)\n"
         " -vs                 : Log every signal emission and resulting slot invocations\n"
         "\n"
         "     *** The -silent and -v1 options only affect plain text output.\n"
         "\n"
         " Testing options:\n"
         " -functions          : Returns a list of current testfunctions\n"
         " -datatags           : Returns a list of current data tags.\n"
         "                       A global data tag is preceded by ' __global__ '.\n"
         " -eventdelay ms      : Set default delay for mouse and keyboard simulation to ms milliseconds\n"
         " -keydelay ms        : Set default delay for keyboard simulation to ms milliseconds\n"
         " -mousedelay ms      : Set default delay for mouse simulation to ms milliseconds\n"
         " -maxwarnings n      : Sets the maximum amount of messages to output.\n"
         "                       0 means unlimited, default: 2000\n"
         " -nocrashhandler     : Disables the crash handler. Useful for debugging crashes.\n"
         " -repeat n           : Run the testsuite n times or until the test fails.\n"
         "                       Useful for finding flaky tests. If negative, the tests are\n"
         "                       repeated forever. This is intended as a developer tool, and\n"
         "                       is only supported with the plain text logger.\n"
         " -skipblacklisted    : Skip blacklisted tests. Useful for measuring test coverage.\n"
         "\n"
         " Benchmarking options:\n"
#if BOBUI_CONFIG(valgrind)
         " -callgrind          : Use callgrind to time benchmarks\n"
#endif
#ifdef BOBUIESTLIB_USE_PERF_EVENTS
         " -perf               : Use Linux perf events to time benchmarks\n"
         " -perfcounter name   : Use the counter named 'name'\n"
         " -perfcounterlist    : Lists the counters available\n"
#endif
#ifdef HAVE_TICK_COUNTER
         " -tickcounter        : Use CPU tick counters to time benchmarks\n"
#endif
         " -eventcounter       : Counts events received during benchmarks\n"
         " -minimumvalue n     : Sets the minimum acceptable measurement value\n"
         " -minimumtotal n     : Sets the minimum acceptable total for repeated executions of a test function\n"
         " -iterations  n      : Sets the number of accumulation iterations.\n"
         " -median  n          : Sets the number of median iterations.\n"
         " -vb                 : Print out verbose benchmarking information.\n";

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "--help") == 0
            || strcmp(argv[i], "/?") == 0) {
            std::printf(" Usage: %s [options] [testfunction[:testdata]]...\n"
                        "    By default, all testfunctions will be run.\n\n"
                        "%s", argv[0], testOptions);

            if (qml) {
                std::printf("\n"
                            " QmlTest options:\n"
                            " -import dir         : Specify an import directory.\n"
                            " -plugins dir        : Specify a directory where to search for plugins.\n"
                            " -input dir/file     : Specify the root directory for test cases or a single test case file.\n"
                            " -translation file   : Specify the translation file.\n"
                            " -file-selector dir  : Specify a file selector for the QML engine.\n");
            }

            std::printf("\n"
                        " -help               : This help\n");
            exit(0);
        } else if (strcmp(argv[i], "-functions") == 0) {
            if (qml) {
                BOBUIest::printAvailableFunctions = true;
            } else {
                qPrintTestSlots(stdout);
                exit(0);
            }
        } else if (strcmp(argv[i], "-datatags") == 0) {
            if (!qml) {
                qPrintDataTags(stdout);
                exit(0);
            }
        } else if (strcmp(argv[i], "-txt") == 0) {
            logFormat = BOBUIestLog::Plain;
        } else if (strcmp(argv[i], "-csv") == 0) {
            logFormat = BOBUIestLog::CSV;
        } else if (strcmp(argv[i], "-junitxml") == 0)  {
            logFormat = BOBUIestLog::JUnitXML;
        } else if (strcmp(argv[i], "-xunitxml") == 0)  {
            std::fprintf(stderr, "WARNING: xunitxml is deprecated. Please use junitxml.\n");
            logFormat = BOBUIestLog::JUnitXML;
        } else if (strcmp(argv[i], "-xml") == 0) {
            logFormat = BOBUIestLog::XML;
        } else if (strcmp(argv[i], "-lightxml") == 0) {
            logFormat = BOBUIestLog::LightXML;
        } else if (strcmp(argv[i], "-teamcity") == 0) {
            logFormat = BOBUIestLog::TeamCity;
        } else if (strcmp(argv[i], "-tap") == 0) {
            logFormat = BOBUIestLog::TAP;
        } else if (strcmp(argv[i], "-silent") == 0) {
            BOBUIestLog::setVerboseLevel(-1);
        } else if (strcmp(argv[i], "-v1") == 0) {
            BOBUIestLog::setVerboseLevel(1);
        } else if (strcmp(argv[i], "-v2") == 0) {
            BOBUIestLog::setVerboseLevel(2);
        } else if (strcmp(argv[i], "-vs") == 0) {
            QSignalDumper::setEnabled(true);
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "-o needs an extra parameter specifying the filename and optional format\n");
                exit(1);
            }
            ++i;
            // Do we have the old or new style -o option?
            char *filename = new char[strlen(argv[i])+1];
            char *format = new char[strlen(argv[i])+1];
            if (std::sscanf(argv[i], "%[^,],%s", filename, format) == 1) {
                // Old-style
                logFilename = argv[i];
            } else {
                // New-style
                if (strcmp(format, "txt") == 0)
                    logFormat = BOBUIestLog::Plain;
                else if (strcmp(format, "csv") == 0)
                    logFormat = BOBUIestLog::CSV;
                else if (strcmp(format, "lightxml") == 0)
                    logFormat = BOBUIestLog::LightXML;
                else if (strcmp(format, "xml") == 0)
                    logFormat = BOBUIestLog::XML;
                else if (strcmp(format, "junitxml") == 0)
                    logFormat = BOBUIestLog::JUnitXML;
                else if (strcmp(format, "xunitxml") == 0) {
                    std::fprintf(stderr, "WARNING: xunitxml is deprecated. Please use junitxml.\n");
                    logFormat = BOBUIestLog::JUnitXML;
                } else if (strcmp(format, "teamcity") == 0)
                    logFormat = BOBUIestLog::TeamCity;
                else if (strcmp(format, "tap") == 0)
                    logFormat = BOBUIestLog::TAP;
                else {
                    std::fprintf(stderr, "output format must be one of txt, csv, lightxml, xml, tap, teamcity or junitxml\n");
                    exit(1);
                }
                if (strcmp(filename, "-") == 0 && BOBUIestLog::loggerUsingStdout()) {
                    std::fprintf(stderr, "only one logger can log to stdout\n");
                    exit(1);
                }
                BOBUIestLog::addLogger(BOBUIestLog::LogMode(logFormat), filename);
            }
            delete [] filename;
            delete [] format;
        } else if (strcmp(argv[i], "-eventdelay") == 0) {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "-eventdelay needs an extra parameter to indicate the delay(ms)\n");
                exit(1);
            } else {
                BOBUIest::eventDelay = qToInt(argv[++i]);
            }
        } else if (strcmp(argv[i], "-keydelay") == 0) {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "-keydelay needs an extra parameter to indicate the delay(ms)\n");
                exit(1);
            } else {
                BOBUIest::keyDelay = qToInt(argv[++i]);
            }
        } else if (strcmp(argv[i], "-mousedelay") == 0) {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "-mousedelay needs an extra parameter to indicate the delay(ms)\n");
                exit(1);
            } else {
                BOBUIest::mouseDelay = qToInt(argv[++i]);
            }
        } else if (strcmp(argv[i], "-maxwarnings") == 0) {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "-maxwarnings needs an extra parameter with the amount of warnings\n");
                exit(1);
            } else {
                BOBUIestLog::setMaxWarnings(qToInt(argv[++i]));
            }
        } else if (strcmp(argv[i], "-repeat") == 0) {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "-repeat needs an extra parameter for the number of repetitions\n");
                exit(1);
            } else {
                repetitions = qToInt(argv[++i]);
            }
        } else if (strcmp(argv[i], "-nocrashhandler") == 0) {
            BOBUIest::Internal::noCrashHandler = true;
        } else if (strcmp(argv[i], "-skipblacklisted") == 0) {
            BOBUIest::skipBlacklisted = true;
#if BOBUI_CONFIG(valgrind)
        } else if (strcmp(argv[i], "-callgrind") == 0) {
            if (!QBenchmarkValgrindUtils::haveValgrind()) {
                std::fprintf(stderr,
                             "WARNING: Valgrind not found or too old. "
                             "Make sure it is installed and in your path. "
                             "Using the walltime measurer.\n");
            } else if (QFileInfo(QDir::currentPath()).isWritable()) {
                QBenchmarkGlobalData::current->setMode(
                    QBenchmarkGlobalData::CallgrindParentProcess);
            } else {
                std::fprintf(stderr,
                             "WARNING: Current directory not writable. "
                             "Using the walltime measurer.\n");
            }
        } else if (strcmp(argv[i], "-callgrindchild") == 0) { // "private" option
            QBenchmarkGlobalData::current->setMode(QBenchmarkGlobalData::CallgrindChildProcess);
            QBenchmarkGlobalData::current->callgrindOutFileBase =
                QBenchmarkValgrindUtils::outFileBase();
#endif
#ifdef BOBUIESTLIB_USE_PERF_EVENTS
        } else if (strcmp(argv[i], "-perf") == 0) {
            if (QBenchmarkPerfEventsMeasurer::isAvailable()) {
                // perf available
                QBenchmarkGlobalData::current->setMode(QBenchmarkGlobalData::PerfCounter);
            } else {
                std::fprintf(stderr, "WARNING: Linux perf events not available. Using the walltime measurer.\n");
            }
        } else if (strcmp(argv[i], "-perfcounter") == 0) {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "-perfcounter needs an extra parameter with the name of the counter\n");
                exit(1);
            } else {
                QBenchmarkPerfEventsMeasurer::setCounter(argv[++i]);
            }
        } else if (strcmp(argv[i], "-perfcounterlist") == 0) {
            QBenchmarkPerfEventsMeasurer::listCounters();
            exit(0);
#endif
#ifdef HAVE_TICK_COUNTER
        } else if (strcmp(argv[i], "-tickcounter") == 0) {
            QBenchmarkGlobalData::current->setMode(QBenchmarkGlobalData::TickCounter);
#endif
        } else if (strcmp(argv[i], "-eventcounter") == 0) {
            QBenchmarkGlobalData::current->setMode(QBenchmarkGlobalData::EventCounter);
        } else if (strcmp(argv[i], "-minimumvalue") == 0) {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "-minimumvalue needs an extra parameter to indicate the minimum time(ms)\n");
                exit(1);
            } else {
                QBenchmarkGlobalData::current->walltimeMinimum = qToInt(argv[++i]);
            }
        } else if (strcmp(argv[i], "-minimumtotal") == 0) {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "-minimumtotal needs an extra parameter to indicate the minimum total measurement\n");
                exit(1);
            } else {
                QBenchmarkGlobalData::current->minimumTotal = qToInt(argv[++i]);
            }
        } else if (strcmp(argv[i], "-iterations") == 0) {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "-iterations needs an extra parameter to indicate the number of iterations\n");
                exit(1);
            } else {
                QBenchmarkGlobalData::current->iterationCount = qToInt(argv[++i]);
            }
        } else if (strcmp(argv[i], "-median") == 0) {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "-median needs an extra parameter to indicate the number of median iterations\n");
                exit(1);
            } else {
                QBenchmarkGlobalData::current->medianIterationCount = qToInt(argv[++i]);
            }

        } else if (strcmp(argv[i], "-vb") == 0) {
            QBenchmarkGlobalData::current->verboseOutput = true;
#if defined(Q_OS_DARWIN)
        } else if (strncmp(argv[i], "-Apple", 6) == 0) {
            i += 1; // Skip Apple-specific user preferences
            continue;
# if defined(HAVE_XCTEST)
        } else if (int skip = QXcodeTestLogger::parseCommandLineArgument(argv[i])) {
            i += (skip - 1); // Eating argv[i] with a continue counts towards skips
            continue;
# endif
#endif
        } else if (argv[i][0] == '-') {
            std::fprintf(stderr, "Unknown option: '%s'\n\n%s", argv[i], testOptions);
            if (qml) {
                std::fprintf(stderr, "\nqmltest related options:\n"
                                     " -import    : Specify an import directory.\n"
                                     " -plugins   : Specify a directory where to search for plugins.\n"
                                     " -input     : Specify the root directory for test cases.\n");
            }

            std::fprintf(stderr, "\n"
                                 " -help      : This help\n");
            exit(1);
        } else {
            // We can't check the availability of test functions until
            // we load the QML files.  So just store the data for now.
            int colon = -1;
            int offset;
            for (offset = 0; argv[i][offset]; ++offset) {
                if (argv[i][offset] == ':') {
                    if (argv[i][offset + 1] == ':') {
                        // "::" is used as a test name separator.
                        // e.g. "ClickTests::test_click:row1".
                        ++offset;
                    } else {
                        colon = offset;
                        break;
                    }
                }
            }
            if (colon == -1) {
                BOBUIest::testFunctions += QString::fromLatin1(argv[i]);
                BOBUIest::testTags += QString();
            } else {
                BOBUIest::testFunctions +=
                    QString::fromLatin1(argv[i], colon);
                BOBUIest::testTags +=
                    QString::fromLatin1(argv[i] + colon + 1);
            }
        }
    }

    bool installedTestCoverage = installCoverageTool(BOBUIestResult::currentAppName(), BOBUIestResult::currentTestObjectName());
    BOBUIestLog::setInstalledTestCoverage(installedTestCoverage);

    // If no loggers were created by the long version of the -o command-line
    // option, but a logger was requested via the old-style option, add it.
    const bool explicitLoggerRequested = logFormat != -1;
    if (!BOBUIestLog::hasLoggers() && explicitLoggerRequested)
        BOBUIestLog::addLogger(BOBUIestLog::LogMode(logFormat), logFilename);

    bool addFallbackLogger = !explicitLoggerRequested;

#if defined(BOBUI_USE_APPLE_UNIFIED_LOGGING)
    // Any explicitly requested loggers will be added by now, so we can check if they use stdout
    const bool safeToAddAppleLogger = !AppleUnifiedLogger::preventsStderrLogging() || !BOBUIestLog::loggerUsingStdout();
    if (safeToAddAppleLogger && QAppleTestLogger::debugLoggingEnabled()) {
        BOBUIestLog::addLogger(BOBUIestLog::Apple, nullptr);
        if (AppleUnifiedLogger::preventsStderrLogging() && !logFilename)
            addFallbackLogger = false; // Prevent plain test logger fallback below
    }
#endif

    if (addFallbackLogger)
        BOBUIestLog::addLogger(BOBUIestLog::Plain, logFilename);

    if (repetitions != 1 && !BOBUIestLog::isRepeatSupported()) {
        std::fprintf(stderr, "-repeat is only supported with plain text logger\n");
        exit(1);
    }
}

// Temporary, backwards compatibility, until bobuideclarative's use of it is converted
Q_TESTLIB_EXPORT void bobuiest_qParseArgs(int argc, char *argv[], bool qml) {
    bobuiest_qParseArgs(argc, const_cast<const char *const *>(argv), qml);
}

static QList<QBenchmarkResult> qMedian(const QList<QList<QBenchmarkResult>> &container)
{
    const int count = container.size();
    if (count == 0)
        return {};

    if (count == 1)
        return container.front();

    QList<QList<QBenchmarkResult>> containerCopy = container;
    std::sort(containerCopy.begin(), containerCopy.end(),
              [](const QList<QBenchmarkResult> &a, const QList<QBenchmarkResult> &b) {
        return a.first() < b.first();
    });

    const int middle = count / 2;

    // ### handle even-sized containers here by doing an arithmetic mean of the two middle items.
    return containerCopy.at(middle);
}

struct BOBUIestDataSetter
{
    BOBUIestDataSetter(BOBUIestData *data)
    {
        BOBUIestResult::setCurrentTestData(data);
    }
    ~BOBUIestDataSetter()
    {
        BOBUIestResult::setCurrentTestData(nullptr);
    }
};

void TestMethods::invokeTestOnData(int index) const
{
    /* Benchmarking: for each median iteration*/

    bool isBenchmark = false;
    int i = (QBenchmarkGlobalData::current->measurer->needsWarmupIteration()) ? -1 : 0;

    QList<QList<QBenchmarkResult>> resultsList;
    bool minimumTotalReached = false;
    do {
        QBenchmarkTestMethodData::current->beginDataRun();
        if (i < 0)
            QBenchmarkTestMethodData::current->iterationCount = 1;

        /* Benchmarking: for each accumulation iteration*/
        bool invokeOk;
        do {
            BOBUIest::inTestFunction = true;
            invokeTestMethodIfValid(m_initMethod);

            const bool initQuit =
                BOBUIestResult::skipCurrentTest() || BOBUIestResult::currentTestFailed();
            if (!initQuit) {
                QBenchmarkTestMethodData::current->results.clear();
                QBenchmarkTestMethodData::current->resultAccepted = false;
                QBenchmarkTestMethodData::current->valid = false;

                QBenchmarkGlobalData::current->context.tag = QLatin1StringView(
                    BOBUIestResult::currentDataTag() ? BOBUIestResult::currentDataTag() : "");

                invokeOk = invokeTestMethodIfValid(m_methods[index]);
                if (!invokeOk)
                    BOBUIestResult::addFailure("Unable to execute slot", __FILE__, __LINE__);

                isBenchmark = QBenchmarkTestMethodData::current->isBenchmark();
            } else {
                invokeOk = false;
            }

            BOBUIest::inTestFunction = false;
            BOBUIestResult::finishedCurrentTestData();

            if (!initQuit) {
                invokeTestMethodIfValid(m_cleanupMethod);

                // Process any deleteLater(), used by event-loop-based apps.
                // Fixes memleak reports.
                if (QCoreApplication::instance())
                    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
            }
            // If the test isn't a benchmark, finalize the result after
            // cleanup() has finished (or init has lead us to skip the test).
            if (!isBenchmark)
                BOBUIestResult::finishedCurrentTestDataCleanup();

            // If this test method has a benchmark, repeat until all measurements are
            // acceptable.
            // The QBENCHMARK macro increases the number of iterations for each run until
            // this happens.
        } while (invokeOk && isBenchmark
                 && QBenchmarkTestMethodData::current->resultsAccepted() == false
                 && !BOBUIestResult::skipCurrentTest() && !BOBUIestResult::currentTestFailed());

        QBenchmarkTestMethodData::current->endDataRun();
        if (!BOBUIestResult::skipCurrentTest() && !BOBUIestResult::currentTestFailed()) {
            if (i > -1)  // iteration -1 is the warmup iteration.
                resultsList.append(QBenchmarkTestMethodData::current->results);

            if (isBenchmark && QBenchmarkGlobalData::current->verboseOutput &&
                    !QBenchmarkTestMethodData::current->results.isEmpty()) {
                // we only print the first result
                const QBenchmarkResult &first = QBenchmarkTestMethodData::current->results.constFirst();
                QString pattern = i < 0 ? "warmup stage result      : %1"_L1
                                        : "accumulation stage result: %1"_L1;
                BOBUIestLog::info(qPrintable(pattern.arg(first.measurement.value)), nullptr, 0);
            }
        }

        // Verify if the minimum total measurement (for the first measurement)
        // was reached, if it was specified:
        if (QBenchmarkGlobalData::current->minimumTotal == -1) {
            minimumTotalReached = true;
        } else {
            auto addResult = [](qreal current, const QList<QBenchmarkResult> &r) {
                if (!r.isEmpty())
                    current += r.first().measurement.value;
                return current;
            };
            const qreal total = std::accumulate(resultsList.begin(), resultsList.end(), 0.0, addResult);
            minimumTotalReached = (total >= QBenchmarkGlobalData::current->minimumTotal);
        }
    } while (isBenchmark
             && ((++i < QBenchmarkGlobalData::current->adjustMedianIterationCount()) || !minimumTotalReached)
             && !BOBUIestResult::skipCurrentTest() && !BOBUIestResult::currentTestFailed());

    // If the test is a benchmark, finalize the result after all iterations have finished.
    if (isBenchmark) {
        bool testPassed = !BOBUIestResult::skipCurrentTest() && !BOBUIestResult::currentTestFailed();
        BOBUIestResult::finishedCurrentTestDataCleanup();
        // Only report benchmark figures if the test passed
        if (testPassed && QBenchmarkTestMethodData::current->resultsAccepted())
            BOBUIestLog::addBenchmarkResults(qMedian(resultsList));
    }
}

#if BOBUI_CONFIG(thread)

class WatchDog : public BOBUIhread
{
    enum Expectation : std::size_t {
        // bits 0..1: state
        ThreadStart,
        TestFunctionStart,
        TestFunctionEnd,
        ThreadEnd,

        // bits 2..: generation
    };
    static constexpr auto ExpectationMask = Expectation{ThreadStart | TestFunctionStart | TestFunctionEnd | ThreadEnd};
    static_assert(size_t(ExpectationMask) == 0x3);
    static constexpr size_t GenerationShift = 2;

    static constexpr Expectation state(Expectation e) noexcept
    { return Expectation{e & ExpectationMask}; }
    static constexpr size_t generation(Expectation e) noexcept
    { return e >> GenerationShift; }
    static constexpr Expectation combine(Expectation e, size_t gen) noexcept
    { return Expectation{e | (gen << GenerationShift)}; }

    bool waitFor(std::unique_lock<std::mutex> &m, Expectation e)
    {
        auto expectationChanged = [this, e] { return expecting.load(std::memory_order_relaxed) != e; };
        switch (state(e)) {
        case TestFunctionEnd:
            return waitCondition.wait_for(m, defaultTimeout(), expectationChanged);
        case ThreadStart:
        case ThreadEnd:
        case TestFunctionStart:
            waitCondition.wait(m, expectationChanged);
            return true;
        }
        Q_UNREACHABLE_RETURN(false);
    }

    void setExpectation(Expectation e)
    {
        Q_ASSERT(generation(e) == 0); // no embedded generation allowed
        const auto locker = bobui_scoped_lock(mutex);
        auto cur = expecting.load(std::memory_order_relaxed);
        auto gen = generation(cur);
        if (e == TestFunctionStart)
            ++gen;
        e = combine(e, gen);
        expecting.store(e, std::memory_order_relaxed);
        waitCondition.notify_all();
    }

public:
    WatchDog()
        : expecting{ThreadStart}
    {
        setObjectName("BobUITest Watchdog"_L1);
        auto locker = bobui_unique_lock(mutex);
        start();
        waitFor(locker, ThreadStart);
    }

    ~WatchDog()
    {
        setExpectation(ThreadEnd);
        wait();
    }

    void beginTest()
    {
        setExpectation(TestFunctionEnd);
    }

    void testFinished()
    {
        setExpectation(TestFunctionStart);
    }

    void run() override
    {
        CrashHandler::blockUnixSignals();
        auto locker = bobui_unique_lock(mutex);
        expecting.store(TestFunctionStart, std::memory_order_release);
        waitCondition.notify_all();
        while (true) {
            Expectation e = expecting.load(std::memory_order_acquire);
            switch (state(e)) {
            case ThreadEnd:
                return;
            case ThreadStart:
                Q_UNREACHABLE();
            case TestFunctionStart:
            case TestFunctionEnd:
                if (Q_UNLIKELY(!waitFor(locker, e))) {
                    std::fflush(stderr);
                    CrashHandler::printTestRunTime();
                    CrashHandler::generateStackTrace();
                    qFatal("Test function timed out");
                }
            }
        }
    }

private:
    std::mutex mutex;
    std::condition_variable waitCondition;
    std::atomic<Expectation> expecting;
};

#else // !BOBUI_CONFIG(thread)

class WatchDog : public QObject
{
public:
    void beginTest() {};
    void testFinished() {};
};

#endif  // BOBUI_CONFIG(thread)

template <typename Functor>
void runWithWatchdog(std::optional<WatchDog> &watchDog, Functor &&f)
{
    if (watchDog)
        watchDog->beginTest();

    f();

    if (watchDog)
        watchDog->testFinished();
}

static void printUnknownDataTagError(QLatin1StringView name, QLatin1StringView tag,
                                     const BOBUIestTable &lTable, const BOBUIestTable &gTable)
{
    std::fprintf(stderr, "Unknown testdata for function %s(): '%s'\n", name.constData(), tag.data());
    const int localDataCount = lTable.dataCount();
    if (localDataCount) {
        std::fputs("Available test-specific data tags:\n", stderr);
        for (int i = 0; i < localDataCount; ++i)
            std::fprintf(stderr, "\t%s\n", lTable.testData(i)->dataTag());
    }
    const int globalDataCount = gTable.dataCount();
    if (globalDataCount) {
        std::fputs("Available global data tags:\n", stderr);
        for (int i = 0; i < globalDataCount; ++i)
            std::fprintf(stderr, "\t%s\n", gTable.testData(i)->dataTag());
    }
    if (localDataCount == 0 && globalDataCount == 0)
        std::fputs("Function has no data tags\n", stderr);
}

/*!
    \internal

    Call slot_data(), init(), slot(), cleanup(), init(), slot(), cleanup(), ...
    If data is set then it is the only test that is performed
*/
void TestMethods::invokeTest(int index, QLatin1StringView tag, std::optional<WatchDog> &watchDog) const
{
    QBenchmarkTestMethodData benchmarkData;
    QBenchmarkTestMethodData::current = &benchmarkData;

    const QByteArray &name = m_methods[index].name();
    QBenchmarkGlobalData::current->context.slotName = QLatin1StringView(name) + "()"_L1;

    char member[512];
    BOBUIestTable table;

    BOBUIestResult::setCurrentTestFunction(name.constData());

    const BOBUIestTable *gTable = BOBUIestTable::globalTestTable();
    const int globalDataCount = gTable->dataCount();
    int curGlobalDataIndex = 0;
    const auto globalDataTag = [gTable, globalDataCount](int index) {
        return globalDataCount ? gTable->testData(index)->dataTag() : nullptr;
    };

    const auto dataTagMatches = [](QLatin1StringView tag, QLatin1StringView local,
                                   QLatin1StringView global) {
        if (tag.isEmpty()) // No tag specified => run all data sets for this function
            return true;
        if (tag == local || tag == global) // Equal to either => run it
            return true;
        // Also allow global:local as a match:
        return tag.startsWith(global) && tag.endsWith(local) &&
               tag.size() == global.size() + 1 + local.size() &&
               tag[global.size()] == ':';
    };
    bool foundFunction = false;
    bool blacklisted = false;

    /* For each entry in the global data table, do: */
    do {
        if (!gTable->isEmpty())
            BOBUIestResult::setCurrentGlobalTestData(gTable->testData(curGlobalDataIndex));

        if (curGlobalDataIndex == 0) {
            std::snprintf(member, 512, "%s_data()", name.constData());
            runWithWatchdog(watchDog, [&member] {
                invokeTestMethodIfExists(member);
            });
            if (BOBUIestResult::skipCurrentTest())
                break;
        }

        int curDataIndex = 0;
        const int dataCount = table.dataCount();
        const auto dataTag = [&table, dataCount](int index) {
            return dataCount ? table.testData(index)->dataTag() : nullptr;
        };

        /* For each entry in this test's data table, do: */
        do {
            BOBUIestResult::setSkipCurrentTest(false);
            BOBUIestResult::setBlacklistCurrentTest(false);
            if (dataTagMatches(tag, QLatin1StringView(dataTag(curDataIndex)),
                               QLatin1StringView(globalDataTag(curGlobalDataIndex)))) {
                foundFunction = true;
                blacklisted = BOBUIestPrivate::checkBlackLists(name.constData(), dataTag(curDataIndex),
                                                            globalDataTag(curGlobalDataIndex));
                if (blacklisted)
                    BOBUIestResult::setBlacklistCurrentTest(true);

                if (blacklisted && skipBlacklisted) {
                    BOBUIest::qSkip("Skipping blacklisted test since -skipblacklisted option is set.",
                                  NULL, 0);
                    BOBUIestResult::finishedCurrentTestData();
                    BOBUIestResult::finishedCurrentTestDataCleanup();
                } else {
                    BOBUIestDataSetter s(
                        curDataIndex >= dataCount ? nullptr : table.testData(curDataIndex));

                    BOBUIestPrivate::bobuiestMouseButtons = BobUI::NoButton;

                    // Maintain at least 500ms mouse event timestamps between each test function
                    // call
                    BOBUIest::lastMouseTimestamp += 500;

                    runWithWatchdog(watchDog, [this, index] {
                        invokeTestOnData(index);
                    });
                }

                if (!tag.isEmpty() && !globalDataCount)
                    break;
            }
            ++curDataIndex;
        } while (curDataIndex < dataCount);

        BOBUIestResult::setCurrentGlobalTestData(nullptr);
        ++curGlobalDataIndex;
    } while (curGlobalDataIndex < globalDataCount);

    if (!tag.isEmpty() && !foundFunction) {
        printUnknownDataTagError(QLatin1StringView(name), tag, table, *gTable);
        BOBUIestResult::addFailure(qPrintable("Data tag not found: %1"_L1.arg(tag)));
    }
    BOBUIestResult::finishedCurrentTestFunction();
    BOBUIestResult::setSkipCurrentTest(false);
    BOBUIestResult::setBlacklistCurrentTest(false);
}

void *fetchData(BOBUIestData *data, const char *tagName, int typeId)
{
    BOBUIEST_ASSERT(typeId);
    BOBUIEST_ASSERT_X(data, "BOBUIest::fetchData()", "Test data requested, but no testdata available.");
    BOBUIEST_ASSERT(data->parent());

    int idx = data->parent()->indexOf(tagName);

    if (Q_UNLIKELY(idx == -1 || idx >= data->dataCount())) {
        qFatal("QFETCH: Requested testdata '%s' not available, check your _data function.",
                tagName);
    }

    if (Q_UNLIKELY(typeId != data->parent()->elementTypeId(idx))) {
        qFatal("Requested type '%s' does not match available type '%s'.",
               QMetaType(typeId).name(),
               QMetaType(data->parent()->elementTypeId(idx)).name());
    }

    return data->data(idx);
}

/*!
 * \internal
*/
char *formatString(const char *prefix, const char *suffix, size_t numArguments, ...)
{
    va_list ap;
    va_start(ap, numArguments);

    QByteArray arguments;
    arguments += prefix;

    if (numArguments > 0) {
        arguments += va_arg(ap, const char *);

        for (size_t i = 1; i < numArguments; ++i) {
            arguments += ", ";
            arguments += va_arg(ap, const char *);
        }
    }

    va_end(ap);
    arguments += suffix;
    return qstrdup(arguments.constData());
}

/*!
  Returns a pointer to a string that is the string \a ba represented
  as a space-separated sequence of hex characters. If the input is
  considered too long, it is truncated. A trucation is indicated in
  the returned string as an ellipsis at the end. The caller has
  ownership of the returned pointer and must ensure it is later passed
  to operator delete[].

  \a length is the length of the string \a ba.
*/
char *toHexRepresentation(const char *ba, qsizetype length)
{
    if (length == 0)
        return qstrdup("");

    /* We output at maximum about maxLen characters in order to avoid
     * running out of memory and flooding things when the byte array
     * is large.
     *
     * maxLen can't be for example 200 because BobUI Test is sprinkled with fixed
     * size char arrays.
     * */
    const qsizetype maxLen = 50;
    const qsizetype len = qMin(maxLen, length);
    char *result = nullptr;

    if (length > maxLen) {
        const qsizetype size = len * 3 + 4;
        result = new char[size];

        char *const forElipsis = result + size - 5;
        forElipsis[0] = ' ';
        forElipsis[1] = '.';
        forElipsis[2] = '.';
        forElipsis[3] = '.';
        result[size - 1] = '\0';
    }
    else {
        const qsizetype size = len * 3;
        result = new char[size];
        result[size - 1] = '\0';
    }

    qsizetype i = 0;
    qsizetype o = 0;

    while (true) {
        const char at = ba[i];

        result[o] = toHexUpper(at >> 4);
        ++o;
        result[o] = toHexUpper(at);

        ++i;
        ++o;
        if (i == len)
            break;
        result[o] = ' ';
        ++o;
    }

    return result;
}

/*!
    \internal
    Returns the same QByteArray but with only the ASCII characters still shown;
    everything else is replaced with \c {\xHH}.
*/
char *toPrettyCString(const char *p, qsizetype length)
{
    bool trimmed = false;
    auto buffer = std::make_unique<char[]>(256);
    const char *end = p + length;
    char *dst = buffer.get();

    bool lastWasHexEscape = false;
    *dst++ = '"';
    for ( ; p != end; ++p) {
        // we can add:
        //  1 byte: a single character
        //  2 bytes: a simple escape sequence (\n)
        //  3 bytes: "" and a character
        //  4 bytes: an hex escape sequence (\xHH)
        if (dst - buffer.get() > 246) {
            // plus the quote, the three dots and NUL, it's 255 in the worst case
            trimmed = true;
            break;
        }

        // check if we need to insert "" to break an hex escape sequence
        if (Q_UNLIKELY(lastWasHexEscape)) {
            if (fromHex(*p) != -1) {
                // yes, insert it
                *dst++ = '"';
                *dst++ = '"';
            }
            lastWasHexEscape = false;
        }

        if (*p < 0x7f && *p >= 0x20 && *p != '\\' && *p != '"') {
            *dst++ = *p;
            continue;
        }

        // write as an escape sequence
        // this means we may advance dst to buffer.data() + 247 or 250
        *dst++ = '\\';
        switch (*p) {
        case 0x5c:
        case 0x22:
            *dst++ = uchar(*p);
            break;
        case 0x8:
            *dst++ = 'b';
            break;
        case 0xc:
            *dst++ = 'f';
            break;
        case 0xa:
            *dst++ = 'n';
            break;
        case 0xd:
            *dst++ = 'r';
            break;
        case 0x9:
            *dst++ = 't';
            break;
        default:
            // print as hex escape
            *dst++ = 'x';
            *dst++ = toHexUpper(uchar(*p) >> 4);
            *dst++ = toHexUpper(uchar(*p));
            lastWasHexEscape = true;
            break;
        }
    }

    *dst++ = '"';
    if (trimmed) {
        *dst++ = '.';
        *dst++ = '.';
        *dst++ = '.';
    }
    *dst++ = '\0';
    return buffer.release();
}

/*!
    \fn char *toPrettyUnicode(QStringView string)
    \internal
    Returns the same QString but with only the ASCII characters still shown;
    everything else is replaced with \c {\uXXXX}.

    Similar to QDebug::putString().
*/

constexpr qsizetype PrettyUnicodeMaxOutputSize = 256;
// escape sequence, closing quote, the three dots and NUL
constexpr qsizetype PrettyUnicodeMaxIncrement = sizeof(R"(\uXXXX"...)"); // includes NUL

static char *writePrettyUnicodeChar(char16_t ch, char * const buffer)
{
    auto dst = buffer;
    auto first = [&](int n) { Q_ASSERT(dst - buffer == n); return dst; };
    if (ch < 0x7f && ch >= 0x20 && ch != '\\' && ch != '"') {
        *dst++ = ch;
        return first(1);
    }

    // write as an escape sequence
    *dst++ = '\\';
    switch (ch) {
    case 0x22:
    case 0x5c:
        *dst++ = uchar(ch);
        break;
    case 0x8:
        *dst++ = 'b';
        break;
    case 0xc:
        *dst++ = 'f';
        break;
    case 0xa:
        *dst++ = 'n';
        break;
    case 0xd:
        *dst++ = 'r';
        break;
    case 0x9:
        *dst++ = 't';
        break;
    default:
        *dst++ = 'u';
        *dst++ = toHexUpper(ch >> 12);
        *dst++ = toHexUpper(ch >> 8);
        *dst++ = toHexUpper(ch >> 4);
        *dst++ = toHexUpper(ch);
        return first(6);
    }
    return first(2);
}

char *toPrettyUnicode(QStringView string)
{
    auto p = string.utf16();
    auto length = string.size();
    // keep it simple for the vast majority of cases
    bool trimmed = false;
    auto buffer = std::make_unique<char[]>(PrettyUnicodeMaxOutputSize);
    const auto end = p + length;
    char *dst = buffer.get();

    *dst++ = '"';
    for ( ; p != end; ++p) {
        if (dst - buffer.get() > PrettyUnicodeMaxOutputSize - PrettyUnicodeMaxIncrement) {
            trimmed = true;
            break;
        }
        dst = writePrettyUnicodeChar(*p, dst);
    }

    *dst++ = '"';
    if (trimmed) {
        *dst++ = '.';
        *dst++ = '.';
        *dst++ = '.';
    }
    *dst++ = '\0';
    return buffer.release();
}

void TestMethods::invokeTests(QObject *testObject) const
{
    const QMetaObject *metaObject = testObject->metaObject();
    BOBUIEST_ASSERT(metaObject);

    std::optional<WatchDog> watchDog = std::nullopt;
    if (!CrashHandler::alreadyDebugging()
#if BOBUI_CONFIG(valgrind)
        && QBenchmarkGlobalData::current->mode() != QBenchmarkGlobalData::CallgrindChildProcess
#endif
       ) {
        watchDog.emplace();
    }

    BOBUIestResult::setCurrentTestFunction("initTestCase");
    runWithWatchdog(watchDog, [this, testObject] {
        invokeTestMethodIfValid(m_initTestCaseDataMethod, testObject);
    });

    QSignalDumper::startDump();

    if (!BOBUIestResult::skipCurrentTest() && !BOBUIestResult::currentTestFailed()) {

        runWithWatchdog(watchDog, [this, testObject] {
            invokeTestMethodIfValid(m_initTestCaseMethod, testObject);
        });

        // finishedCurrentTestDataCleanup() resets BOBUIestResult::currentTestFailed(), so use a local copy.
        const bool previousFailed = BOBUIestResult::currentTestFailed();
        BOBUIestResult::finishedCurrentTestData();
        BOBUIestResult::finishedCurrentTestDataCleanup();
        BOBUIestResult::finishedCurrentTestFunction();

        if (!BOBUIestResult::skipCurrentTest() && !previousFailed) {
            for (int i = 0, count = int(m_methods.size()); i < count; ++i) {
                const char *data = nullptr;
                if (i < BOBUIest::testTags.size() && !BOBUIest::testTags.at(i).isEmpty())
                    data = qstrdup(BOBUIest::testTags.at(i).toLatin1().constData());
                invokeTest(i, QLatin1StringView(data), watchDog);
                delete [] data;
            }
        }

        const bool wasSkipped = BOBUIestResult::skipCurrentTest();
        BOBUIestResult::setSkipCurrentTest(false);
        BOBUIestResult::setBlacklistCurrentTest(false);
        BOBUIestResult::setCurrentTestFunction("cleanupTestCase");
        runWithWatchdog(watchDog, [this, testObject] {
            invokeTestMethodIfValid(m_cleanupTestCaseMethod, testObject);
        });

        BOBUIestResult::finishedCurrentTestData();
        // Restore skip state as it affects decision on whether we passed:
        BOBUIestResult::setSkipCurrentTest(wasSkipped || BOBUIestResult::skipCurrentTest());
        BOBUIestResult::finishedCurrentTestDataCleanup();
    }
    BOBUIestResult::finishedCurrentTestFunction();
    BOBUIestResult::setCurrentTestFunction(nullptr);

    QSignalDumper::endDump();
}

#if BOBUI_DEPRECATED_SINCE(6, 8)
static const char *functionRefFormatter(const void *f)
{
    auto formatter = static_cast<const qxp::function_ref<const char *()> *>(f);
    return (*formatter)();
};

bool reportResult(bool success, qxp::function_ref<const char *()> lhs,
                  qxp::function_ref<const char *()> rhs,
                  const char *lhsExpr, const char *rhsExpr,
                  ComparisonOperation op, const char *file, int line)
{
    return BOBUIestResult::reportResult(success, &lhs, &rhs,
                                     functionRefFormatter, functionRefFormatter,
                                     lhsExpr, rhsExpr, op, file, line);
}
#endif // BOBUI_DEPRECATED_SINCE(6, 8)

bool reportResult(bool success, const void *lhs, const void *rhs,
                  const char *(*lhsFormatter)(const void*),
                  const char *(*rhsFormatter)(const void*),
                  const char *lhsExpr, const char *rhsExpr,
                  ComparisonOperation op, const char *file, int line)
{
    return BOBUIestResult::reportResult(success, lhs, rhs, lhsFormatter, rhsFormatter,
                                     lhsExpr, rhsExpr, op, file, line);
}
} // namespace BOBUIest

static void initEnvironment()
{
    qputenv("BOBUI_BOBUIESTLIB_RUNNING", "1");
}

#ifdef Q_OS_ANDROID
static QFile androidExitCodeFile()
{
    const QString testHome = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return QFile(testHome + "/bobuiest_last_exit_code"_L1);
}
#endif

/*!
    Executes tests declared in \a testObject. In addition, the private slots
    \c{initTestCase()}, \c{cleanupTestCase()}, \c{init()} and \c{cleanup()}
    are executed if they exist. See \l{Creating a Test} for more details.

    Optionally, the command line arguments \a argc and \a argv can be provided.
    For a list of recognized arguments, read \l {BobUI Test Command Line Arguments}.

    The following example will run all tests in \c MyTestObject:

    \snippet code/src_bobuiestlib_bobuiestcase.cpp 18

    This function returns 0 if no tests failed, or a value other than 0 if one
    or more tests failed or in case of unhandled exceptions.  (Skipped tests do
    not influence the return value.)

    For stand-alone test applications, the convenience macro \l BOBUIEST_MAIN() can
    be used to declare a main() function that parses the command line arguments
    and executes the tests, avoiding the need to call this function explicitly.

    The return value from this function is also the exit code of the test
    application when the \l BOBUIEST_MAIN() macro is used.

    For stand-alone test applications, this function should not be called more
    than once, as command-line options for logging test output to files and
    executing individual test functions will not behave correctly.

    \note This function is not reentrant, only one test can run at a time. A
    test that was executed with qExec() can't run another test via qExec() and
    threads are not allowed to call qExec() simultaneously.

    If you have programmatically created the arguments, as opposed to getting them
    from the arguments in \c main(), it is likely of interest to use
    BOBUIest::qExec(QObject *, const QStringList &) since it is Unicode safe.

    \sa BOBUIEST_MAIN(), BOBUIEST_GUILESS_MAIN(), BOBUIEST_APPLESS_MAIN()
*/

int BOBUIest::qExec(QObject *testObject, int argc, char **argv)
{
    // NB: BobUIQuick's testing recombines qInit(), qRun() and qCleanup() to
    // provide a replacement for qExec() that calls qRun() once for each
    // built-in style. So think twice about moving parts between these three
    // functions, as doing so may mess up BobUIQuick's testing.
    qInit(testObject, argc, argv);
    int ret = qRun();
    qCleanup();

#if defined(Q_OS_WASM)
    EM_ASM({
        if (typeof Module != "undefined" && typeof Module.notifyTestFinished != "undefined")
            Module.notifyTestFinished($0);
    }, ret);
#endif // Q_OS_WASM

    return ret;
}

/*! \internal
*/
void BOBUIest::qInit(QObject *testObject, int argc, char **argv)
{
    initEnvironment();
    CrashHandler::maybeDisableCoreDump();
    QBenchmarkGlobalData::current = new QBenchmarkGlobalData;

#if defined(Q_OS_MACOS)
    // Don't restore saved window state for auto tests
    BOBUIestPrivate::disableWindowRestore();

    // Disable App Nap which may cause tests to stall
    if (!appNapDisabler)
        appNapDisabler.emplace();

    IOPMAssertionCreateWithName(kIOPMAssertionTypeNoDisplaySleep,
                                kIOPMAssertionLevelOn, CFSTR("BobUITest running tests"),
                                &macPowerSavingDisabled);
#endif

    BOBUIestPrivate::parseBlackList();
    BOBUIestResult::reset();

    BOBUIEST_ASSERT(testObject);
    BOBUIEST_ASSERT(!currentTestObject);
    currentTestObject = testObject;

    const QMetaObject *metaObject = testObject->metaObject();
    BOBUIEST_ASSERT(metaObject);

    BOBUIestResult::setCurrentTestObject(metaObject->className());
    if (argc > 0)
        BOBUIestResult::setCurrentAppName(argv[0]);

    bobuiest_qParseArgs(argc, argv, false);

#if BOBUI_CONFIG(valgrind)
    if (QBenchmarkGlobalData::current->mode() != QBenchmarkGlobalData::CallgrindParentProcess)
#endif
        BOBUIestLog::startLogging();

#ifdef Q_OS_ANDROID
    androidExitCodeFile().remove();
#endif
}

/*! \internal
*/
int BOBUIest::qRun()
{
    BOBUIEST_ASSERT(currentTestObject);

#if BOBUI_CONFIG(valgrind)
    int callgrindChildExitCode = 0;
#endif

#ifndef BOBUI_NO_EXCEPTIONS
    try {
#endif

#if BOBUI_CONFIG(valgrind)
    if (QBenchmarkGlobalData::current->mode() == QBenchmarkGlobalData::CallgrindParentProcess) {
        if (Q_UNLIKELY(!qApp))
            qFatal("BobUITest: -callgrind option is not available with BOBUIEST_APPLESS_MAIN");

        const QStringList origAppArgs(QCoreApplication::arguments());
        if (!QBenchmarkValgrindUtils::rerunThroughCallgrind(origAppArgs, callgrindChildExitCode))
            return -1;

        QBenchmarkValgrindUtils::cleanup();

    } else
#endif
    {
        std::optional<CrashHandler::FatalSignalHandler> handler;
        CrashHandler::prepareStackTrace();
        if (!Internal::noCrashHandler)
            handler.emplace();

        bool seenBad = false;
        TestMethods::MetaMethods commandLineMethods;
        commandLineMethods.reserve(static_cast<size_t>(BOBUIest::testFunctions.size()));
        for (const QString &tf : std::as_const(BOBUIest::testFunctions)) {
            const QByteArray tfB = tf.toLatin1();
            const QByteArray signature = tfB + QByteArrayLiteral("()");
            QMetaMethod m = TestMethods::findMethod(currentTestObject, signature.constData());
            if (m.isValid() && isValidSlot(m)) {
                commandLineMethods.push_back(m);
            } else {
                std::fprintf(stderr, "Unknown test function: '%s'.", tfB.constData());
                if (!qPrintTestSlots(stderr, tfB.constData(), " Possible matches:\n"))
                    std::fputc('\n', stderr);
                BOBUIestResult::setCurrentTestFunction(tfB.constData());
                BOBUIestResult::addFailure(qPrintable("Function not found: %1"_L1.arg(tf)));
                BOBUIestResult::finishedCurrentTestFunction();
                // Ditch the tag that came with tf as test function:
                BOBUIest::testTags.remove(commandLineMethods.size());
                seenBad = true;
            }
        }
        if (seenBad) {
            // Provide relevant help to do better next time:
            std::fprintf(stderr, "\n%s -functions\nlists all available test functions.\n\n",
                                 BOBUIestResult::currentAppName());
            if (commandLineMethods.empty()) // All requested functions missing.
                return 1;
        }
        TestMethods test(currentTestObject, std::move(commandLineMethods));

        int remainingRepetitions = repetitions;
        const bool repeatForever = repetitions < 0;
        while (BOBUIestLog::failCount() == 0 && (repeatForever || remainingRepetitions-- > 0)) {
            BOBUIestTable::globalTestTable();
            test.invokeTests(currentTestObject);
            BOBUIestTable::clearGlobalTestTable();
        }
    }

#ifndef BOBUI_NO_EXCEPTIONS
    } catch (...) {
        BOBUIestResult::addFailure("Caught unhandled exception", __FILE__, __LINE__);
        if (BOBUIestResult::currentTestFunction()) {
            BOBUIestResult::finishedCurrentTestFunction();
            BOBUIestResult::setCurrentTestFunction(nullptr);
        }

        qCleanup();

        // Re-throw exception to make debugging easier
        throw;
        return 1;
    }
#endif

#if BOBUI_CONFIG(valgrind)
    if (QBenchmarkGlobalData::current->mode() == QBenchmarkGlobalData::CallgrindParentProcess)
        return callgrindChildExitCode;
#endif
    // make sure our exit code is never going above 127
    // since that could wrap and indicate 0 test fails
    const int exitCode = qMin(BOBUIestLog::failCount(), 127);

#ifdef Q_OS_ANDROID
    QFile exitCodeFile = androidExitCodeFile();
    if (exitCodeFile.open(QIODevice::WriteOnly)) {
        exitCodeFile.write(qPrintable(QString::number(exitCode)));
    } else {
        qWarning("Failed to open %s for writing test exit code: %s",
                 qPrintable(exitCodeFile.fileName()), qPrintable(exitCodeFile.errorString()));
    }
#endif

    return exitCode;
}

/*! \internal
*/
void BOBUIest::qCleanup()
{
    currentTestObject = nullptr;

#if BOBUI_CONFIG(valgrind)
    if (QBenchmarkGlobalData::current->mode() != QBenchmarkGlobalData::CallgrindParentProcess)
#endif
        BOBUIestLog::stopLogging();

    delete QBenchmarkGlobalData::current;
    QBenchmarkGlobalData::current = nullptr;

#if defined(Q_OS_MACOS)
    IOPMAssertionRelease(macPowerSavingDisabled);
    appNapDisabler = std::nullopt;
#endif
}

#if BOBUI_CONFIG(batch_test_support) || defined(Q_QDOC)
/*!
    Registers the test \a name, with entry function \a entryFunction, in a
    central test case registry for the current binary.

    The \a name will be listed when running the batch test binary with no
    parameters. Running the test binary with the argv[1] of \a name will result
    in \a entryFunction being called.

    \since 6.5
*/
void BOBUIest::qRegisterTestCase(const QString &name, TestEntryFunction entryFunction)
{
    BOBUIest::TestRegistry::instance()->registerTest(name, entryFunction);
}

QList<QString> BOBUIest::qGetTestCaseNames()
{
    return BOBUIest::TestRegistry::instance()->getAllTestNames();
}

BOBUIest::TestEntryFunction BOBUIest::qGetTestCaseEntryFunction(const QString& name)
{
    return BOBUIest::TestRegistry::instance()->getTestEntryFunction(name);
}

#endif  // BOBUI_CONFIG(batch_test_support)

/*!
  \overload
  \since 4.4

  Behaves identically to qExec(QObject *, int, char**) but takes a
  QStringList of \a arguments instead of a \c char** list.
*/
int BOBUIest::qExec(QObject *testObject, const QStringList &arguments)
{
    const int argc = arguments.size();
    QVarLengthArray<char *> argv(argc);

    QList<QByteArray> args;
    args.reserve(argc);

    for (int i = 0; i < argc; ++i)
    {
        args.append(arguments.at(i).toLocal8Bit().constData());
        argv[i] = args.last().data();
    }

    return qExec(testObject, argc, argv.data());
}

/*! \internal
*/
void BOBUIest::qFail(const char *message, const char *file, int line)
{
    BOBUIestResult::fail(message, file, line);
}

/*! \internal
*/
bool BOBUIest::qVerify(bool statement, const char *statementStr, const char *description,
                   const char *file, int line)
{
    return BOBUIestResult::verify(statement, statementStr, description, file, line);
}

/*! \fn void BOBUIest::qSkip(const char *message, const char *file, int line)
    \internal
*/
void BOBUIest::qSkip(const char *message, const char *file, int line)
{
    BOBUIestResult::addSkip(message, file, line);
    BOBUIestResult::setSkipCurrentTest(true);
}

/*! \fn bool BOBUIest::qExpectFail(const char *dataIndex, const char *comment, TestFailMode mode, const char *file, int line)
    \internal
*/
bool BOBUIest::qExpectFail(const char *dataIndex, const char *comment,
                       BOBUIest::TestFailMode mode, const char *file, int line)
{
    return BOBUIestResult::expectFail(dataIndex, qstrdup(comment), mode, file, line);
}

/*!
    \internal

    Executes qFail() following a failed QVERIFY_THROWS_EXCEPTION or
    QVERIFY_THROWS_NO_EXCEPTION, passing a suitable message created from \a expected,
    \a what, along with \a file and \a line.

    The \a expected parameter contains the type of the exception that is expected to
    be thrown, or \nullptr, if no exception was expected.

    The \a what parameter contains the result of \c{std::exception::what()}, or nullptr,
    if a non-\c{std::exception}-derived exception was caught.

    The \a file and \a line parameters hold expansions of the \c{__FILE__} and \c{__LINE__}
    macros, respectively.
*/
void BOBUIest::qCaught(const char *expected, const char *what, const char *file, int line)
{
    auto message = [&] {
        const auto exType  = what ? "std::" : "unknown ";
        const auto ofType  = expected ? " of type " : "";
        const auto no      = expected ? "an" : "no";
        const auto withMsg = what ? " with message " : "";
        const auto protect = [](const char *s) { return s ? s : ""; };

        return QString::asprintf("Expected %s exception%s%s to be thrown, "
                                 "but caught %sexception%s%s",
                                 no, ofType, protect(expected),
                                 exType, withMsg, protect(what));
    };
    qFail(message().toUtf8().constData(), file, line);
}

/*!
    \internal

    Contains the implementation of the catch(...) block of
    QVERIFY_THROWS_EXCEPTION.

    The function inspects std::current_exception() by rethrowing it using
    std::rethrow_exception().

    The function must be called from a catch handler.

    If the exception inherits std::exception, its what() message is logged and
    this function returns normally. The caller of this function must then
    execute a \c{BOBUIEST_FAIL_ACTION} to exit from the test function.

    Otherwise, a message saying an unknown exception was caught is logged and
    this function rethrows the exception, skipping the \c{BOBUIEST_FAIL_ACTION}
    that follows this function call in the caller.
*/
void BOBUIest::qCaught(const char *expected, const char *file, int line)
{
    try {
        // let's see what the cat brought us:
        std::rethrow_exception(std::current_exception());
    } catch (const std::exception &e) {
        qCaught(expected, e.what(), file, line);
    } catch (...) {
        qCaught(expected, nullptr, file, line);
        throw;
    }
    // caller shall invoke `BOBUIEST_FAIL_ACTION` if control reached here
}


#if BOBUI_DEPRECATED_SINCE(6, 3)
/*!
  \internal
  \deprecated [6.3] Use qWarning() instead
*/
void BOBUIest::qWarn(const char *message, const char *file, int line)
{
    BOBUIestLog::warn(message, file, line);
}
#endif

/*!
    Ignores messages created by qDebug(), qInfo() or qWarning(). If the \a message
    with the corresponding \a type is outputted, it will be removed from the
    test log. If the test finished and the \a message was not outputted,
    a test failure is appended to the test log.

    \note Invoking this function will only ignore one message. If the message
    you want to ignore is output twice, you have to call ignoreMessage() twice,
    too.

    Example:
    \snippet code/src_bobuiestlib_bobuiestcase.cpp 19

    The example above tests that QDir::mkdir() outputs the right warning when invoked
    with an invalid file name.

    \note \a message will be interpreted as UTF-8.
*/
void BOBUIest::ignoreMessage(BobUIMsgType type, const char *message)
{
    BOBUIestLog::ignoreMessage(type, message);
}

#if BOBUI_CONFIG(regularexpression)
/*!
    \overload

    Ignores messages created by qDebug(), qInfo() or qWarning(). If the message
    matching \a messagePattern
    with the corresponding \a type is outputted, it will be removed from the
    test log. If the test finished and the message was not outputted,
    a test failure is appended to the test log.

    \note Invoking this function will only ignore one message. If the message
    you want to ignore is output twice, you have to call ignoreMessage() twice,
    too.

    \since 5.3
*/
void BOBUIest::ignoreMessage(BobUIMsgType type, const QRegularExpression &messagePattern)
{
    BOBUIestLog::ignoreMessage(type, messagePattern);
}
#endif // BOBUI_CONFIG(regularexpression)

/*!
    \since 6.8
    \overload failOnWarning()

    Appends a test failure to the test log if any warning is output.

    \sa failOnWarning(const char *)
*/
void BOBUIest::failOnWarning()
{
    return BOBUIestLog::failOnWarning();
}

/*!
    \since 6.3
    \overload failOnWarning()

    Appends a test failure to the test log if the \a message is output.

    \sa failOnWarning()
*/
void BOBUIest::failOnWarning(const char *message)
{
    return BOBUIestLog::failOnWarning(message);
}

#if BOBUI_CONFIG(regularexpression)
/*!
    \since 6.3

    Appends a test failure to the test log for each warning that matches
    \a messagePattern.

    The test function will continue execution when a failure is added. To abort
    the test instead, you can check \l currentTestFailed() and return early if
    it's \c true.

    For each warning, the first pattern that matches will cause a failure,
    and the remaining patterns will be ignored.

    All patterns are cleared at the end of each test function.

    \code
    void FileTest::loadFiles()
    {
        BOBUIest::failOnWarning(QRegularExpression("^Failed to load"));

        // Each of these will cause a test failure:
        qWarning() << "Failed to load image";
        qWarning() << "Failed to load video";
    }
    \endcode

    To fail every test that triggers a given warning, pass a suitable regular
    expression to this function in \l {Creating a Test}{init()}:

    \code
    void FileTest::init()
    {
        BOBUIest::failOnWarning(
            QRegularExpression("QFile::.*: File(.*) already open"));
    }
    \endcode

    For the common case of failing on \e any warning pass no parameter:

    \code
    void FileTest::init()
    {
        BOBUIest::failOnWarning();
    }
    \endcode

    \note \l ignoreMessage() takes precedence over this function, so any
    warnings that match a pattern given to both \c ignoreMessage() and
    \c failOnWarning() will be ignored.

    \sa {BobUI Test Environment Variables}{BOBUIEST_FATAL_FAIL}
*/
void BOBUIest::failOnWarning(const QRegularExpression &messagePattern)
{
    BOBUIestLog::failOnWarning(messagePattern);
}
#endif // BOBUI_CONFIG(regularexpression)

/*! \internal
*/

#ifdef Q_OS_WIN
static inline bool isWindowsBuildDirectory(const QString &dirName)
{
    return dirName.compare("Debug"_L1, BobUI::CaseInsensitive) == 0
           || dirName.compare("Release"_L1, BobUI::CaseInsensitive) == 0;
}
#endif

#if BOBUI_CONFIG(temporaryfile)
/*!
    Extracts a directory from resources to disk. The content is extracted
    recursively to a temporary folder. The extracted content is removed
    automatically once the last reference to the return value goes out of scope.

    \a dirName is the name of the directory to extract from resources.

    Returns the temporary directory where the data was extracted or null in case of
    errors.
*/
QSharedPointer<BOBUIemporaryDir> BOBUIest::qExtractTestData(const QString &dirName)
{
      QSharedPointer<BOBUIemporaryDir> result; // null until success, then == tempDir

      QSharedPointer<BOBUIemporaryDir> tempDir = QSharedPointer<BOBUIemporaryDir>::create();

      tempDir->setAutoRemove(true);

      if (!tempDir->isValid())
          return result;

      const QString dataPath = tempDir->path();
      const QString resourcePath = u':' + dirName;
      const QFileInfo fileInfo(resourcePath);

      if (!fileInfo.isDir()) {
          qWarning("Resource path '%s' is not a directory.", qPrintable(resourcePath));
          return result;
      }

      bool isResourceDirEmpty = true;
      for (const auto &dirEntry : QDirListing(resourcePath, QDirListing::IteratorFlag::Recursive)) {
          isResourceDirEmpty = false;
          if (!dirEntry.isDir()) {
              const QString &filePath = dirEntry.filePath();
              const QString destination =
                  dataPath + u'/' + QStringView{filePath}.sliced(resourcePath.size());
              QFileInfo destinationFileInfo(destination);
              QDir().mkpath(destinationFileInfo.path());
              QFile file(filePath);
              if (!file.copy(destination)) {
                  qWarning("Failed to copy '%ls': %ls.", qUtf16Printable(filePath),
                           qUtf16Printable(file.errorString()));
                  return result;
              }

              file.setFileName(destination);
              if (!file.setPermissions(QFile::ReadUser | QFile::WriteUser | QFile::ReadGroup)) {
                  qWarning("Failed to set permissions on '%ls': %ls.", qUtf16Printable(destination),
                           qUtf16Printable(file.errorString()));
                  return result;
              }
          }
      }

      if (isResourceDirEmpty) {
          qWarning("Resource directory '%s' is empty.", qPrintable(resourcePath));
          return result;
      }

      result = std::move(tempDir);

      return result;
}
#endif // BOBUI_CONFIG(temporaryfile)

/*! \internal
*/

QString BOBUIest::qFindTestData(const QString& base, const char *file, int line, const char *builddir,
                             const char *sourcedir)
{
    QString found;

    // Testdata priorities:

    //  1. relative to test binary.
    if (qApp) {
        QDir binDirectory(QCoreApplication::applicationDirPath());
        if (binDirectory.exists(base)) {
            found = binDirectory.absoluteFilePath(base);
        }
#ifdef Q_OS_WIN
        // Windows: The executable is typically located in one of the
        // 'Release' or 'Debug' directories.
        else if (isWindowsBuildDirectory(binDirectory.dirName())
                 && binDirectory.cdUp() && binDirectory.exists(base)) {
            found = binDirectory.absoluteFilePath(base);
        }
#endif // Q_OS_WIN
        else if (BOBUIestLog::verboseLevel() >= 2) {
            const QString candidate = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + u'/' + base);
            BOBUIestLog::info(qPrintable("testdata %1 not found relative to test binary [%2]; "
                                      "checking next location"_L1.arg(base, candidate)),
                           file, line);
        }
    }

    //  2. installed path.
    if (found.isEmpty()) {
        const char *testObjectName = BOBUIestResult::currentTestObjectName();
        if (testObjectName) {
            const QString testsPath = QLibraryInfo::path(QLibraryInfo::TestsPath);
            const QString candidate = "%1/%2/%3"_L1
                .arg(testsPath, QFile::decodeName(testObjectName).toLower(), base);
            if (QFileInfo::exists(candidate)) {
                found = candidate;
            } else if (BOBUIestLog::verboseLevel() >= 2) {
                BOBUIestLog::info(qPrintable("testdata %1 not found in tests install path [%2]; "
                                          "checking next location"_L1
                                          .arg(base, QDir::toNativeSeparators(candidate))),
                               file, line);
            }
        }
    }

    //  3. relative to test source.
    if (found.isEmpty() && qstrncmp(file, ":/", 2) != 0) {
        // srcdir is the directory containing the calling source file.
        QFileInfo srcdir(QFileInfo(QFile::decodeName(file)).path());

        // If the srcdir is relative, that means it is relative to the current working
        // directory of the compiler at compile time, which should be passed in as `builddir'.
        if (!srcdir.isAbsolute() && builddir)
            srcdir.setFile(QFile::decodeName(builddir) + u'/' + srcdir.filePath());

        const QString canonicalPath = srcdir.canonicalFilePath();
        const QString candidate = "%1/%2"_L1.arg(canonicalPath, base);
        if (!canonicalPath.isEmpty() && QFileInfo::exists(candidate)) {
            found = candidate;
        } else if (BOBUIestLog::verboseLevel() >= 2) {
            BOBUIestLog::info(qPrintable(
                "testdata %1 not found relative to source path [%2]"_L1
                    .arg(base, QDir::toNativeSeparators(candidate))),
                file, line);
        }
    }

    // 4. Try resources
    if (found.isEmpty()) {
        const QString candidate = ":/%1"_L1.arg(base);
        if (QFileInfo::exists(candidate)) {
            found = candidate;
        } else if (BOBUIestLog::verboseLevel() >= 2) {
            BOBUIestLog::info(qPrintable(
                "testdata %1 not found in resources [%2]"_L1
                    .arg(base, QDir::toNativeSeparators(candidate))),
                file, line);
        }
    }

    // 5. Try current directory
    if (found.isEmpty()) {
        const QString candidate = QDir::currentPath() + u'/' + base;
        if (QFileInfo::exists(candidate)) {
            found = candidate;
        } else if (BOBUIestLog::verboseLevel() >= 2) {
            BOBUIestLog::info(qPrintable(
                "testdata %1 not found in current directory [%2]"_L1
                    .arg(base, QDir::toNativeSeparators(candidate))),
                file, line);
        }
    }

    // 6. Try main source directory
    if (found.isEmpty()) {
        const QString candidate = BOBUIest::mainSourcePath % u'/' % base;
        if (QFileInfo::exists(candidate)) {
            found = candidate;
        } else if (BOBUIestLog::verboseLevel() >= 2) {
            BOBUIestLog::info(qPrintable(
                "testdata %1 not found in main source directory [%2]"_L1
                    .arg(base, QDir::toNativeSeparators(candidate))),
                file, line);
        }
    }

    // 7. Try the supplied source directory
    if (found.isEmpty() && sourcedir) {
        const QString candidate = QFile::decodeName(sourcedir) % u'/' % base;
        if (QFileInfo::exists(candidate)) {
            found = candidate;
        } else if (BOBUIestLog::verboseLevel() >= 2) {
            BOBUIestLog::info(qPrintable(
                "testdata %1 not found in supplied source directory [%2]"_L1
                    .arg(base, QDir::toNativeSeparators(candidate))),
                file, line);
        }
    }


    if (found.isEmpty()) {
        BOBUIestLog::warn(qPrintable(
            "testdata %1 could not be located!"_L1.arg(base)),
            file, line);
    } else if (BOBUIestLog::verboseLevel() >= 1) {
        BOBUIestLog::info(qPrintable(
            "testdata %1 was located at %2"_L1.arg(base, QDir::toNativeSeparators(found))),
            file, line);
    }

    return found;
}

/*! \internal
*/
QString BOBUIest::qFindTestData(const char *base, const char *file, int line, const char *builddir,
                             const char *sourcedir)
{
    return qFindTestData(QFile::decodeName(base), file, line, builddir, sourcedir);
}

/*! \internal
*/
void *BOBUIest::qData(const char *tagName, int typeId)
{
    return fetchData(BOBUIestResult::currentTestData(), tagName, typeId);
}

/*! \internal
*/
void *BOBUIest::qGlobalData(const char *tagName, int typeId)
{
    return fetchData(BOBUIestResult::currentGlobalTestData(), tagName, typeId);
}

/*! \internal
*/
void *BOBUIest::qElementData(const char *tagName, int metaTypeId)
{
    BOBUIEST_ASSERT(tagName);
    BOBUIestData *data = BOBUIestResult::currentTestData();
    BOBUIEST_ASSERT(data);
    BOBUIEST_ASSERT(data->parent());

    int idx = data->parent()->indexOf(tagName);
    BOBUIEST_ASSERT(idx != -1);
    BOBUIEST_ASSERT(data->parent()->elementTypeId(idx) == metaTypeId);

    return data->data(data->parent()->indexOf(tagName));
}

/*! \internal
*/
void BOBUIest::addColumnInternal(int id, const char *name)
{
    BOBUIestTable *tbl = BOBUIestTable::currentTestTable();
    BOBUIEST_ASSERT_X(tbl, "BOBUIest::addColumn()", "Cannot add testdata outside of a _data slot.");

    tbl->addColumn(id, name);
}

/*!
    Appends a new row to the current test data.

    The test output will identify the test run with this test data using the
    name \a dataTag.

    Returns a BOBUIestData reference that can be used to stream in data, one value
    for each column in the table.

    Example:
    \snippet code/src_bobuiestlib_bobuiestcase.cpp 20

    \note This function can only be called as part of a test's data function
    that is invoked by the test framework.

    See \l {Chapter 2: Data Driven Testing}{Data Driven Testing} for
    a more extensive example.

    \sa addRow(), addColumn(), QFETCH()
*/
BOBUIestData &BOBUIest::newRow(const char *dataTag)
{
    BOBUIEST_ASSERT_X(dataTag, "BOBUIest::newRow()", "Data tag cannot be null");
    BOBUIestTable *tbl = BOBUIestTable::currentTestTable();
    BOBUIEST_ASSERT_X(tbl, "BOBUIest::newRow()", "Cannot add testdata outside of a _data slot.");
    BOBUIEST_ASSERT_X(tbl->elementCount(), "BOBUIest::newRow()",
                   "Must add columns before attempting to add rows.");

    return *tbl->newData(dataTag);
}

/*!
    \since 5.9

    Appends a new row to the current test data.

    The function's arguments are passed to std::snprintf() for
    formatting according to \a format. See the
    \l{https://en.cppreference.com/w/cpp/io/c/fprintf}{std::snprintf()
    documentation} for caveats and limitations.

    The test output will identify the test run with this test data using the
    name that results from this formatting.

    Returns a BOBUIestData reference that can be used to stream in data, one value
    for each column in the table.

    Example:
    \snippet code/src_bobuiestlib_bobuiestcase.cpp addRow

    \note This function can only be called as part of a test's data function
    that is invoked by the test framework.

    See \l {Chapter 2: Data Driven Testing}{Data Driven Testing} for
    a more extensive example.

    \sa newRow(), addColumn(), QFETCH()
*/
BOBUIestData &BOBUIest::addRow(const char *format, ...)
{
    BOBUIEST_ASSERT_X(format, "BOBUIest::addRow()", "Format string cannot be null");
    BOBUIestTable *tbl = BOBUIestTable::currentTestTable();
    BOBUIEST_ASSERT_X(tbl, "BOBUIest::addRow()", "Cannot add testdata outside of a _data slot.");
    BOBUIEST_ASSERT_X(tbl->elementCount(), "BOBUIest::addRow()",
                   "Must add columns before attempting to add rows.");

    char buf[1024];

    va_list va;
    va_start(va, format);
    // we don't care about failures, we accept truncation, as well as trailing garbage.
    // Names with more than 1K characters are nonsense, anyway.
    std::vsnprintf(buf, sizeof buf, format, va);
    va_end(va);

    return *tbl->newData(buf);
}

/*! \fn template <typename T> void BOBUIest::addColumn(const char *name, T *dummy = 0)

    Adds a column with type \c{T} to the current test data.
    \a name is the name of the column. \a dummy is a workaround
    for buggy compilers and can be ignored.

    To populate the column with values, newRow() can be used. Use
    \l QFETCH() to fetch the data in the actual test.

    Example:
    \snippet code/src_bobuiestlib_bobuiestcase.cpp 21

    \note This function can only be used called as part of a test's data
    function that is invoked by the test framework.

    See \l {Chapter 2: Data Driven Testing}{Data Driven Testing} for
    a more extensive example.

    \sa BOBUIest::newRow(), QFETCH(), QMetaType
*/

/*!
    Returns the name of the binary that is currently executed.
*/
const char *BOBUIest::currentAppName()
{
    return BOBUIestResult::currentAppName();
}

/*!
    Returns the name of the test function that is currently executed.

    Example:

    \snippet code/src_bobuiestlib_bobuiestcase.cpp 22
*/
const char *BOBUIest::currentTestFunction()
{
    return BOBUIestResult::currentTestFunction();
}

/*!
    Returns the name of the current test data. If the test doesn't
    have any assigned testdata, the function returns \nullptr.
*/
const char *BOBUIest::currentDataTag()
{
    return BOBUIestResult::currentDataTag();
}

/*!
    Returns the name of the current global test data. If the test doesn't
    have any assigned global testdata, the function returns \nullptr.

    \since 6.11
*/
const char *BOBUIest::currentGlobalDataTag()
{
    return BOBUIestResult::currentGlobalDataTag();
}

/*!
    Returns \c true if the current test function has failed, otherwise false.

    \sa BOBUIest::currentTestResolved()
*/
bool BOBUIest::currentTestFailed()
{
    return BOBUIestResult::currentTestFailed();
}

/*!
    \since 6.5
    Returns \c true if the current test function has failed or skipped.

    This applies if the test has failed or exercised a skip. When it is true,
    the test function should return early. In particular, the \c{BOBUIRY_*} macros
    and the test event loop terminate their loops early if executed during the
    test function (but not its cleanup()). After a test has called a helper
    function that uses this module's macros, it can use this function to test
    whether to return early.

    \sa BOBUIest::currentTestFailed()
*/
bool BOBUIest::currentTestResolved()
{
    return BOBUIestResult::currentTestFailed() || BOBUIestResult::skipCurrentTest();
}

/*!
    \internal
    \since 6.4
    Returns \c true during the run of the test-function and its set-up.

    Used by the \c{BOBUIRY_*} macros and \l BOBUIestEventLoop to check whether to
    return when BOBUIest::currentTestResolved() is true.
*/
bool BOBUIest::runningTest()
{
    return BOBUIest::inTestFunction;
}

/*! \internal
*/
QObject *BOBUIest::testObject()
{
    return currentTestObject;
}

/*! \internal
*/
void BOBUIest::setMainSourcePath(const char *file, const char *builddir)
{
    QString mainSourceFile = QFile::decodeName(file);
    QFileInfo fi;
    if (builddir)
        fi.setFile(QDir(QFile::decodeName(builddir)), mainSourceFile);
    else
        fi.setFile(mainSourceFile);
    BOBUIest::mainSourcePath = fi.absolutePath();
}

#if BOBUI_DEPRECATED_SINCE(6, 4)
/*! \internal
    \deprecated [6.4]
    This function is called by various specializations of BOBUIest::qCompare
    to decide whether to report a failure and to produce verbose test output.

    The failureMsg parameter can be null, in which case a default message
    will be output if the compare fails.  If the compare succeeds, failureMsg
    will not be output.

    Using this function is not optimal, because it requires the string
    representations of \a actualVal and \a expectedVal to be pre-calculated,
    even though they will be used only if the comparison fails. Prefer using the
    \l compare_helper() overload that takes qxp::function_ref() for such cases.

    If the caller creates a custom failure message showing the compared values,
    or if those values cannot be stringified, use the overload of the function
    that takes no \a actualVal and \a expectedVal parameters.
*/
bool BOBUIest::compare_helper(bool success, const char *failureMsg,
                           char *actualVal, char *expectedVal,
                           const char *actual, const char *expected,
                           const char *file, int line)
{
    return BOBUIestResult::compare(success, failureMsg, actualVal, expectedVal,
                                actual, expected, file, line);
}
#endif // BOBUI_DEPRECATED_SINCE(6, 4)

#if BOBUI_DEPRECATED_SINCE(6, 8)
/*! \internal
    \since 6.4
    This function is called by various specializations of BOBUIest::qCompare
    to decide whether to report a failure and to produce verbose test output.

    The \a failureMsg parameter can be \c {nullptr}, in which case a default
    message will be output if the compare fails. If the comparison succeeds,
    \a failureMsg will not be output.

    This overload of the function uses qxp::function_ref to defer conversion of
    \a actualVal and \a expectedVal to strings until that is really needed
    (when the comparison fails). This speeds up test case execution on success.
*/
bool BOBUIest::compare_helper(bool success, const char *failureMsg,
                           qxp::function_ref<const char *()> actualVal,
                           qxp::function_ref<const char *()> expectedVal,
                           const char *actual, const char *expected,
                           const char *file, int line)
{
    return BOBUIestResult::reportResult(success, &actualVal, &expectedVal,
                                     BOBUIest::functionRefFormatter,
                                     BOBUIest::functionRefFormatter, actual, expected,
                                     BOBUIest::ComparisonOperation::CustomCompare,
                                     file, line, failureMsg);
}
#endif // BOBUI_DEPRECATED_SINCE(6, 8)

/*! \internal
    \since 6.8
    This function is called by various specializations of BOBUIest::qCompare
    to decide whether to report a failure and to produce verbose test output.

    The \a failureMsg parameter can be \c {nullptr}, in which case a default
    message will be output if the compare fails. If the comparison succeeds,
    \a failureMsg will not be output.
*/

bool BOBUIest::compare_helper(bool success, const char *failureMsg,
                           const void *actualPtr, const void *expectedPtr,
                           const char *(*actualFormatter)(const void *),
                           const char *(*expectedFormatter)(const void *),
                           const char *actual, const char *expected,
                           const char *file, int line)
{
    return BOBUIestResult::reportResult(success, actualPtr, expectedPtr,
                                     actualFormatter, expectedFormatter,
                                     actual, expected,
                                     BOBUIest::ComparisonOperation::CustomCompare,
                                     file, line, failureMsg);
}


/*! \internal
    \since 6.9
    This function reports the result of a three-way comparison, when needed.

    Aside from logging every check if in verbose mode and reporting an
    unexpected pass when failure was expected, if \a success is \c true
    this produces no output. Otherwise, a failure is reported. The output
    on failure reports the expressions compared, their values, the actual
    result of the comparison and the expected result of comparison, along
    with the supplied failure message \a failureMsg and the \a file and
    \a line number at which the error arose.

    The expressions compared are supplied as \a lhsExpression and
    \a rhsExpression.
    These are combined, with \c{"<=>"}, to obtain the actual comparison
    expression. Their actual values are pointed to by \a lhsPtr and
    \a rhsPtr, which are formatted by \a lhsFormatter and \a rhsFormatter
    as, respectively, \c lhsFormatter(lhsPtr) and \c rhsFormatter(rhsPtr).
    The actual comparison expression is contrasted,
    in the output, with the expected comparison expression
    \a expectedExpression. Their respective values are supplied by
    \a actualOrderPtr and \a expectedOrderPtr pointers, which are
    formatted by \a actualOrderFormatter and \a expectedOrderFormatter.

    If \a failureMsg is \nullptr a default is used. If a formatter
    function returns \a nullptr, the text \c{"<null>"} is used.
*/
bool BOBUIest::compare_3way_helper(bool success, const char *failureMsg,
                                const void *lhsPtr, const void *rhsPtr,
                                const char *(*lhsFormatter)(const void*),
                                const char *(*rhsFormatter)(const void*),
                                const char *lhsExpression, const char *rhsExpression,
                                const char *(*actualOrderFormatter)(const void *),
                                const char *(*expectedOrderFormatter)(const void *),
                                const void *actualOrderPtr, const void *expectedOrderPtr,
                                const char *expectedExpression,
                                const char *file, int line)
{
    return BOBUIestResult::report3WayResult(success, failureMsg,
                                         lhsPtr, rhsPtr,
                                         lhsFormatter, rhsFormatter,
                                         lhsExpression, rhsExpression,
                                         actualOrderFormatter,
                                         expectedOrderFormatter,
                                         actualOrderPtr, expectedOrderPtr,
                                         expectedExpression,
                                         file, line);
}

/*! \internal
    \since 6.4
    This function is called by various specializations of BOBUIest::qCompare
    to decide whether to report a failure and to produce verbose test output.

    This overload should be used when there is no string representation of
    actual and expected values, so only the \a failureMsg is shown when the
    comparison fails. Because of that, \a failureMsg can't be \c {nullptr}.
    If the comparison succeeds, \a failureMsg will not be output.
*/
bool BOBUIest::compare_helper(bool success, const char *failureMsg, const char *actual,
                           const char *expected, const char *file, int line)
{
    return BOBUIestResult::compare(success, failureMsg, actual, expected, file, line);
}

template <typename T>
static bool floatingCompare(const T &actual, const T &expected)
{
    switch (qFpClassify(expected))
    {
    case FP_INFINITE:
        return (expected < 0) == (actual < 0) && qFpClassify(actual) == FP_INFINITE;
    case FP_NAN:
        return qFpClassify(actual) == FP_NAN;
    default:
        if (!qFuzzyIsNull(expected))
            return qFuzzyCompare(actual, expected);
        Q_FALLTHROUGH();
    case FP_SUBNORMAL: // subnormal is always fuzzily null
    case FP_ZERO:
        return qFuzzyIsNull(actual);
    }
}

/*! \fn bool BOBUIest::qCompare(const qfloat16 &t1, const qfloat16 &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/
bool BOBUIest::qCompare(qfloat16 const &t1, qfloat16 const &t2, const char *actual, const char *expected,
                     const char *file, int line)
{
    auto formatter = Internal::genericToString<qfloat16>;
    return compare_helper(floatingCompare(t1, t2),
                          "Compared qfloat16s are not the same (fuzzy compare)",
                          &t1, &t2, formatter, formatter,
                          actual, expected, file, line);
}

/*! \fn bool BOBUIest::qCompare(const float &t1, const float &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/
bool BOBUIest::qCompare(float const &t1, float const &t2, const char *actual, const char *expected,
                     const char *file, int line)
{
    return BOBUIestResult::compare(floatingCompare(t1, t2),
                                "Compared floats are not the same (fuzzy compare)",
                                t1, t2, actual, expected, file, line);
}

/*! \fn bool BOBUIest::qCompare(const double &t1, const double &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/
bool BOBUIest::qCompare(double const &t1, double const &t2, const char *actual, const char *expected,
                     const char *file, int line)
{
    return BOBUIestResult::compare(floatingCompare(t1, t2),
                                "Compared doubles are not the same (fuzzy compare)",
                                t1, t2, actual, expected, file, line);
}

/*! \fn bool BOBUIest::qCompare(int t1, int t2, const char *actual, const char *expected, const char *file, int line)
    \internal
    \since 5.14
*/
bool BOBUIest::qCompare(int t1, int t2, const char *actual, const char *expected,
                    const char *file, int line)
{
    return BOBUIestResult::compare(t1 == t2,
                                "Compared values are not the same",
                                t1, t2, actual, expected, file, line);
}

#if BOBUI_POINTER_SIZE == 8
/*! \fn bool BOBUIest::qCompare(qsizetype t1, qsizetype t2, const char *actual, const char *expected, const char *file, int line)
    \internal
    \since 6.0
*/

bool BOBUIest::qCompare(qsizetype t1, qsizetype t2, const char *actual, const char *expected,
                     const char *file, int line)
{
    return BOBUIestResult::compare(t1 == t2,
                                "Compared values are not the same",
                                t1, t2, actual, expected, file, line);
}
#endif // BOBUI_POINTER_SIZE == 8

/*! \fn bool BOBUIest::qCompare(unsigned t1, unsigned t2, const char *actual, const char *expected, const char *file, int line)
    \internal
    \since 5.14
*/
bool BOBUIest::qCompare(unsigned t1, unsigned t2, const char *actual, const char *expected,
                     const char *file, int line)
{
    return BOBUIestResult::compare(t1 == t2,
                                "Compared values are not the same",
                                t1, t2, actual, expected, file, line);
}

/*! \fn bool BOBUIest::qCompare(QStringView t1, QStringView t2, const char *actual, const char *expected, const char *file, int line)
    \internal
    \since 5.14
*/
bool BOBUIest::qCompare(QStringView t1, QStringView t2, const char *actual, const char *expected,
                     const char *file, int line)
{
    return BOBUIestResult::compare(t1 == t2,
                                "Compared values are not the same",
                                t1, t2, actual, expected, file, line);
}

/*!
    \internal
    \since 5.14
*/
bool BOBUIest::qCompare(QStringView t1, const QLatin1StringView &t2, const char *actual, const char *expected,
                     const char *file, int line)
{
    return BOBUIestResult::compare(t1 == t2,
                                "Compared values are not the same",
                                t1, t2, actual, expected, file, line);
}

/*!
    \internal
    \since 5.14
*/
bool BOBUIest::qCompare(const QLatin1StringView &t1, QStringView t2, const char *actual, const char *expected,
                     const char *file, int line)
{
    return BOBUIestResult::compare(t1 == t2,
                                "Compared values are not the same",
                                t1, t2, actual, expected, file, line);
}

/*! \fn bool BOBUIest::qCompare(const QString &t1, const QString &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
    \since 5.14
*/

/*! \fn bool BOBUIest::qCompare(const QString &t1, const QLatin1StringView &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
    \since 5.14
*/

/*! \fn bool BOBUIest::qCompare(const QLatin1StringView &t1, const QString &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
    \since 5.14
*/

/*! \fn bool BOBUIest::qCompare(const double &t1, const float &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(const float &t1, const double &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

#define TO_STRING_IMPL(TYPE, FORMAT) \
template <> Q_TESTLIB_EXPORT char *BOBUIest::toString<TYPE>(const TYPE &t) \
{ \
    char *msg = new char[128]; \
    std::snprintf(msg, 128, #FORMAT, t); \
    return msg; \
}

TO_STRING_IMPL(short, %hd)
TO_STRING_IMPL(ushort, %hu)
TO_STRING_IMPL(int, %d)
TO_STRING_IMPL(uint, %u)
TO_STRING_IMPL(long, %ld)
TO_STRING_IMPL(ulong, %lu)
TO_STRING_IMPL(qint64, %lld)
TO_STRING_IMPL(quint64, %llu)
TO_STRING_IMPL(bool, %d)
TO_STRING_IMPL(signed char, %hhd)
TO_STRING_IMPL(unsigned char, %hhu)

// Merge of ISO C23 getpayload() and issignaling()
template <typename T> static auto decodeNanPayload(T t)
{
    constexpr int Digits = std::numeric_limits<T>::digits;
    constexpr quint64 MantissaMask = (Q_UINT64_C(1) << (Digits - 1)) - 1;
    constexpr quint64 IsQuietBit = quint64(BOBUI_CONFIG(signaling_nan)) << (Digits - 2);
    constexpr quint64 PayloadMask = MantissaMask & ~IsQuietBit;

    struct R {
        quint64 payload;
        bool isQuiet;
    } r;
    Q_ASSERT(qIsNaN(t));
    quint64 u = qFromUnaligned<typename QIntegerForSizeof<T>::Unsigned>(&t);
    r.payload = u & PayloadMask;
    r.isQuiet = !BOBUI_CONFIG(signaling_nan) || (u & IsQuietBit);
    return r;
}

// Be consistent about display of infinities and NaNs (snprintf()'s varies,
// notably on MinGW, despite POSIX documenting "[-]inf" or "[-]infinity" for %f,
// %e and %g, uppercasing for their capital versions; similar for "nan"):
template <typename T> static char *toStringFp(T t)
{
    using std::signbit;
    char *msg = new char[128];
    bool negative = signbit(t);

    switch (qFpClassify(t)) {
    case FP_INFINITE:
        qstrncpy(msg, (negative ? "-inf" : "inf"), 128);
        break;
    case FP_NAN:
        if (auto r = decodeNanPayload(t); r.payload) {
            std::snprintf(msg, 128, "%s%snan(%#llx)",
                          negative ? "-" : "", r.isQuiet ? "" : "s", r.payload);
        } else {
            Q_ASSERT(r.isQuiet);  // only quiet NaNs can have payload == 0
            qstrncpy(msg, (negative ? "-nan" : "nan"), 128);
        }
        break;
    case FP_ZERO:
        qstrncpy(msg, (negative ? "-0 (-0x0p+0)" : "0 (0x0p+0)"), 128);
        break;
    default:
        std::snprintf(msg, 128, "%.*g (%a)", std::numeric_limits<T>::digits10 + 1, double(t),
                      double(t));
        break;
    }
    return msg;
}

#define TO_STRING_FLOAT(TYPE) \
template <> Q_TESTLIB_EXPORT char *BOBUIest::toString<TYPE>(const TYPE &t) \
{ \
    return toStringFp(t); \
}
TO_STRING_FLOAT(qfloat16)
TO_STRING_FLOAT(float)
TO_STRING_FLOAT(double)

template <> Q_TESTLIB_EXPORT char *BOBUIest::toString<char>(const char &t)
{
    unsigned char c = static_cast<unsigned char>(t);
    char *msg = new char[16];
    switch (c) {
    case 0x00:
        qstrcpy(msg, "'\\0'");
        break;
    case 0x07:
        qstrcpy(msg, "'\\a'");
        break;
    case 0x08:
        qstrcpy(msg, "'\\b'");
        break;
    case 0x09:
        qstrcpy(msg, "'\\t'");
        break;
    case 0x0a:
        qstrcpy(msg, "'\\n'");
        break;
    case 0x0b:
        qstrcpy(msg, "'\\v'");
        break;
    case 0x0c:
        qstrcpy(msg, "'\\f'");
        break;
    case 0x0d:
        qstrcpy(msg, "'\\r'");
        break;
    case 0x22:
        qstrcpy(msg, "'\\\"'");
        break;
    case 0x27:
        qstrcpy(msg, "'\\\''");
        break;
    case 0x5c:
        qstrcpy(msg, "'\\\\'");
        break;
    default:
        if (c < 0x20 || c >= 0x7F)
            std::snprintf(msg, 16, "'\\x%02x'", c);
        else
            std::snprintf(msg, 16, "'%c'" , c);
    }
    return msg;
}

/*! \internal
*/
char *BOBUIest::toString(const char *str)
{
    if (!str) {
        char *msg = new char[1];
        *msg = '\0';
        return msg;
    }
    char *msg = new char[strlen(str) + 1];
    return qstrcpy(msg, str);
}

/*! \internal
*/
char *BOBUIest::toString(const volatile void *p) // Use volatile to match compare_ptr_helper()
{
    char *msg = new char[128];
    std::snprintf(msg, 128, "%p", p);
    return msg;
}

/*! \internal
*/
char *BOBUIest::toString(const volatile QObject *vo)
{
    if (vo == nullptr)
        return qstrdup("<null>");

    return BOBUIest::toString(const_cast<const QObject*>(vo));
}

/*! \internal
*/
char *BOBUIest::toString(const QObject *o)
{
    if (o == nullptr)
        return qstrdup("<null>");

    const QString &name = o->objectName();
    const char *className = o->metaObject()->className();
    char *msg = new char[256];
    if (name.isEmpty())
        std::snprintf(msg, 256, "%s/%p", className, o);
    else
        std::snprintf(msg, 256, "%s/\"%s\"", className, qPrintable(name));
    return msg;
}


/*! \fn char *BOBUIest::toString(const QColor &color)
    \internal
*/

/*! \fn char *BOBUIest::toString(const QRegion &region)
    \internal
*/

/*! \fn char *BOBUIest::toString(const QHostAddress &addr)
    \internal
*/

/*! \fn char *BOBUIest::toString(QNetworkReply::NetworkError code)
    \internal
*/

/*! \fn char *BOBUIest::toString(const QNetworkCookie &cookie)
    \internal
*/

/*! \fn char *BOBUIest::toString(const QList<QNetworkCookie> &list)
    \internal
*/

/*! \internal
*/
bool BOBUIest::compare_string_helper(const char *t1, const char *t2, const char *actual,
                                  const char *expected, const char *file, int line)
{
    auto formatter = Internal::genericToString<const char *>;
    return compare_helper(qstrcmp(t1, t2) == 0, "Compared strings are not the same",
                          &t1, &t2, formatter, formatter,
                          actual, expected, file, line);
}

/*!
   \namespace BOBUIest::Internal
   \internal
*/

/*! \fn bool BOBUIest::compare_ptr_helper(const volatile void *t1, const volatile void *t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::compare_ptr_helper(const volatile void *t1, std::nullptr_t, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::compare_ptr_helper(std::nullptr_t, const volatile void *t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn template <typename T1, typename T2> bool BOBUIest::qCompare(const T1 &t1, const T2 &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(const QIcon &t1, const QIcon &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(const QImage &t1, const QImage &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(const QPixmap &t1, const QPixmap &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn template <typename T> bool BOBUIest::qCompare(const T &t1, const T &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn template <typename T> bool BOBUIest::qCompare(const T *t1, const T *t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn template <typename T> bool BOBUIest::qCompare(T *t, std::nullptr_t, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn template <typename T> bool BOBUIest::qCompare(std::nullptr_t, T *t, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn template <typename T> bool BOBUIest::qCompare(T *t1, T *t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn template <typename T1, typename T2> bool BOBUIest::qCompare(const T1 *t1, const T2 *t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn template <typename T1, typename T2> bool BOBUIest::qCompare(T1 *t1, T2 *t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(const char *t1, const char *t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(char *t1, char *t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(char *t1, const char *t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(const char *t1, char *t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(const QString &t1, const QLatin1StringView &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(const QLatin1StringView &t1, const QString &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(const QStringList &t1, const QStringList &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn  template <typename T> bool BOBUIest::qCompare(const QList<T> &t1, const QList<T> &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn template <typename T> bool BOBUIest::qCompare(const QFlags<T> &t1, const T &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn template <typename T> bool BOBUIest::qCompare(const QFlags<T> &t1, const int &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(const qint64 &t1, const qint32 &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(const qint64 &t1, const quint32 &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(const quint64 &t1, const quint32 &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(const qint32 &t1, const qint64 &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(const quint32 &t1, const qint64 &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn bool BOBUIest::qCompare(const quint32 &t1, const quint64 &t2, const char *actual, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn  template <typename T> bool BOBUIest::qTest(const T& actual, const char *elementName, const char *actualStr, const char *expected, const char *file, int line)
    \internal
*/

/*! \fn void BOBUIest::sendKeyEvent(KeyAction action, QWidget *widget, BobUI::Key code, QString text, BobUI::KeyboardModifiers modifier, int delay=-1)
    \internal
*/

/*! \fn void BOBUIest::sendKeyEvent(KeyAction action, QWindow *window, BobUI::Key code, QString text, BobUI::KeyboardModifiers modifier, int delay=-1)
    \internal
*/

/*! \fn void BOBUIest::sendKeyEvent(KeyAction action, QWidget *widget, BobUI::Key code, char ascii, BobUI::KeyboardModifiers modifier, int delay=-1)
    \internal
*/

/*! \fn void BOBUIest::sendKeyEvent(KeyAction action, QWindow *window, BobUI::Key code, char ascii, BobUI::KeyboardModifiers modifier, int delay=-1)
    \internal
*/

/*! \fn void BOBUIest::simulateEvent(QWidget *widget, bool press, int code, BobUI::KeyboardModifiers modifier, QString text, bool repeat, int delay=-1)
    \internal
*/

/*! \fn void BOBUIest::simulateEvent(QWindow *window, bool press, int code, BobUI::KeyboardModifiers modifier, QString text, bool repeat, int delay=-1)
    \internal
*/

BOBUI_END_NAMESPACE
