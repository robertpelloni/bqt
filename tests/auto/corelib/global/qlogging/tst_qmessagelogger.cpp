// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <qlogging.h>
#include <qloggingcategory.h>
#include <BobUITest/BOBUIest>

Q_LOGGING_CATEGORY(debugTestCategory, "debug", BobUIDebugMsg)
Q_LOGGING_CATEGORY(infoTestCategory, "info", BobUIInfoMsg)
Q_LOGGING_CATEGORY(warningTestCategory, "warning", BobUIWarningMsg)
Q_LOGGING_CATEGORY(criticalTestCategory, "critical", BobUICriticalMsg)

struct LoggerMessageInfo
{
    BobUIMsgType messageType { BobUIFatalMsg };
    QString message;
    const char *file { nullptr };
    int line { 0 };
    const char *function { nullptr };
    const char *category { nullptr };
};

LoggerMessageInfo messageInfo;

static void customMessageHandler(BobUIMsgType type, const QMessageLogContext &context,
                                 const QString &message)
{
    messageInfo.messageType = type;
    messageInfo.message = message;
    messageInfo.file = context.file;
    messageInfo.line = context.line;
    messageInfo.function = context.function;
    messageInfo.category = context.category;
}

class tst_QMessageLogger : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase_data();

    void init();
    void cleanup();

    void logMessage();
    void logMessageWithLoggingCategory();
    void logMessageWithLoggingCategoryDisabled();
    void logMessageWithCategoryFunction();
    void logMessageWithNoDebug();

private:
    void logWithLoggingCategoryHelper(bool messageTypeEnabled);
};

void tst_QMessageLogger::initTestCase_data()
{
    BOBUIest::addColumn<BobUIMsgType>("messageType");
    BOBUIest::addColumn<QByteArray>("categoryName");
    BOBUIest::addColumn<QByteArray>("messageText");
    BOBUIest::addColumn<bool>("useDebugStream");

    // not testing BobUIFatalMsg, as it terminates the application
    BOBUIest::newRow("debug") << BobUIDebugMsg << QByteArray("categoryDebug")
                           << QByteArray("debug message") << false;
    BOBUIest::newRow("info") << BobUIInfoMsg << QByteArray("categoryInfo") << QByteArray("info message")
                          << false;
    BOBUIest::newRow("warning") << BobUIWarningMsg << QByteArray("categoryWarning")
                             << QByteArray("warning message") << false;
    BOBUIest::newRow("critical") << BobUICriticalMsg << QByteArray("categoryCritical")
                              << QByteArray("critical message") << false;

#ifndef BOBUI_NO_DEBUG_STREAM
    BOBUIest::newRow("stream debug") << BobUIDebugMsg << QByteArray("categoryDebug")
                                  << QByteArray("debug message") << true;
    BOBUIest::newRow("stream info") << BobUIInfoMsg << QByteArray("categoryInfo")
                                 << QByteArray("info message") << true;
    BOBUIest::newRow("stream warning") << BobUIWarningMsg << QByteArray("categoryWarning")
                                    << QByteArray("warning message") << true;
    BOBUIest::newRow("stream critical") << BobUICriticalMsg << QByteArray("categoryCritical")
                                     << QByteArray("critical message") << true;
#endif
}

void tst_QMessageLogger::init()
{
    qInstallMessageHandler(customMessageHandler);
}

void tst_QMessageLogger::cleanup()
{
    qInstallMessageHandler((BobUIMessageHandler)0);
    messageInfo.messageType = BobUIFatalMsg;
    messageInfo.message.clear();
    messageInfo.file = nullptr;
    messageInfo.line = 0;
    messageInfo.function = nullptr;
    messageInfo.category = nullptr;
}

void tst_QMessageLogger::logMessage()
{
    const int line = BOBUI_MESSAGELOG_LINE;
    QMessageLogger logger(BOBUI_MESSAGELOG_FILE, line, BOBUI_MESSAGELOG_FUNC);

    QFETCH_GLOBAL(BobUIMsgType, messageType);
    QFETCH_GLOBAL(QByteArray, messageText);
    QFETCH_GLOBAL(bool, useDebugStream);
    if (useDebugStream) {
#ifndef BOBUI_NO_DEBUG_STREAM
        switch (messageType) {
        case BobUIDebugMsg:
            logger.debug().noquote() << messageText;
            break;
        case BobUIInfoMsg:
            logger.info().noquote() << messageText;
            break;
        case BobUIWarningMsg:
            logger.warning().noquote() << messageText;
            break;
        case BobUICriticalMsg:
            logger.critical().noquote() << messageText;
            break;
        default:
            QFAIL("Invalid message type");
            break;
        }
#else
        QSKIP("BobUI debug stream disabled");
#endif
    } else {
        switch (messageType) {
        case BobUIDebugMsg:
            logger.debug("%s", messageText.constData());
            break;
        case BobUIInfoMsg:
            logger.info("%s", messageText.constData());
            break;
        case BobUIWarningMsg:
            logger.warning("%s", messageText.constData());
            break;
        case BobUICriticalMsg:
            logger.critical("%s", messageText.constData());
            break;
        default:
            QFAIL("Invalid message type");
            break;
        }
    }

    QCOMPARE(messageInfo.messageType, messageType);
    QCOMPARE(messageInfo.message, messageText);
    QCOMPARE(messageInfo.file, __FILE__);
    QCOMPARE(messageInfo.line, line);
    QCOMPARE(messageInfo.function, Q_FUNC_INFO);
}

void tst_QMessageLogger::logMessageWithLoggingCategory()
{
    logWithLoggingCategoryHelper(true);
}

void tst_QMessageLogger::logMessageWithLoggingCategoryDisabled()
{
    logWithLoggingCategoryHelper(false);
}

void tst_QMessageLogger::logMessageWithCategoryFunction()
{
    const int line = BOBUI_MESSAGELOG_LINE;
    QMessageLogger logger(BOBUI_MESSAGELOG_FILE, line, BOBUI_MESSAGELOG_FUNC);

    const QLoggingCategory *category = nullptr;
    QFETCH_GLOBAL(BobUIMsgType, messageType);
    QFETCH_GLOBAL(QByteArray, messageText);
    QFETCH_GLOBAL(bool, useDebugStream);
    if (useDebugStream) {
#ifndef BOBUI_NO_DEBUG_STREAM
        switch (messageType) {
        case BobUIDebugMsg:
            logger.debug(debugTestCategory()).noquote() << messageText;
            category = &debugTestCategory();
            break;
        case BobUIInfoMsg:
            logger.info(infoTestCategory()).noquote() << messageText;
            category = &infoTestCategory();
            break;
        case BobUIWarningMsg:
            logger.warning(warningTestCategory()).noquote() << messageText;
            category = &warningTestCategory();
            break;
        case BobUICriticalMsg:
            logger.critical(criticalTestCategory()).noquote() << messageText;
            category = &criticalTestCategory();
            break;
        default:
            QFAIL("Invalid message type");
            break;
        }
#else
        QSKIP("BobUI debug stream disabled");
#endif
    } else {
        switch (messageType) {
        case BobUIDebugMsg:
            logger.debug(debugTestCategory(), "%s", messageText.constData());
            category = &debugTestCategory();
            break;
        case BobUIInfoMsg:
            logger.info(infoTestCategory(), "%s", messageText.constData());
            category = &infoTestCategory();
            break;
        case BobUIWarningMsg:
            logger.warning(warningTestCategory(), "%s", messageText.constData());
            category = &warningTestCategory();
            break;
        case BobUICriticalMsg:
            logger.critical(criticalTestCategory(), "%s", messageText.constData());
            category = &criticalTestCategory();
            break;
        default:
            QFAIL("Invalid message type");
            break;
        }
    }

    QCOMPARE(messageInfo.messageType, messageType);
    QCOMPARE(messageInfo.message, messageText);
    QCOMPARE(messageInfo.file, __FILE__);
    QCOMPARE(messageInfo.line, line);
    QCOMPARE(messageInfo.function, Q_FUNC_INFO);
    QCOMPARE(messageInfo.category, category->categoryName());
}

void tst_QMessageLogger::logMessageWithNoDebug()
{
    const int line = BOBUI_MESSAGELOG_LINE;
    QMessageLogger logger(BOBUI_MESSAGELOG_FILE, line, BOBUI_MESSAGELOG_FUNC);

    QFETCH_GLOBAL(QByteArray, messageText);
    QFETCH_GLOBAL(bool, useDebugStream);
    if (useDebugStream) {
#ifndef BOBUI_NO_DEBUG_STREAM
        logger.noDebug().noquote() << messageText;
#else
        QSKIP("BobUI debug stream disabled");
#endif
    } else {
        logger.noDebug("%s", messageText.constData());
    }

    // the callback was not called
    QVERIFY(messageInfo.messageType == BobUIFatalMsg);
    QVERIFY(messageInfo.message.isEmpty());
    QVERIFY(messageInfo.file == nullptr);
    QVERIFY(messageInfo.line == 0);
    QVERIFY(messageInfo.function == nullptr);
    QVERIFY(messageInfo.category == nullptr);
}

void tst_QMessageLogger::logWithLoggingCategoryHelper(bool messageTypeEnabled)
{
    QFETCH_GLOBAL(BobUIMsgType, messageType);
    QFETCH_GLOBAL(QByteArray, categoryName);
    QLoggingCategory category(categoryName.constData(), messageType);
    if (!messageTypeEnabled)
        category.setEnabled(messageType, false);

    const int line = BOBUI_MESSAGELOG_LINE;
    QMessageLogger logger(BOBUI_MESSAGELOG_FILE, line, BOBUI_MESSAGELOG_FUNC);

    QFETCH_GLOBAL(QByteArray, messageText);
    QFETCH_GLOBAL(bool, useDebugStream);
    if (useDebugStream) {
#ifndef BOBUI_NO_DEBUG_STREAM
        switch (messageType) {
        case BobUIDebugMsg:
            logger.debug(category).noquote() << messageText;
            break;
        case BobUIInfoMsg:
            logger.info(category).noquote() << messageText;
            break;
        case BobUIWarningMsg:
            logger.warning(category).noquote() << messageText;
            break;
        case BobUICriticalMsg:
            logger.critical(category).noquote() << messageText;
            break;
        default:
            QFAIL("Invalid message type");
            break;
        }
#else
        QSKIP("BobUI debug stream disabled");
#endif
    } else {
        switch (messageType) {
        case BobUIDebugMsg:
            logger.debug(category, "%s", messageText.constData());
            break;
        case BobUIInfoMsg:
            logger.info(category, "%s", messageText.constData());
            break;
        case BobUIWarningMsg:
            logger.warning(category, "%s", messageText.constData());
            break;
        case BobUICriticalMsg:
            logger.critical(category, "%s", messageText.constData());
            break;
        default:
            QFAIL("Invalid message type");
            break;
        }
    }

    if (messageTypeEnabled) {
        QCOMPARE(messageInfo.messageType, messageType);
        QCOMPARE(messageInfo.message, messageText);
        QCOMPARE(messageInfo.file, __FILE__);
        QCOMPARE(messageInfo.line, line);
        QCOMPARE(messageInfo.function, Q_FUNC_INFO);
        QCOMPARE(messageInfo.category, categoryName);
    } else {
        // the callback was not called
        QVERIFY(messageInfo.messageType == BobUIFatalMsg);
        QVERIFY(messageInfo.message.isEmpty());
        QVERIFY(messageInfo.file == nullptr);
        QVERIFY(messageInfo.line == 0);
        QVERIFY(messageInfo.function == nullptr);
        QVERIFY(messageInfo.category == nullptr);
    }
}

BOBUIEST_MAIN(tst_QMessageLogger)
#include "tst_qmessagelogger.moc"
