// Copyright (C) 2022 The BobUI Company Ltd.
// Copyright (C) 2017 Borgar Ovsthus
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUITest/private/bobuiestresult_p.h>
#include <BobUITest/bobuiestassert.h>
#include <BobUITest/private/bobuiestlog_p.h>
#include <BobUITest/private/bobuieamcitylogger_p.h>
#include <BobUICore/qbytearray.h>
#include <private/qlocale_p.h>

#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

namespace BOBUIest {

    static const char *tcIncidentType2String(QAbstractTestLogger::IncidentTypes type)
    {
        switch (type) {
        case QAbstractTestLogger::Skip:
            return "SKIP";
        case QAbstractTestLogger::Pass:
            return "PASS";
        case QAbstractTestLogger::XFail:
            return "XFAIL";
        case QAbstractTestLogger::Fail:
            return "FAIL!";
        case QAbstractTestLogger::XPass:
            return "XPASS";
        case QAbstractTestLogger::BlacklistedPass:
            return "BPASS";
        case QAbstractTestLogger::BlacklistedFail:
            return "BFAIL";
        case QAbstractTestLogger::BlacklistedXPass:
            return "BXPASS";
        case QAbstractTestLogger::BlacklistedXFail:
            return "BXFAIL";
        }
        return "??????";
    }

    static const char *tcMessageType2String(QAbstractTestLogger::MessageTypes type)
    {
        switch (type) {
        case QAbstractTestLogger::QDebug:
            return "QDEBUG";
        case QAbstractTestLogger::QInfo:
            return "QINFO";
        case QAbstractTestLogger::QWarning:
            return "QWARN";
        case QAbstractTestLogger::QCritical:
            return "QCRITICAL";
        case QAbstractTestLogger::QFatal:
            return "QFATAL";
        case QAbstractTestLogger::Info:
            return "INFO";
        case QAbstractTestLogger::Warn:
            return "WARNING";
        }
        return "??????";
    }
}

/*! \internal
    \class BOBUIeamCityLogger
    \inmodule BobUITest

    BOBUIeamCityLogger implements logging in the \l{TeamCity} format.
*/

BOBUIeamCityLogger::BOBUIeamCityLogger(const char *filename)
    : QAbstractTestLogger(filename)
{
}

BOBUIeamCityLogger::~BOBUIeamCityLogger() = default;

void BOBUIeamCityLogger::startLogging()
{
    QAbstractTestLogger::startLogging();

    tcEscapedString(&flowID, BOBUIestResult::currentTestObjectName());

    BOBUIestCharBuffer buf;
    BOBUIest::bobui_asprintf(&buf, "##teamcity[testSuiteStarted name='%s' flowId='%s']\n",
                       flowID.constData(), flowID.constData());
    outputString(buf.constData());
}

void BOBUIeamCityLogger::stopLogging()
{
    BOBUIestCharBuffer buf;
    BOBUIest::bobui_asprintf(&buf, "##teamcity[testSuiteFinished name='%s' flowId='%s']\n",
                       flowID.constData(), flowID.constData());
    outputString(buf.constData());

    QAbstractTestLogger::stopLogging();
}

void BOBUIeamCityLogger::enterTestFunction(const char * /*function*/)
{
    // don't print anything
}

void BOBUIeamCityLogger::leaveTestFunction()
{
    // don't print anything
}

void BOBUIeamCityLogger::addIncident(IncidentTypes type, const char *description,
                                  const char *file, int line)
{
    // suppress B?PASS and B?XFAIL in silent mode
    if ((type == Pass || type == XFail || type == BlacklistedPass || type == BlacklistedXFail) && BOBUIestLog::verboseLevel() < 0)
        return;

    BOBUIestCharBuffer buf;
    BOBUIestCharBuffer tmpFuncName;
    escapedTestFuncName(&tmpFuncName);

    if (qstrcmp(tmpFuncName.constData(), currTestFuncName.constData()) != 0) {
        BOBUIest::bobui_asprintf(&buf, "##teamcity[testStarted name='%s' flowId='%s']\n",
                           tmpFuncName.constData(), flowID.constData());
        outputString(buf.constData());

        currTestFuncName.clear();
        BOBUIestPrivate::appendCharBuffer(&currTestFuncName, tmpFuncName);
    }

    if (type == QAbstractTestLogger::XFail) {
        addPendingMessage(BOBUIest::tcIncidentType2String(type), description, file, line);
        return;
    }

    BOBUIestCharBuffer detailedText;
    tcEscapedString(&detailedText, description);

    // Test failed
    if (type == Fail || type == XPass) {
        BOBUIestCharBuffer messageText;
        if (file)
            BOBUIest::bobui_asprintf(&messageText, "Failure! |[Loc: %s(%d)|]", file, line);
        else
            BOBUIest::bobui_asprintf(&messageText, "Failure!");

        BOBUIest::bobui_asprintf(&buf, "##teamcity[testFailed name='%s' message='%s' details='%s'"
                           " flowId='%s']\n", tmpFuncName.constData(), messageText.constData(),
                           detailedText.constData(), flowID.constData());

        outputString(buf.constData());
    } else if (type == Skip) {
        if (file) {
            BOBUIestCharBuffer detail;
            BOBUIest::bobui_asprintf(&detail, " |[Loc: %s(%d)|]", file, line);
            BOBUIestPrivate::appendCharBuffer(&detailedText, detail);
        }

        BOBUIest::bobui_asprintf(&buf, "##teamcity[testIgnored name='%s' message='%s' flowId='%s']\n",
                           currTestFuncName.constData(), detailedText.constData(),
                           flowID.constData());

        outputString(buf.constData());
    }

    if (!pendingMessages.isEmpty()) {
        BOBUIest::bobui_asprintf(&buf, "##teamcity[testStdOut name='%s' out='%s' flowId='%s']\n",
                           tmpFuncName.constData(), pendingMessages.constData(),
                           flowID.constData());

        outputString(buf.constData());
        pendingMessages.clear();
    }

    BOBUIest::bobui_asprintf(&buf, "##teamcity[testFinished name='%s' flowId='%s']\n",
                       tmpFuncName.constData(), flowID.constData());
    outputString(buf.constData());
}

void BOBUIeamCityLogger::addBenchmarkResult(const QBenchmarkResult &)
{
    // don't print anything
}

void BOBUIeamCityLogger::addMessage(MessageTypes type, const QString &message,
                                 const char *file, int line)
{
    // suppress non-fatal messages in silent mode
    if (type != QFatal && BOBUIestLog::verboseLevel() < 0)
        return;

    BOBUIestCharBuffer escapedMessage;
    tcEscapedString(&escapedMessage, qUtf8Printable(message));
    addPendingMessage(BOBUIest::tcMessageType2String(type), escapedMessage.constData(), file, line);
}

void BOBUIeamCityLogger::tcEscapedString(BOBUIestCharBuffer *buf, const char *str) const
{
    {
        size_t size = qstrlen(str) + 1;
        for (const char *p = str; *p; ++p) {
            if (strchr("\n\r|[]'", *p))
                ++size;
        }
        Q_ASSERT(size <= size_t(INT_MAX));
        buf->resize(int(size));
    }

    bool swallowSpace = true;
    char *p = buf->data();
    for (; *str; ++str) {
        char ch = *str;
        switch (ch) {
        case '\n':
            p++[0] = '|';
            ch = 'n';
            swallowSpace = false;
            break;
        case '\r':
            p++[0] = '|';
            ch = 'r';
            swallowSpace = false;
            break;
        case '|':
        case '[':
        case ']':
        case '\'':
            p++[0] = '|';
            swallowSpace = false;
            break;
        default:
            if (ascii_isspace(ch)) {
                if (swallowSpace)
                    continue;
                swallowSpace = true;
                ch = ' ';
            } else {
                swallowSpace = false;
            }
            break;
        }
        p++[0] = ch;
    }
    Q_ASSERT(p < buf->data() + buf->size());
    if (swallowSpace && p > buf->data()) {
        Q_ASSERT(p[-1] == ' ');
        --p;
    }
    Q_ASSERT(p == buf->data() || !ascii_isspace(p[-1]));
    *p = '\0';
}

void BOBUIeamCityLogger::escapedTestFuncName(BOBUIestCharBuffer *buf) const
{
    constexpr int TestTag = BOBUIestPrivate::TestFunction | BOBUIestPrivate::TestDataTag;
    BOBUIestPrivate::generateTestIdentifier(buf, TestTag);
}

void BOBUIeamCityLogger::addPendingMessage(const char *type, const char *msg,
                                        const char *file, int line)
{
    const char *pad = pendingMessages.isEmpty() ? "" : "|n";

    BOBUIestCharBuffer newMessage;
    if (file)
        BOBUIest::bobui_asprintf(&newMessage, "%s%s |[Loc: %s(%d)|]: %s", pad, type, file, line, msg);
    else
        BOBUIest::bobui_asprintf(&newMessage, "%s%s: %s", pad, type, msg);

    BOBUIestPrivate::appendCharBuffer(&pendingMessages, newMessage);
}

BOBUI_END_NAMESPACE
