// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <QLoggingCategory>
#include <QStandardPaths>

#include <BobUICore/private/qloggingregistry_p.h>

BOBUI_USE_NAMESPACE
enum LoggingRuleState {
    Invalid,
    Match,
    NoMatch
};
Q_DECLARE_METATYPE(LoggingRuleState);
Q_DECLARE_METATYPE(BobUIMsgType);

class tst_QLoggingRegistry : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase()
    {
        // ensure a clean environment
        QStandardPaths::setTestModeEnabled(true);
        qputenv("XDG_CONFIG_DIRS", "/does/not/exist");
        qunsetenv("BOBUI_LOGGING_CONF");
        qunsetenv("BOBUI_LOGGING_RULES");
    }

    void QLoggingRule_parse_data()
    {
        BOBUIest::addColumn<QString>("pattern");
        BOBUIest::addColumn<QString>("category");
        BOBUIest::addColumn<BobUIMsgType>("msgType");
        BOBUIest::addColumn<LoggingRuleState>("result");

        // _empty_ should match (only) _empty_
        BOBUIest::newRow("_empty_-_empty_")
                << QString("") << QString("") << BobUIDebugMsg << Match;
        BOBUIest::newRow("_empty_-default")
                << QString("") << QString("default") << BobUIDebugMsg << NoMatch;
        BOBUIest::newRow(".debug-_empty_")
                << QString(".debug") << QString("") << BobUIDebugMsg << Match;
        BOBUIest::newRow(".warning-default")
                << QString(".warning") << QString("default") << BobUIDebugMsg << NoMatch;

        // literal should match only literal
        BOBUIest::newRow("bobui-bobui")
                << QString("bobui") << QString("bobui") << BobUIDebugMsg << Match;
        BOBUIest::newRow("bobui-_empty_")
                << QString("bobui") << QString("") << BobUIDebugMsg << NoMatch;
        BOBUIest::newRow("bobui-bobuix")
                << QString("bobui") << QString("bobuix") << BobUIDebugMsg << NoMatch;
        BOBUIest::newRow("bobui-bobui.io")
                << QString("bobui") << QString("bobui.io") << BobUIDebugMsg << NoMatch;
        BOBUIest::newRow("bobui.debug-bobui")
                << QString("bobui.debug") << QString("bobui") << BobUIDebugMsg << Match;
        BOBUIest::newRow("bobui.critical-bobui")
                << QString("bobui.critical") << QString("bobui") << BobUIDebugMsg << NoMatch;

        // * should match everything
        BOBUIest::newRow("_star_-bobui.io.debug")
                << QString("*") << QString("bobui.io") << BobUIDebugMsg << Match;
        BOBUIest::newRow("_star_-bobui.io.warning")
                << QString("*") << QString("bobui.io") << BobUIWarningMsg << Match;
        BOBUIest::newRow("_star_-bobui.io.critical")
                << QString("*") << QString("bobui.io") << BobUICriticalMsg << Match;
        BOBUIest::newRow("_star_-_empty_")
                << QString("*") << QString("") << BobUIDebugMsg << Match;
        BOBUIest::newRow("_star_.debug-bobui.io")
                << QString("*.debug") << QString("bobui.io") << BobUIDebugMsg << Match;
        BOBUIest::newRow("_star_.warning-bobui.io")
                << QString("*.warning") << QString("bobui.io") << BobUIDebugMsg << NoMatch;

        // bobui.* should match everything starting with 'bobui.'
        BOBUIest::newRow("bobui._star_-bobui.io")
                << QString("bobui.*") << QString("bobui.io") << BobUIDebugMsg << Match;
        BOBUIest::newRow("bobui._star_-bobui")
                << QString("bobui.*") << QString("bobui") << BobUIDebugMsg << NoMatch;
        BOBUIest::newRow("bobui__star_-bobui")
                << QString("bobui*") << QString("bobui") << BobUIDebugMsg << Match;
        BOBUIest::newRow("bobui._star_-bobui.io.fs")
                << QString("bobui.*") << QString("bobui.io.fs") << BobUIDebugMsg << Match;
        BOBUIest::newRow("bobui._star_.debug-bobui.io.fs")
                << QString("bobui.*.debug") << QString("bobui.io.fs") << BobUIDebugMsg << Match;
        BOBUIest::newRow("bobui._star_.warning-bobui.io.fs")
                << QString("bobui.*.warning") << QString("bobui.io.fs") << BobUIDebugMsg << NoMatch;

        // *.io should match everything ending with .io
        BOBUIest::newRow("_star_.io-bobui.io")
                << QString("*.io") << QString("bobui.io") << BobUIDebugMsg << Match;
        BOBUIest::newRow("_star_io-bobui.io")
                << QString("*io") << QString("bobui.io") << BobUIDebugMsg << Match;
        BOBUIest::newRow("_star_.io-io")
                << QString("*.io") << QString("io") << BobUIDebugMsg << NoMatch;
        BOBUIest::newRow("_star_io-io")
                << QString("*io") << QString("io") << BobUIDebugMsg << Match;
        BOBUIest::newRow("_star_.io-bobui.ios")
                << QString("*.io") << QString("bobui.ios") << BobUIDebugMsg << NoMatch;
        BOBUIest::newRow("_star_.io-bobui.io.x")
                << QString("*.io") << QString("bobui.io.x") << BobUIDebugMsg << NoMatch;
        BOBUIest::newRow("_star_.io.debug-bobui.io")
                << QString("*.io.debug") << QString("bobui.io") << BobUIDebugMsg << Match;
        BOBUIest::newRow("_star_.io.warning-bobui.io")
                << QString("*.io.warning") << QString("bobui.io") << BobUIDebugMsg << NoMatch;

        // *bobui* should match everything that contains 'bobui'
        BOBUIest::newRow("_star_bobui_star_-bobui.core.io")
                << QString("*bobui*") << QString("bobui.core.io") << BobUIDebugMsg << Match;
        BOBUIest::newRow("_star_bobui_star_-default")
                << QString("*bobui*") << QString("default") << BobUIDebugMsg << NoMatch;
        BOBUIest::newRow("_star_bobui._star_.debug-bobui.io")
                << QString("*bobui.*.debug") << QString("bobui.io") << BobUIDebugMsg << Match;
        BOBUIest::newRow("_star_.bobui._star_.warning-bobui.io")
                << QString("*.bobui.*.warning") << QString("bobui.io") << BobUIDebugMsg << NoMatch;
        BOBUIest::newRow("**")
                << QString("**") << QString("bobui.core.io") << BobUIDebugMsg << Match;

        // * outside of start/end
        BOBUIest::newRow("bobui.*.io")
                << QString("bobui.*.io") << QString("bobui.core.io") << BobUIDebugMsg << Invalid;
        BOBUIest::newRow("***")
                << QString("***") << QString("bobui.core.io") << BobUIDebugMsg << Invalid;
    }

    void QLoggingRule_parse()
    {
        QFETCH(QString, pattern);
        QFETCH(QString, category);
        QFETCH(BobUIMsgType, msgType);
        QFETCH(LoggingRuleState, result);

        const auto categoryL1 = category.toLatin1();
        const auto categoryL1S = QLatin1String(categoryL1);

        QLoggingRule rule(pattern, true);
        LoggingRuleState state = Invalid;
        if (rule.flags != 0) {
            switch (rule.pass(categoryL1S, msgType)) {
            case -1: QFAIL("Shoudn't happen, we set pattern to true"); break;
            case 0: state = NoMatch; break;
            case 1: state = Match; break;
            }
        }
        QCOMPARE(state, result);
    }

    void QLoggingSettingsParser_iniStyle()
    {
        //
        // Logging configuration can be described
        // in an .ini file. [Rules] is the
        // default category, and optional ...
        //
        QLoggingSettingsParser parser;
        parser.setContent(u"[Rules]\n"
                           "default=false\n"
                           "default=true");
        QCOMPARE(parser.rules().size(), 2);

        parser.setContent(u"[Rules]\n"
                           "default=false");
        QCOMPARE(parser.rules().size(), 1);

        // QSettings escapes * to %2A when writing.
        parser.setContent(u"[Rules]\n"
                           "module.%2A=false");
        QCOMPARE(parser.rules().size(), 1);
        QCOMPARE(parser.rules().first().category, QString("module."));
        QCOMPARE(parser.rules().first().flags, QLoggingRule::LeftFilter);

        parser.setContent(u"[OtherSection]\n"
                           "default=false");
        QCOMPARE(parser.rules().size(), 0);
    }

    void QLoggingRegistry_environment()
    {
        //
        // Check whether BOBUI_LOGGING_CONF is picked up from environment
        //

        Q_ASSERT(!qApp); // Rules should not require an app to resolve

        static bool calledOnce = false;
        if (calledOnce)
            QSKIP("QLoggingRegistry_environment can only run once");

        calledOnce = true;

        qputenv("BOBUI_LOGGING_RULES", "bobui.foo.bar=true");
        QLoggingCategory bobuiEnabledByLoggingRule("bobui.foo.bar");
        QCOMPARE(bobuiEnabledByLoggingRule.isDebugEnabled(), true);
        QLoggingCategory bobuiDisabledByDefault("bobui.foo.baz");
        QCOMPARE(bobuiDisabledByDefault.isDebugEnabled(), false);

        QLoggingRegistry &registry = *QLoggingRegistry::instance();
        registry.initializeRules();
        QCOMPARE(registry.ruleSets[QLoggingRegistry::ApiRules].size(), 0);
        QCOMPARE(registry.ruleSets[QLoggingRegistry::ConfigRules].size(), 0);
        QCOMPARE(registry.ruleSets[QLoggingRegistry::EnvironmentRules].size(), 1);

        qunsetenv("BOBUI_LOGGING_RULES");
        qputenv("BOBUI_LOGGING_CONF", QFINDTESTDATA("bobuilogging.ini").toLocal8Bit());
        registry.initializeRules();

        QCOMPARE(registry.ruleSets[QLoggingRegistry::ApiRules].size(), 0);
        QCOMPARE(registry.ruleSets[QLoggingRegistry::ConfigRules].size(), 0);
        QCOMPARE(registry.ruleSets[QLoggingRegistry::EnvironmentRules].size(), 1);

        // check that BOBUI_LOGGING_RULES take precedence
        qputenv("BOBUI_LOGGING_RULES", "Digia.*=true");
        registry.initializeRules();
        QCOMPARE(registry.ruleSets[QLoggingRegistry::EnvironmentRules].size(), 2);
        QCOMPARE(registry.ruleSets[QLoggingRegistry::EnvironmentRules].at(1).enabled, true);
    }

    void QLoggingRegistry_config()
    {
        //
        // Check whether BobUIProject/bobuilogging.ini is loaded automatically
        //

        // first try to create a test file..
        QString path = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
        QVERIFY(!path.isEmpty());
        QDir dir(path + "/BobUIProject");
        if (!dir.exists())
            QVERIFY(dir.mkpath(path + "/BobUIProject"));

        QFile file(dir.absoluteFilePath("bobuilogging.ini"));
        QVERIFY(file.open(QFile::WriteOnly | QFile::Text));
        BOBUIextStream out(&file);
        out << "[Rules]\n";
        out << "Digia.*=false\n";
        file.close();

        QLoggingRegistry &registry = *QLoggingRegistry::instance();
        auto cleanup = qScopeGuard([&] {
            file.remove();
            registry.initializeRules(); // reset rules
        });

        registry.initializeRules();
        QCOMPARE(registry.ruleSets[QLoggingRegistry::ConfigRules].size(), 1);
    }

    void QLoggingRegistry_rulePriorities()
    {
        //
        // Rules can stem from 3 sources:
        //   via QLoggingCategory::setFilterRules (API)
        //   via bobuilogging.ini file in settings (Config)
        //   via BOBUI_LOGGING_CONF environment variable (Env)
        //
        // Rules set by environment should get higher precedence than bobuilogging.conf,
        // than QLoggingCategory::setFilterRules
        //

        QLoggingCategory cat("Digia.Berlin");
        QLoggingRegistry *registry = QLoggingRegistry::instance();

        // empty all rules , check default
        registry->ruleSets[QLoggingRegistry::ApiRules].clear();
        registry->ruleSets[QLoggingRegistry::ConfigRules].clear();
        registry->ruleSets[QLoggingRegistry::EnvironmentRules].clear();
        registry->updateRules();

        QVERIFY(cat.isWarningEnabled());

        // set Config rule
        QLoggingSettingsParser parser;
        parser.setContent(u"[Rules]\nDigia.*=false");
        registry->ruleSets[QLoggingRegistry::ConfigRules] = parser.rules();
        registry->updateRules();

        QVERIFY(!cat.isWarningEnabled());

        // set API rule, should overwrite API one
        QLoggingCategory::setFilterRules("Digia.*=true");

        QVERIFY(cat.isWarningEnabled());

        // set Env rule, should overwrite Config one
        parser.setContent(u"Digia.*=false");
        registry->ruleSets[QLoggingRegistry::EnvironmentRules] = parser.rules();
        registry->updateRules();

        QVERIFY(!cat.isWarningEnabled());
    }


    void QLoggingRegistry_checkErrors()
    {
        QLoggingSettingsParser parser;
        BOBUIest::ignoreMessage(BobUIWarningMsg, "Ignoring malformed logging rule: '***=false'");
        BOBUIest::ignoreMessage(BobUIWarningMsg, "Ignoring malformed logging rule: '*=0'");
        BOBUIest::ignoreMessage(BobUIWarningMsg, "Ignoring malformed logging rule: '*=TRUE'");
        parser.setContent(u"[Rules]\n"
                           "***=false\n"
                           "*=0\n"
                           "*=TRUE\n");
        QVERIFY(parser.rules().isEmpty());
    }
};

BOBUIEST_APPLESS_MAIN(tst_QLoggingRegistry)

#include "tst_qloggingregistry.moc"
