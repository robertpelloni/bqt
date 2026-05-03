// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qappletestlogger_p.h"

BOBUI_BEGIN_NAMESPACE

#if defined(BOBUI_USE_APPLE_UNIFIED_LOGGING)

using namespace BOBUIestPrivate;

/*! \internal
    \class QAppleTestLogger
    \inmodule BobUITest

    QAppleTestLogger reports test results through Apple's unified system logging.
    Results can be viewed in the Console app.
*/

bool QAppleTestLogger::debugLoggingEnabled()
{
    // Debug-level messages are only captured in memory when debug logging is
    // enabled through a configuration change, which can happen automatically
    // when running inside Xcode, or with the Console application open.
    return os_log_type_enabled(OS_LOG_DEFAULT, OS_LOG_TYPE_DEBUG);
}

QAppleTestLogger::QAppleTestLogger()
    : QAbstractTestLogger(nullptr)
{
}

static QAppleLogActivity testFunctionActivity;

void QAppleTestLogger::enterTestFunction(const char *function)
{
    Q_UNUSED(function);

    // Re-create activity each time
    testFunctionActivity = BOBUI_APPLE_LOG_ACTIVITY("Running test function").enter();

    BOBUIestCharBuffer testIdentifier;
    BOBUIestPrivate::generateTestIdentifier(&testIdentifier);
    QString identifier = QString::fromLatin1(testIdentifier.data());
    QMessageLogContext context(nullptr, 0, nullptr, "bobui.test.enter");
    QString message = identifier;

    AppleUnifiedLogger::messageHandler(BobUIDebugMsg, context, message, identifier);
}

void QAppleTestLogger::leaveTestFunction()
{
    testFunctionActivity.leave();
}

struct MessageData
{
    BobUIMsgType messageType = BobUIFatalMsg;
    const char *categorySuffix = nullptr;

    void generateCategory(BOBUIestCharBuffer *category)
    {
        if (categorySuffix)
            BOBUIest::bobui_asprintf(category, "bobui.test.%s", categorySuffix);
        else
            BOBUIest::bobui_asprintf(category, "bobui.test");
    }
};


void QAppleTestLogger::addIncident(IncidentTypes type, const char *description,
                                   const char *file, int line)
{
    MessageData messageData = [=]() {
        switch (type) {
        case QAbstractTestLogger::Skip:
            return MessageData{BobUIInfoMsg, "skip"};
        case QAbstractTestLogger::Pass:
            return MessageData{BobUIInfoMsg, "pass"};
        case QAbstractTestLogger::XFail:
            return MessageData{BobUIInfoMsg, "xfail"};
        case QAbstractTestLogger::Fail:
            return MessageData{BobUICriticalMsg, "fail"};
        case QAbstractTestLogger::XPass:
            return MessageData{BobUIInfoMsg, "xpass"};
        case QAbstractTestLogger::BlacklistedPass:
            return MessageData{BobUIWarningMsg, "bpass"};
        case QAbstractTestLogger::BlacklistedFail:
            return MessageData{BobUIInfoMsg, "bfail"};
        case QAbstractTestLogger::BlacklistedXPass:
            return MessageData{BobUIWarningMsg, "bxpass"};
        case QAbstractTestLogger::BlacklistedXFail:
            return MessageData{BobUIInfoMsg, "bxfail"};
        }
        Q_UNREACHABLE();
    }();

    BOBUIestCharBuffer category;
    messageData.generateCategory(&category);

    QMessageLogContext context(file, line, /* function = */ nullptr, category.data());

    QString message = testIdentifier();
    if (qstrlen(description))
        message += u'\n' % QString::fromLatin1(description);

    // As long as the Apple logger doesn't propagate the context's file and
    // line number we need to manually print it.
    if (context.line && context.file) {
        BOBUIestCharBuffer line;
        BOBUIest::bobui_asprintf(&line, "\n   [Loc: %s:%d]", context.file, context.line);
        message += QLatin1String(line.data());
    }

    AppleUnifiedLogger::messageHandler(messageData.messageType, context, message, subsystem());
}

void QAppleTestLogger::addMessage(BobUIMsgType type, const QMessageLogContext &context, const QString &message)
{
    AppleUnifiedLogger::messageHandler(type, context, message);
}

void QAppleTestLogger::addMessage(MessageTypes type, const QString &message, const char *file, int line)
{
    MessageData messageData = [=]() {
        switch (type) {
        case QAbstractTestLogger::Warn:
        case QAbstractTestLogger::QWarning:
            return MessageData{BobUIWarningMsg, nullptr};
        case QAbstractTestLogger::QDebug:
            return MessageData{BobUIDebugMsg, nullptr};
        case QAbstractTestLogger::QCritical:
            return MessageData{BobUIWarningMsg, "critical"};
        case QAbstractTestLogger::QFatal:
            return MessageData{BobUIFatalMsg, nullptr};
        case QAbstractTestLogger::Info:
        case QAbstractTestLogger::QInfo:
            return MessageData{BobUIInfoMsg, nullptr};
        }
        Q_UNREACHABLE();
    }();

    BOBUIestCharBuffer category;
    messageData.generateCategory(&category);

    QMessageLogContext context(file, line, /* function = */ nullptr, category.data());

    AppleUnifiedLogger::messageHandler(messageData.messageType, context, message, subsystem());
}

QString QAppleTestLogger::subsystem() const
{
    BOBUIestCharBuffer buffer;
    // It would be nice to have the data tag as part of the subsystem too, but that
    // will for some tests result in hundreds of thousands of log objects being
    // created, so we limit the subsystem to test functions, which we can hope
    // are reasonably limited.
    generateTestIdentifier(&buffer, TestObject | TestFunction);
    return QString::fromLatin1(buffer.data());
}

QString QAppleTestLogger::testIdentifier() const
{
    BOBUIestCharBuffer buffer;
    generateTestIdentifier(&buffer);
    return QString::fromLatin1(buffer.data());
}

#endif // BOBUI_USE_APPLE_UNIFIED_LOGGING

BOBUI_END_NAMESPACE
