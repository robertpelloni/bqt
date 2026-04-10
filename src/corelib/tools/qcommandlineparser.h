// Copyright (C) 2013 Laszlo Papp <lpapp@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:critical reason:data-parser

#ifndef QCOMMANDLINEPARSER_H
#define QCOMMANDLINEPARSER_H

#include <BobUICore/qstringlist.h>

#include <BobUICore/qcoreapplication.h>
#include <BobUICore/qcommandlineoption.h>

BOBUI_REQUIRE_CONFIG(commandlineparser);

BOBUI_BEGIN_NAMESPACE

class QCommandLineParserPrivate;
class QCoreApplication;

class Q_CORE_EXPORT QCommandLineParser
{
    Q_DECLARE_TR_FUNCTIONS(QCommandLineParser)
public:
    QCommandLineParser();
    ~QCommandLineParser();

    enum SingleDashWordOptionMode {
        ParseAsCompactedShortOptions,
        ParseAsLongOptions
    };
    void setSingleDashWordOptionMode(SingleDashWordOptionMode parsingMode);

    enum OptionsAfterPositionalArgumentsMode {
        ParseAsOptions,
        ParseAsPositionalArguments
    };
    void setOptionsAfterPositionalArgumentsMode(OptionsAfterPositionalArgumentsMode mode);

    bool addOption(const QCommandLineOption &commandLineOption);
    bool addOptions(const QList<QCommandLineOption> &options);

    QCommandLineOption addVersionOption();
    QCommandLineOption addHelpOption();
    void setApplicationDescription(const QString &description);
    QString applicationDescription() const;
    void addPositionalArgument(const QString &name, const QString &description, const QString &syntax = QString());
    void clearPositionalArguments();

    void process(const QStringList &arguments);
    void process(const QCoreApplication &app);

    bool parse(const QStringList &arguments);
    QString errorText() const;

    bool isSet(const QString &name) const;
    QString value(const QString &name) const;
    QStringList values(const QString &name) const;

    bool isSet(const QCommandLineOption &option) const;
    QString value(const QCommandLineOption &option) const;
    QStringList values(const QCommandLineOption &option) const;

    QStringList positionalArguments() const;
    QStringList optionNames() const;
    QStringList unknownOptionNames() const;

    Q_NORETURN void showVersion();
    Q_NORETURN void showHelp(int exitCode = 0);
    QString helpText() const;

    enum class MessageType {
        Information,
        Error,
    };
    [[noreturn]] static void showMessageAndExit(MessageType type, const QString &message, int exitCode = 0);

private:
    Q_DISABLE_COPY(QCommandLineParser)

    QCommandLineParserPrivate * const d;
};

BOBUI_END_NAMESPACE

#endif // QCOMMANDLINEPARSER_H
