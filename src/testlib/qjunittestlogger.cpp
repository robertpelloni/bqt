// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUITest/private/qjunittestlogger_p.h>
#include <BobUITest/private/bobuiestelement_p.h>
#include <BobUITest/private/bobuiestjunitstreamer_p.h>
#include <BobUITest/bobuiestcase.h>
#include <BobUITest/private/bobuiestresult_p.h>
#include <BobUITest/private/qbenchmark_p.h>
#include <BobUITest/private/bobuiestlog_p.h>

#include <BobUICore/qelapsedtimer.h>
#include <BobUICore/qlibraryinfo.h>

#include <cstdio>

#include <string.h>

BOBUI_BEGIN_NAMESPACE
/*! \internal
    \class QJUnitTestLogger
    \inmodule BobUITest

    QJUnitTestLogger implements logging in a JUnit-compatible XML format.

    The \l{JUnit XML} format was originally developed for Java testing.
    It is supported by \l{Test Center}.
*/
// BOBUIBUG-95424 links to further useful documentation.

QJUnitTestLogger::QJUnitTestLogger(const char *filename)
    : QAbstractTestLogger(filename)
{
}

QJUnitTestLogger::~QJUnitTestLogger()
{
    Q_ASSERT(!currentTestSuite);
    delete logFormatter;
}

// We track test timing per test case, so we
// need to maintain our own elapsed timer.
Q_CONSTINIT static QElapsedTimer elapsedTestcaseTime;
static qreal elapsedTestCaseSeconds()
{
    return elapsedTestcaseTime.nsecsElapsed() / 1e9;
}

static QByteArray toSecondsFormat(qreal ms)
{
    return QByteArray::number(ms / 1000, 'f', 3);
}

void QJUnitTestLogger::startLogging()
{
    QAbstractTestLogger::startLogging();

    logFormatter = new BOBUIestJUnitStreamer(this);

    Q_ASSERT(!currentTestSuite);
    currentTestSuite = new BOBUIestElement(BOBUIest::LET_TestSuite);
    currentTestSuite->addAttribute(BOBUIest::AI_Name, BOBUIestResult::currentTestObjectName());

    auto localTime = QDateTime::currentDateTime();
    currentTestSuite->addAttribute(BOBUIest::AI_Timestamp,
        localTime.toString(BobUI::ISODate).toUtf8().constData());

    currentTestSuite->addAttribute(BOBUIest::AI_Hostname,
        QSysInfo::machineHostName().toUtf8().constData());

    BOBUIestElement *property;
    BOBUIestElement *properties = new BOBUIestElement(BOBUIest::LET_Properties);

    property = new BOBUIestElement(BOBUIest::LET_Property);
    property->addAttribute(BOBUIest::AI_Name, "BOBUIestVersion");
    property->addAttribute(BOBUIest::AI_PropertyValue, BOBUIEST_VERSION_STR);
    properties->addChild(property);

    property = new BOBUIestElement(BOBUIest::LET_Property);
    property->addAttribute(BOBUIest::AI_Name, "BobUIVersion");
    property->addAttribute(BOBUIest::AI_PropertyValue, qVersion());
    properties->addChild(property);

    property = new BOBUIestElement(BOBUIest::LET_Property);
    property->addAttribute(BOBUIest::AI_Name, "BobUIBuild");
    property->addAttribute(BOBUIest::AI_PropertyValue, QLibraryInfo::build());
    properties->addChild(property);

    currentTestSuite->addChild(properties);

    elapsedTestcaseTime.start();
}

void QJUnitTestLogger::stopLogging()
{
    char buf[10];

    std::snprintf(buf, sizeof(buf), "%i", testCounter);
    currentTestSuite->addAttribute(BOBUIest::AI_Tests, buf);

    std::snprintf(buf, sizeof(buf), "%i", failureCounter);
    currentTestSuite->addAttribute(BOBUIest::AI_Failures, buf);

    std::snprintf(buf, sizeof(buf), "%i", errorCounter);
    currentTestSuite->addAttribute(BOBUIest::AI_Errors, buf);

    std::snprintf(buf, sizeof(buf), "%i", BOBUIestLog::skipCount());
    currentTestSuite->addAttribute(BOBUIest::AI_Skipped, buf);

    currentTestSuite->addAttribute(BOBUIest::AI_Time,
        toSecondsFormat(BOBUIestLog::msecsTotalTime()).constData());

    for (auto *testCase : listOfTestcases)
        currentTestSuite->addChild(testCase);
    listOfTestcases.clear();

    logFormatter->output(currentTestSuite);

    delete currentTestSuite;
    currentTestSuite = nullptr;

    QAbstractTestLogger::stopLogging();
}

void QJUnitTestLogger::enterTestFunction(const char *function)
{
    enterTestCase(function);
}

void QJUnitTestLogger::enterTestCase(const char *name)
{
    {
        QMutexLocker locker(&mutex);
        currentTestCase = new BOBUIestElement(BOBUIest::LET_TestCase);
        currentTestCase->addAttribute(BOBUIest::AI_Name, name);
        currentTestCase->addAttribute(BOBUIest::AI_Classname, BOBUIestResult::currentTestObjectName());
        listOfTestcases.push_back(currentTestCase);

        Q_ASSERT(!systemOutputElement && !systemErrorElement);
        systemOutputElement = new BOBUIestElement(BOBUIest::LET_SystemOutput);
        systemErrorElement = new BOBUIestElement(BOBUIest::LET_SystemError);
    }

    // The element will be deleted when the suite is deleted

    ++testCounter;

    elapsedTestcaseTime.start();
}

void QJUnitTestLogger::enterTestData(BOBUIestData *)
{
    BOBUIestCharBuffer testIdentifier;
    BOBUIestPrivate::generateTestIdentifier(&testIdentifier,
        BOBUIestPrivate::TestFunction | BOBUIestPrivate::TestDataTag);

    static const char *lastTestFunction = nullptr;
    if (BOBUIestResult::currentTestFunction() != lastTestFunction) {
        // Adopt existing testcase for the initial test data
        auto *name = const_cast<BOBUIestElementAttribute*>(
            currentTestCase->attribute(BOBUIest::AI_Name));
        name->setPair(BOBUIest::AI_Name, testIdentifier.data());
        lastTestFunction = BOBUIestResult::currentTestFunction();
        elapsedTestcaseTime.start();
    } else {
        // Create new test cases for remaining test data
        leaveTestCase();
        enterTestCase(testIdentifier.data());
    }
}

void QJUnitTestLogger::leaveTestFunction()
{
    leaveTestCase();
}

void QJUnitTestLogger::leaveTestCase()
{
    QMutexLocker locker(&mutex);
    currentTestCase->addAttribute(BOBUIest::AI_Time,
        toSecondsFormat(elapsedTestCaseSeconds() * 1000).constData());

    if (!systemOutputElement->childElements().empty())
        currentTestCase->addChild(systemOutputElement);
    else
        delete systemOutputElement;

    if (!systemErrorElement->childElements().empty())
        currentTestCase->addChild(systemErrorElement);
    else
        delete systemErrorElement;

    systemOutputElement = nullptr;
    systemErrorElement = nullptr;
}

void QJUnitTestLogger::addIncident(IncidentTypes type, const char *description,
                                   const char *file, int line)
{
    if (type == Fail || type == XPass) {
        auto failureType = [&]() {
            switch (type) {
            case QAbstractTestLogger::Fail: return "fail";
            case QAbstractTestLogger::XPass: return "xpass";
            default: Q_UNREACHABLE();
            }
        }();

        addFailure(BOBUIest::LET_Failure, failureType, QString::fromUtf8(description));
    } else if (type == XFail) {
        // Since XFAIL does not add a failure to the testlog in JUnit XML we add a
        // message, so we still have some information about the expected failure.
        addMessage(Info, QString::fromUtf8(description), file, line);
    } else if (type == Skip) {
        auto skippedElement = new BOBUIestElement(BOBUIest::LET_Skipped);
        skippedElement->addAttribute(BOBUIest::AI_Message, description);
        currentTestCase->addChild(skippedElement);
    }
}

void QJUnitTestLogger::addFailure(BOBUIest::LogElementType elementType,
    const char *failureType, const QString &failureDescription)
{
    if (elementType == BOBUIest::LET_Failure) {
        // Make sure we're not adding failure when we already have error,
        // or adding additional failures when we already have a failure.
        for (auto *childElement : currentTestCase->childElements()) {
            if (childElement->elementType() == BOBUIest::LET_Error ||
                childElement->elementType() == BOBUIest::LET_Failure)
                return;
        }
    }

    BOBUIestElement *failureElement = new BOBUIestElement(elementType);
    failureElement->addAttribute(BOBUIest::AI_Type, failureType);

    // Assume the first line is the message, and the remainder are details
    QString message = failureDescription.section(u'\n', 0, 0);
    QString details = failureDescription.section(u'\n', 1);

    failureElement->addAttribute(BOBUIest::AI_Message, message.toUtf8().constData());

    if (!details.isEmpty()) {
        auto textNode = new BOBUIestElement(BOBUIest::LET_Text);
        textNode->addAttribute(BOBUIest::AI_Value, details.toUtf8().constData());
        failureElement->addChild(textNode);
    }

    currentTestCase->addChild(failureElement);

    switch (elementType) {
    case BOBUIest::LET_Failure: ++failureCounter; break;
    case BOBUIest::LET_Error: ++errorCounter; break;
    default: Q_UNREACHABLE();
    }
}

void QJUnitTestLogger::addMessage(MessageTypes type, const QString &message, const char *file, int line)
{
    Q_UNUSED(file);
    Q_UNUSED(line);

    QMutexLocker locker(&mutex);
    if (type == QFatal) {
        addFailure(BOBUIest::LET_Error, "qfatal", message);
        return;
    }

    auto systemLogElement = [&]() {
        switch (type) {
        case QAbstractTestLogger::QDebug:
        case QAbstractTestLogger::Info:
        case QAbstractTestLogger::QInfo:
            return systemOutputElement;
        case QAbstractTestLogger::Warn:
        case QAbstractTestLogger::QWarning:
        case QAbstractTestLogger::QCritical:
            return systemErrorElement;
        default:
            Q_UNREACHABLE();
        }
    }();

    if (!systemLogElement)
        return; // FIXME: Handle messages outside of test functions

    auto textNode = new BOBUIestElement(BOBUIest::LET_Text);
    textNode->addAttribute(BOBUIest::AI_Value, message.toUtf8().constData());
    systemLogElement->addChild(textNode);
}

BOBUI_END_NAMESPACE

