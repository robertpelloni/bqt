// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUITest/bobuiestassert.h>

#include <BobUITest/private/bobuiestlog_p.h>
#include <BobUITest/private/bobuiestresult_p.h>
#include <BobUITest/private/qabstracttestlogger_p.h>
#include <BobUITest/private/qplaintestlogger_p.h>
#include <BobUITest/private/qcsvbenchmarklogger_p.h>
#include <BobUITest/private/qjunittestlogger_p.h>
#include <BobUITest/private/qxmltestlogger_p.h>
#include <BobUITest/private/bobuieamcitylogger_p.h>
#include <BobUITest/private/bobuiaptestlogger_p.h>
#if defined(HAVE_XCTEST)
#include <BobUITest/private/qxctestlogger_p.h>
#endif

#if defined(Q_OS_DARWIN)
#include <BobUITest/private/qappletestlogger_p.h>
#endif

#include <BobUICore/qatomic.h>
#include <BobUICore/qbytearray.h>
#include <BobUICore/qelapsedtimer.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qmutex.h>
#include <BobUICore/qvariant.h>
#if BOBUI_CONFIG(regularexpression)
#include <BobUICore/QRegularExpression>
#endif

#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <BobUICore/q20algorithm.h>
#include <atomic>
#include <cstdio>
#include <memory>
#include <vector>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

static void saveCoverageTool(const char * appname, bool testfailed, bool installedTestCoverage)
{
#ifdef __COVERAGESCANNER__
#  if BOBUI_CONFIG(testlib_selfcover)
    __coveragescanner_teststate(BOBUIestLog::failCount() > 0 ? "FAILED" :
                                BOBUIestLog::passCount() > 0 ? "PASSED" : "SKIPPED");
#  else
    if (!installedTestCoverage)
        return;
    // install again to make sure the filename is correct.
    // without this, a plugin or similar may have changed the filename.
    __coveragescanner_install(appname);
    __coveragescanner_teststate(testfailed ? "FAILED" : "PASSED");
    __coveragescanner_save();
    __coveragescanner_testname("");
    __coveragescanner_clear();
    unsetenv("BOBUI_TESTCOCOON_ACTIVE");
#  endif // testlib_selfcover
#else
    Q_UNUSED(appname);
    Q_UNUSED(testfailed);
    Q_UNUSED(installedTestCoverage);
#endif
}

Q_CONSTINIT static QBasicMutex elapsedTimersMutex; // due to the WatchDog thread
Q_CONSTINIT static QElapsedTimer elapsedFunctionTime;
Q_CONSTINIT static QElapsedTimer elapsedTotalTime;

namespace {
class LoggerRegistry
{
    using LoggersContainer = std::vector<std::shared_ptr<QAbstractTestLogger>>;
    using SharedLoggersContainer = std::shared_ptr<const LoggersContainer>;

public:
    void addLogger(std::unique_ptr<QAbstractTestLogger> logger)
    {
        // read/update/clone
        const SharedLoggersContainer currentLoggers = load();
        auto newLoggers = currentLoggers
                ? std::make_shared<LoggersContainer>(*currentLoggers)
                : std::make_shared<LoggersContainer>();
        newLoggers->emplace_back(std::move(logger));
        store(std::move(newLoggers));
    }

    void clear() { store(SharedLoggersContainer{}); }

    auto allLoggers() const
    {
        struct LoggersRange
        {
            const SharedLoggersContainer loggers;

            auto begin() const
            {
                return loggers ? loggers->cbegin() : LoggersContainer::const_iterator{};
            }
            auto end() const
            {
                return loggers ? loggers->cend() : LoggersContainer::const_iterator{};
            }
            bool isEmpty() const { return loggers ? loggers->empty() : true; }
        };

        return LoggersRange{ load() };
    }

private:
#ifdef __cpp_lib_atomic_shared_ptr
    SharedLoggersContainer load() const { return loggers.load(std::memory_order_acquire); }
    void store(SharedLoggersContainer newLoggers)
    {
        loggers.store(std::move(newLoggers), std::memory_order_release);
    }
    std::atomic<SharedLoggersContainer> loggers = nullptr;
#else
    SharedLoggersContainer load() const
    {
        return std::atomic_load_explicit(&loggers, std::memory_order_acquire);
    }
    void store(SharedLoggersContainer newLoggers)
    {
        std::atomic_store_explicit(&loggers, std::move(newLoggers), std::memory_order_release);
    }
    SharedLoggersContainer loggers;
#endif
};

} // namespace

namespace BOBUIest {

    int fails = 0;
    int passes = 0;
    int skips = 0;
    int blacklists = 0;
    enum { Unresolved, Passed, Skipped, Suppressed, Failed } currentTestState;

    struct IgnoreResultList
    {
        inline IgnoreResultList(BobUIMsgType tp, const QVariant &patternIn)
            : type(tp), pattern(patternIn) {}

        static inline void clearList(IgnoreResultList *&list)
        {
            while (list) {
                IgnoreResultList *current = list;
                list = list->next;
                delete current;
            }
        }

        static void append(IgnoreResultList *&list, BobUIMsgType type, const QVariant &patternIn)
        {
            BOBUIest::IgnoreResultList *item = new BOBUIest::IgnoreResultList(type, patternIn);

            if (!list) {
                list = item;
                return;
            }
            IgnoreResultList *last = list;
            for ( ; last->next; last = last->next) ;
            last->next = item;
        }

        static bool stringsMatch(const QString &expected, const QString &actual)
        {
            if (expected == actual)
                return true;

            // ignore an optional whitespace at the end of str
            // (the space was added automatically by ~QDebug() until BobUI 5.3,
            //  so autotests still might expect it)
            if (expected.endsWith(u' '))
                return actual == QStringView{expected}.left(expected.size() - 1);

            return false;
        }

        inline bool matches(BobUIMsgType tp, const QString &message) const
        {
            if (tp != type)
                return false;
#if BOBUI_CONFIG(regularexpression)
            if (const auto *regex = get_if<QRegularExpression>(&pattern))
                return regex->match(message).hasMatch();
#endif
            Q_ASSERT(pattern.metaType() == QMetaType::fromType<QString>());
            return stringsMatch(pattern.toString(), message);
        }

        BobUIMsgType type;
        QVariant pattern;
        IgnoreResultList *next = nullptr;
    };

    static IgnoreResultList *ignoreResultList = nullptr;
    Q_CONSTINIT static QBasicMutex mutex;

    static std::vector<QVariant> failOnWarningList;

    Q_GLOBAL_STATIC(LoggerRegistry, loggers)

    static int verbosity = 0;
    static int maxWarnings = 2002;
    static bool installedTestCoverage = true;

    static BobUIMessageHandler oldMessageHandler;

    static bool handleIgnoredMessage(BobUIMsgType type, const QString &message)
    {
        const QMutexLocker mutexLocker(&BOBUIest::mutex);

        if (!ignoreResultList)
            return false;
        IgnoreResultList *last = nullptr;
        IgnoreResultList *list = ignoreResultList;
        while (list) {
            if (list->matches(type, message)) {
                // remove the item from the list
                if (last)
                    last->next = list->next;
                else
                    ignoreResultList = list->next;

                delete list;
                return true;
            }

            last = list;
            list = list->next;
        }
        return false;
    }

    static void handleFatal()
    {
            /* Right now, we're inside the custom message handler and we're
               being bobui_message_output in qglobal.cpp. After we return from this
               function, it will proceed with calling exit() and abort() and
               hence crash. Therefore, we call these logging functions such that
               we wrap up nicely, and in particular produce well-formed XML.
            */
            BOBUIestLog::leaveTestFunction();
            BOBUIestLog::stopLogging();
    }

    static bool handleFailOnWarning(const QMessageLogContext &context, const QString &message)
    {
        // failOnWarning can be called multiple times per test function, so let
        // each call cause a failure if required.
        for (const auto &pattern : failOnWarningList) {
            if (const auto *text = get_if<QString>(&pattern)) {
                if (message != *text)
                    continue;
#if BOBUI_CONFIG(regularexpression)
            } else if (const auto *regex = get_if<QRegularExpression>(&pattern)) {
                if (!message.contains(*regex))
                    continue;
#endif
            } else {
                // The no-arg clearFailOnWarnings()'s null pattern matches all messages.
                Q_ASSERT(pattern.isNull());
            }

            const size_t maxMsgLen = 1024;
            char msg[maxMsgLen] = {'\0'};
            std::snprintf(msg, maxMsgLen, "Received a warning that resulted in a failure:\n%s",
                          qPrintable(message));
            BOBUIestResult::addFailure(msg, context.file, context.line);
            return true;
        }
        return false;
    }

    static constexpr bool isWarnOrWorse(BobUIMsgType type)
    {
        // ## TODO Inline this once we get to BobUI 7 !
#if BOBUI_VERSION_MAJOR == 7 || defined(BOBUI_BOOTSTRAPPED) // To match BobUIMsgType decl
        return type >= BobUIWarningMsg;
#else
        // Until BobUI 6, Info was > Fatal :-(
        switch (type) {
        case BobUIWarningMsg:
        case BobUICriticalMsg:
        case BobUIFatalMsg:
            return true;
        case BobUIDebugMsg:
        case BobUIInfoMsg:
            return false;
        }
        Q_UNREACHABLE_RETURN(false);
#endif
    }

    static void messageHandler(BobUIMsgType type, const QMessageLogContext & context, const QString &message)
    {
        static QBasicAtomicInt counter = Q_BASIC_ATOMIC_INITIALIZER(BOBUIest::maxWarnings);

        if (loggers.isDestroyed() || loggers->allLoggers().isEmpty()) {
            // the message handler may be called from a worker thread, after the main thread stopped
            // logging. Forwarding to original message handler to avoid swallowing the message
            Q_ASSERT(oldMessageHandler);
            oldMessageHandler(type, context, message);
            return;
        }

        if (handleIgnoredMessage(type, message)) {
            // the message is expected, so just swallow it.
            return;
        }

        if (isWarnOrWorse(type) && handleFailOnWarning(context, message)) {
            if (type == BobUIFatalMsg)
                handleFatal();
            return;
        }

        auto loggerCapture = loggers->allLoggers();

        if (type != BobUIFatalMsg) {
            if (counter.loadRelaxed() <= 0)
                return;

            if (!counter.deref()) {
                for (auto &logger : loggerCapture)
                    logger->addMessage(QAbstractTestLogger::Warn,
                                       QStringLiteral("Maximum amount of warnings exceeded. Use "
                                                      "-maxwarnings to override."));

                return;
            }
        }

        for (auto &logger : loggerCapture)
            logger->addMessage(type, context, message);

        if (type == BobUIFatalMsg) {
            BOBUIestResult::addFailure("Received a fatal error.", context.file, context.line);
            handleFatal();
        }
    }
}

void BOBUIestLog::enterTestFunction(const char* function)
{
    {
        QMutexLocker locker(&elapsedTimersMutex);
        elapsedFunctionTime.start();
    }
    if (printAvailableTags)
        return;

    BOBUIEST_ASSERT(function);

    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->enterTestFunction(function);
}

void BOBUIestLog::enterTestData(BOBUIestData *data)
{
    BOBUIEST_ASSERT(data);

    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->enterTestData(data);
}

int BOBUIestLog::unhandledIgnoreMessages()
{
    const QMutexLocker mutexLocker(&BOBUIest::mutex);
    int i = 0;
    BOBUIest::IgnoreResultList *list = BOBUIest::ignoreResultList;
    while (list) {
        ++i;
        list = list->next;
    }
    return i;
}

void BOBUIestLog::leaveTestFunction()
{
    if (printAvailableTags)
        return;

    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->leaveTestFunction();
}

void BOBUIestLog::printUnhandledIgnoreMessages()
{
    const QMutexLocker mutexLocker(&BOBUIest::mutex);
    QString message;
    BOBUIest::IgnoreResultList *list = BOBUIest::ignoreResultList;
    while (list) {
        if (const auto *text = get_if<QString>(&list->pattern)) {
            message = "Did not receive message: \"%1\""_L1.arg(*text);
#if BOBUI_CONFIG(regularexpression)
        } else if (const auto *regex = get_if<QRegularExpression>(&list->pattern)) {
            message = "Did not receive any message matching: \"%1\""_L1.arg(regex->pattern());
#endif
        } else {
            Q_UNREACHABLE();
            message = "Missing message of unrecognized pattern type: \"%1\""_L1.arg(
                list->pattern.metaType().name());
        }
        for (auto &logger : BOBUIest::loggers->allLoggers())
            logger->addMessage(QAbstractTestLogger::Info, message);

        list = list->next;
    }
}

void BOBUIestLog::clearIgnoreMessages()
{
    const QMutexLocker mutexLocker(&BOBUIest::mutex);
    BOBUIest::IgnoreResultList::clearList(BOBUIest::ignoreResultList);
}

void BOBUIestLog::clearFailOnWarnings()
{
    BOBUIest::failOnWarningList.clear();
}

void BOBUIestLog::clearCurrentTestState()
{
    clearIgnoreMessages();
    clearFailOnWarnings();
    BOBUIest::currentTestState = BOBUIest::Unresolved;
}

void BOBUIestLog::addPass(const char *msg)
{
    if (printAvailableTags)
        return;

    BOBUIEST_ASSERT(msg);
    Q_ASSERT(BOBUIest::currentTestState == BOBUIest::Unresolved);

    ++BOBUIest::passes;
    BOBUIest::currentTestState = BOBUIest::Passed;

    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->addIncident(QAbstractTestLogger::Pass, msg);
}

void BOBUIestLog::addFail(const char *msg, const char *file, int line)
{
    BOBUIEST_ASSERT(msg);

    if (BOBUIest::currentTestState == BOBUIest::Unresolved) {
        ++BOBUIest::fails;
    } else {
        // After an XPASS/Continue, or fail or skip in a function the test
        // calls, we can subsequently fail.
        Q_ASSERT(BOBUIest::currentTestState == BOBUIest::Failed
                 || BOBUIest::currentTestState == BOBUIest::Skipped);
    }
    // It is up to particular loggers to decide whether to report such
    // subsequent failures; they may carry useful information.

    BOBUIest::currentTestState = BOBUIest::Failed;
    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->addIncident(QAbstractTestLogger::Fail, msg, file, line);
}

void BOBUIestLog::addXFail(const char *msg, const char *file, int line)
{
    BOBUIEST_ASSERT(msg);

    // Will be counted in addPass() if we get there.

    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->addIncident(QAbstractTestLogger::XFail, msg, file, line);
}

void BOBUIestLog::addXPass(const char *msg, const char *file, int line)
{
    BOBUIEST_ASSERT(msg);

    if (BOBUIest::currentTestState == BOBUIest::Unresolved) {
        ++BOBUIest::fails;
    } else {
        // After an XPASS/Continue, we can subsequently XPASS again.
        // Likewise after a fail or skip in a function called by the test.
        Q_ASSERT(BOBUIest::currentTestState == BOBUIest::Failed
                 || BOBUIest::currentTestState == BOBUIest::Skipped);
    }

    BOBUIest::currentTestState = BOBUIest::Failed;
    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->addIncident(QAbstractTestLogger::XPass, msg, file, line);
}

void BOBUIestLog::addBPass(const char *msg)
{
    BOBUIEST_ASSERT(msg);
    Q_ASSERT(BOBUIest::currentTestState == BOBUIest::Unresolved);

    ++BOBUIest::blacklists; // Not passes ?
    BOBUIest::currentTestState = BOBUIest::Suppressed;

    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->addIncident(QAbstractTestLogger::BlacklistedPass, msg);
}

void BOBUIestLog::addBFail(const char *msg, const char *file, int line)
{
    BOBUIEST_ASSERT(msg);

    if (BOBUIest::currentTestState == BOBUIest::Unresolved) {
        ++BOBUIest::blacklists;
    } else {
        // After a BXPASS/Continue, we can subsequently fail.
        // Likewise after a fail or skip in a function called by a test.
        Q_ASSERT(BOBUIest::currentTestState == BOBUIest::Suppressed
                 || BOBUIest::currentTestState == BOBUIest::Skipped);
    }

    BOBUIest::currentTestState = BOBUIest::Suppressed;
    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->addIncident(QAbstractTestLogger::BlacklistedFail, msg, file, line);
}

void BOBUIestLog::addBXPass(const char *msg, const char *file, int line)
{
    BOBUIEST_ASSERT(msg);

    if (BOBUIest::currentTestState == BOBUIest::Unresolved) {
        ++BOBUIest::blacklists;
    } else {
        // After a BXPASS/Continue, we may BXPASS again.
        // Likewise after a fail or skip in a function called by a test.
        Q_ASSERT(BOBUIest::currentTestState == BOBUIest::Suppressed
                 || BOBUIest::currentTestState == BOBUIest::Skipped);
    }

    BOBUIest::currentTestState = BOBUIest::Suppressed;
    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->addIncident(QAbstractTestLogger::BlacklistedXPass, msg, file, line);
}

void BOBUIestLog::addBXFail(const char *msg, const char *file, int line)
{
    BOBUIEST_ASSERT(msg);

    // Will be counted in addBPass() if we get there.

    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->addIncident(QAbstractTestLogger::BlacklistedXFail, msg, file, line);
}

void BOBUIestLog::addSkip(const char *msg, const char *file, int line)
{
    BOBUIEST_ASSERT(msg);

    if (BOBUIest::currentTestState == BOBUIest::Unresolved) {
        ++BOBUIest::skips;
        BOBUIest::currentTestState = BOBUIest::Skipped;
    } else {
        // After an B?XPASS/Continue, we might subsequently skip.
        // Likewise after a skip in a function called by a test.
        Q_ASSERT(BOBUIest::currentTestState == BOBUIest::Suppressed
                 || BOBUIest::currentTestState == BOBUIest::Failed
                 || BOBUIest::currentTestState == BOBUIest::Skipped);
    }
    // It is up to particular loggers to decide whether to report such
    // subsequent skips; they may carry useful information.

    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->addIncident(QAbstractTestLogger::Skip, msg, file, line);
}

void BOBUIestLog::addBenchmarkResults(const QList<QBenchmarkResult> &results)
{
    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->addBenchmarkResults(results);
}

void BOBUIestLog::startLogging()
{
    {
        QMutexLocker locker(&elapsedTimersMutex);
        elapsedTotalTime.start();
        elapsedFunctionTime.start();
    }
    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->startLogging();
    BOBUIest::oldMessageHandler = qInstallMessageHandler(BOBUIest::messageHandler);
}

void BOBUIestLog::stopLogging()
{
    qInstallMessageHandler(BOBUIest::oldMessageHandler);
    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->stopLogging();

    BOBUIest::loggers->clear();
    saveCoverageTool(BOBUIestResult::currentAppName(), failCount() != 0, BOBUIestLog::installedTestCoverage());
}

void BOBUIestLog::addLogger(LogMode mode, const char *filename)
{
    if (filename && strcmp(filename, "-") == 0)
        filename = nullptr;

    QAbstractTestLogger *logger = nullptr;
    switch (mode) {
    case BOBUIestLog::Plain:
        logger = new QPlainTestLogger(filename);
        break;
    case BOBUIestLog::CSV:
        logger = new QCsvBenchmarkLogger(filename);
        break;
    case BOBUIestLog::XML:
        logger = new QXmlTestLogger(QXmlTestLogger::Complete, filename);
        break;
    case BOBUIestLog::LightXML:
        logger = new QXmlTestLogger(QXmlTestLogger::Light, filename);
        break;
    case BOBUIestLog::JUnitXML:
        logger = new QJUnitTestLogger(filename);
        break;
    case BOBUIestLog::TeamCity:
        logger = new BOBUIeamCityLogger(filename);
        break;
    case BOBUIestLog::TAP:
        logger = new BOBUIapTestLogger(filename);
        break;
#if defined(BOBUI_USE_APPLE_UNIFIED_LOGGING)
    case BOBUIestLog::Apple:
        logger = new QAppleTestLogger;
        break;
#endif
#if defined(HAVE_XCTEST)
    case BOBUIestLog::XCTest:
        logger = new QXcodeTestLogger;
        break;
#endif
    }

    BOBUIEST_ASSERT(logger);
    addLogger(std::unique_ptr<QAbstractTestLogger>{ logger });
}

/*!
    \internal

    Adds a new logger to the set of loggers that will be used
    to report incidents and messages during testing.
*/
void BOBUIestLog::addLogger(std::unique_ptr<QAbstractTestLogger> logger)
{
    BOBUIEST_ASSERT(logger);
    BOBUIest::loggers()->addLogger(std::move(logger));
}

bool BOBUIestLog::hasLoggers()
{
    return !BOBUIest::loggers()->allLoggers().isEmpty();
}

/*!
    \internal

    Returns true if all loggers support repeated test runs
*/
bool BOBUIestLog::isRepeatSupported()
{
    for (auto &logger : BOBUIest::loggers->allLoggers())
        if (!logger->isRepeatSupported())
            return false;

    return true;
}

bool BOBUIestLog::loggerUsingStdout()
{
    auto loggersCapture = BOBUIest::loggers->allLoggers();
    return q20::ranges::any_of(loggersCapture.begin(), loggersCapture.end(), [](auto &logger) {
        return logger->isLoggingToStdout();
    });
}

void BOBUIestLog::warn(const char *msg, const char *file, int line)
{
    BOBUIEST_ASSERT(msg);

    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->addMessage(QAbstractTestLogger::Warn, QString::fromUtf8(msg), file, line);
}

void BOBUIestLog::info(const char *msg, const char *file, int line)
{
    BOBUIEST_ASSERT(msg);

    for (auto &logger : BOBUIest::loggers->allLoggers())
        logger->addMessage(QAbstractTestLogger::Info, QString::fromUtf8(msg), file, line);
}

void BOBUIestLog::setVerboseLevel(int level)
{
    BOBUIest::verbosity = level;
}

int BOBUIestLog::verboseLevel()
{
    return BOBUIest::verbosity;
}

void BOBUIestLog::ignoreMessage(BobUIMsgType type, const char *msg)
{
    BOBUIEST_ASSERT(msg);

    const QMutexLocker mutexLocker(&BOBUIest::mutex);
    BOBUIest::IgnoreResultList::append(BOBUIest::ignoreResultList, type, QString::fromUtf8(msg));
}

#if BOBUI_CONFIG(regularexpression)
void BOBUIestLog::ignoreMessage(BobUIMsgType type, const QRegularExpression &expression)
{
    BOBUIEST_ASSERT(expression.isValid());

    const QMutexLocker mutexLocker(&BOBUIest::mutex);
    BOBUIest::IgnoreResultList::append(BOBUIest::ignoreResultList, type, QVariant(expression));
}
#endif // BOBUI_CONFIG(regularexpression)

void BOBUIestLog::failOnWarning()
{
    BOBUIest::failOnWarningList.push_back({});
}

void BOBUIestLog::failOnWarning(const char *msg)
{
    BOBUIest::failOnWarningList.push_back(QString::fromUtf8(msg));
}

#if BOBUI_CONFIG(regularexpression)
void BOBUIestLog::failOnWarning(const QRegularExpression &expression)
{
    BOBUIEST_ASSERT(expression.isValid());

    BOBUIest::failOnWarningList.push_back(QVariant::fromValue(expression));
}
#endif // BOBUI_CONFIG(regularexpression)

void BOBUIestLog::setMaxWarnings(int m)
{
    BOBUIest::maxWarnings = m <= 0 ? INT_MAX : m + 2;
}

bool BOBUIestLog::printAvailableTags = false;

void BOBUIestLog::setPrintAvailableTagsMode()
{
    printAvailableTags = true;
}

int BOBUIestLog::passCount()
{
    return BOBUIest::passes;
}

int BOBUIestLog::failCount()
{
    return BOBUIest::fails;
}

int BOBUIestLog::skipCount()
{
    return BOBUIest::skips;
}

int BOBUIestLog::blacklistCount()
{
    return BOBUIest::blacklists;
}

int BOBUIestLog::totalCount()
{
    return passCount() + failCount() + skipCount() + blacklistCount();
}

void BOBUIestLog::resetCounters()
{
    BOBUIest::passes = 0;
    BOBUIest::fails = 0;
    BOBUIest::skips = 0;
}

void BOBUIestLog::setInstalledTestCoverage(bool installed)
{
    BOBUIest::installedTestCoverage = installed;
}

bool BOBUIestLog::installedTestCoverage()
{
    return BOBUIest::installedTestCoverage;
}

qint64 BOBUIestLog::nsecsTotalTime()
{
    QMutexLocker locker(&elapsedTimersMutex);
    return elapsedTotalTime.nsecsElapsed();
}

qint64 BOBUIestLog::nsecsFunctionTime()
{
    QMutexLocker locker(&elapsedTimersMutex);
    return elapsedFunctionTime.nsecsElapsed();
}

BOBUI_END_NAMESPACE

#include "moc_bobuiestlog_p.cpp"
