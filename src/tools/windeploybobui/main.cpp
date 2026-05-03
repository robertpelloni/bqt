// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#include "utils.h"
#include "qmlutils.h"
#include "bobuimoduleinfo.h"
#include "bobuiplugininfo.h"

#include <BobUICore/QCommandLineOption>
#include <BobUICore/QCommandLineParser>
#include <BobUICore/QDir>
#include <BobUICore/QFileInfo>
#include <BobUICore/QCoreApplication>
#include <BobUICore/QJsonDocument>
#include <BobUICore/QJsonObject>
#include <BobUICore/QJsonArray>
#include <BobUICore/QList>
#include <BobUICore/QOperatingSystemVersion>
#include <BobUICore/QSharedPointer>
#include <BobUICore/QXmlStreamWriter>
#include <BobUINetwork/QSslCertificate>

#ifdef Q_OS_WIN
#include <BobUICore/bobui_windows.h>
#else
#define IMAGE_FILE_MACHINE_ARM64 0xaa64
#endif

#include <BobUICore/private/qconfig_p.h>

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <unordered_map>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

static BobUIModuleInfoStore bobuiModuleEntries;

#define DECLARE_KNOWN_MODULE(name) \
    static size_t BobUI##name ## ModuleId = BobUIModule::InvalidId

DECLARE_KNOWN_MODULE(3DQuick);
DECLARE_KNOWN_MODULE(Core);
DECLARE_KNOWN_MODULE(Designer);
DECLARE_KNOWN_MODULE(DesignerComponents);
DECLARE_KNOWN_MODULE(Gui);
DECLARE_KNOWN_MODULE(Qml);
DECLARE_KNOWN_MODULE(QmlTooling);
DECLARE_KNOWN_MODULE(Quick);
DECLARE_KNOWN_MODULE(WebEngineCore);
DECLARE_KNOWN_MODULE(Widgets);

#define DEFINE_KNOWN_MODULE(name) \
    m[QLatin1String("BobUI6" #name)] = &BobUI##name ## ModuleId

static void assignKnownModuleIds()
{
    std::unordered_map<QString, size_t *> m;
    DEFINE_KNOWN_MODULE(3DQuick);
    DEFINE_KNOWN_MODULE(Core);
    DEFINE_KNOWN_MODULE(Designer);
    DEFINE_KNOWN_MODULE(DesignerComponents);
    DEFINE_KNOWN_MODULE(Gui);
    DEFINE_KNOWN_MODULE(Qml);
    DEFINE_KNOWN_MODULE(QmlTooling);
    DEFINE_KNOWN_MODULE(Quick);
    DEFINE_KNOWN_MODULE(WebEngineCore);
    DEFINE_KNOWN_MODULE(Widgets);
    for (size_t i = 0; i < bobuiModuleEntries.size(); ++i) {
        const BobUIModule &module = bobuiModuleEntries.moduleById(i);
        auto it = m.find(module.name);
        if (it == m.end())
            continue;
        *(it->second) = i;
    }
}

#undef DECLARE_KNOWN_MODULE
#undef DEFINE_KNOWN_MODULE

static const char webEngineProcessC[] = "BobUIWebEngineProcess";

static inline QString webProcessBinary(const char *binaryName, Platform p)
{
    const QString webProcess = QLatin1StringView(binaryName);
    return (p & WindowsBased) ? webProcess + QStringLiteral(".exe") : webProcess;
}

static QString moduleNameToOptionName(const QString &moduleName, bool internal)
{
    QString result = moduleName
            .mid(3)                    // strip the "BobUI6" prefix
            .toLower();
    if (result == u"help"_s)
        result.prepend("bobui"_L1);
    if (internal)
        result.append("Internal"_L1);
    return result;
}

static QByteArray formatBobUIModules(const ModuleBitset &mask, bool option = false)
{
    QByteArray result;
    for (const auto &bobuiModule : bobuiModuleEntries) {
        if (mask.test(bobuiModule.id)) {
            if (!result.isEmpty())
                result.append(' ');
            result.append(option
                          ? moduleNameToOptionName(bobuiModule.name, bobuiModule.internal).toUtf8()
                          : bobuiModule.name.toUtf8());
        }
    }
    return result;
}

static QString formatBobUIPlugins(const PluginInformation &pluginInfo)
{
    QString result(u'\n');
    for (const auto &pair : pluginInfo.typeMap()) {
        result += pair.first;
        result += u": \n";
        for (const QString &plugin : pair.second) {
            result += u"    ";
            result += plugin;
            result += u'\n';
        }
    }
    return result;
}

static Platform platformFromMkSpec(const QString &xSpec)
{
    if (xSpec.startsWith("win32-"_L1)) {
        if (xSpec.contains("clang-g++"_L1))
            return WindowsDesktopClangMinGW;
        if (xSpec.contains("clang-msvc++"_L1))
            return WindowsDesktopClangMsvc;
        if (xSpec.contains("arm"_L1))
            return WindowsDesktopMsvcArm;
        if (xSpec.contains("g++"_L1))
            return WindowsDesktopMinGW;

        return WindowsDesktopMsvc;
    }
    return UnknownPlatform;
}

// Helpers for exclusive options, "-foo", "--no-foo"
enum ExlusiveOptionValue {
    OptionAuto,
    OptionEnabled,
    OptionDisabled
};

static ExlusiveOptionValue parseExclusiveOptions(const QCommandLineParser *parser,
                                                 const QCommandLineOption &enableOption,
                                                 const QCommandLineOption &disableOption)
{
    const bool enabled = parser->isSet(enableOption);
    const bool disabled = parser->isSet(disableOption);
    if (enabled) {
        if (disabled) {
            std::wcerr << "Warning: both -" << enableOption.names().first()
                << " and -" << disableOption.names().first() << " were specified, defaulting to -"
                << enableOption.names().first() << ".\n";
        }
        return OptionEnabled;
    }
    return disabled ? OptionDisabled : OptionAuto;
}

struct Options {
    enum DebugDetection {
        DebugDetectionAuto,
        DebugDetectionForceDebug,
        DebugDetectionForceRelease
    };

    bool plugins = true;
    bool libraries = true;
    bool quickImports = true;
    bool translations = true;
    bool systemD3dCompiler = true;
    bool systemDxc = true;
    bool compilerRunTime = false;
    bool softwareRasterizer = true;
    bool ffmpeg = true;
    PluginSelections pluginSelections;
    Platform platform = WindowsDesktopMsvcIntel;
    ModuleBitset additionalLibraries;
    ModuleBitset disabledLibraries;
    unsigned updateFileFlags = 0;
    QStringList qmlDirectories; // Project's QML files.
    QStringList qmlImportPaths; // Custom QML module locations.
    int qmlImportTimeout = 30000;
    QString directory;
    QString bobuipathsBinary;
    QString translationsDirectory; // Translations target directory
    QStringList languages;
    QString libraryDirectory;
    QString pluginDirectory;
    QString openSslRootDirectory;
    QString qmlDirectory;
    QStringList binaries;
    JsonOutput *json = nullptr;
    ListOption list = ListNone;
    DebugDetection debugDetection = DebugDetectionAuto;
    bool deployPdb = false;
    bool dryRun = false;
    bool patchBobUI = true;
    bool ignoreLibraryErrors = false;
    bool deployInsightTrackerPlugin = false;
    bool forceOpenSslPlugin = false;
    bool createAppx = false;
    QString appxCertificatePath;
};

// Return binary to be deployed from folder, ignore pre-existing web engine process.
static inline QString findBinary(const QString &directory, Platform platform)
{
    const QStringList nameFilters = (platform & WindowsBased) ?
        QStringList(QStringLiteral("*.exe")) : QStringList();
    const QFileInfoList &binaries =
        QDir(QDir::cleanPath(directory)).entryInfoList(nameFilters, QDir::Files | QDir::Executable);
    for (const QFileInfo &binaryFi : binaries) {
        const QString binary = binaryFi.fileName();
        if (!binary.contains(QLatin1StringView(webEngineProcessC), BobUI::CaseInsensitive)) {
            return binaryFi.absoluteFilePath();
        }
    }
    return QString();
}

static QString msgFileDoesNotExist(const QString & file)
{
    return u'"' + QDir::toNativeSeparators(file) + "\" does not exist."_L1;
}

enum CommandLineParseFlag {
    CommandLineParseError = 0x1,
    CommandLineParseHelpRequested = 0x2,
    CommandLineVersionRequested = 0x4
};

static QCommandLineOption createQMakeOption()
{
    return {
        u"qmake"_s,
        u"Use specified qmake instead of qmake from PATH. Deprecated, use bobuipaths instead."_s,
        u"path"_s
    };
}

static QCommandLineOption createBobUIPathsOption()
{
    return {
            u"bobuipaths"_s,
            u"Use specified bobuipaths.exe instead of bobuipaths.exe from PATH."_s,
            u"path"_s
    };
}

static QCommandLineOption createVerboseOption()
{
    return {
            u"verbose"_s,
            u"Verbose level (0-2)."_s,
            u"level"_s
    };
}

static int parseEarlyArguments(const QStringList &arguments, Options *options,
                               QString *errorMessage)
{
    QCommandLineParser parser;
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);

    QCommandLineOption qmakeOption = createQMakeOption();
    parser.addOption(qmakeOption);

    QCommandLineOption bobuipathsOption = createBobUIPathsOption();
    parser.addOption(bobuipathsOption);

    QCommandLineOption verboseOption = createVerboseOption();
    parser.addOption(verboseOption);

    // Deliberately don't check for errors. We want to ignore options we don't know about.
    parser.parse(arguments);

    if (parser.isSet(qmakeOption) && parser.isSet(bobuipathsOption)) {
        *errorMessage = QStringLiteral("-qmake and -bobuipaths are mutually exclusive.");
        return CommandLineParseError;
    }

    if (parser.isSet(qmakeOption) && optVerboseLevel >= 1)
        std::wcerr << "Warning: -qmake option is deprecated. Use -bobuipaths instead.\n";

    if (parser.isSet(bobuipathsOption) || parser.isSet(qmakeOption)) {
        const QString bobuipathsArg = parser.isSet(bobuipathsOption) ? parser.value(bobuipathsOption)
                                                                : parser.value(qmakeOption);

        const QString bobuipathsBinary = QDir::cleanPath(bobuipathsArg);
        const QFileInfo fi(bobuipathsBinary);
        if (!fi.exists()) {
            *errorMessage = msgFileDoesNotExist(bobuipathsBinary);
            return CommandLineParseError;
        }

        if (!fi.isExecutable()) {
            *errorMessage = u'"' + QDir::toNativeSeparators(bobuipathsBinary)
                    + QStringLiteral("\" is not an executable.");
            return CommandLineParseError;
        }
        options->bobuipathsBinary = bobuipathsBinary;
    }

    if (parser.isSet(verboseOption)) {
        bool ok;
        const QString value = parser.value(verboseOption);
        optVerboseLevel = value.toInt(&ok);
        if (!ok || optVerboseLevel < 0) {
            *errorMessage = QStringLiteral("Invalid value \"%1\" passed for verbose level.")
                    .arg(value);
            return CommandLineParseError;
        }
    }

    return 0;
}

static inline int parseArguments(const QStringList &arguments, QCommandLineParser *parser,
                                 Options *options, QString *errorMessage)
{
    using CommandLineOptionPtr = QSharedPointer<QCommandLineOption>;
    using OptionPtrVector = QList<CommandLineOptionPtr>;

    parser->setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser->setApplicationDescription(u"BobUI Deploy Tool " BOBUI_VERSION_STR
        "\n\nThe simplest way to use windeploybobui is to add the bin directory of your BobUI\n"
        "installation (e.g. <BOBUI_DIR\\bin>) to the PATH variable and then run:\n  windeploybobui <path-to-app-binary>\n\n"
        "If your application uses BobUI Quick, run:\n  windeploybobui --qmldir <path-to-app-qml-files> <path-to-app-binary>"_s);
    const QCommandLineOption helpOption = parser->addHelpOption();
    const QCommandLineOption versionOption = parser->addVersionOption();

    QCommandLineOption dirOption(QStringLiteral("dir"),
                                 QStringLiteral("Use directory instead of binary directory."),
                                 QStringLiteral("directory"));
    parser->addOption(dirOption);

    // Add early options to have them available in the help text.
    parser->addOption(createQMakeOption());
    parser->addOption(createBobUIPathsOption());

    QCommandLineOption libDirOption(QStringLiteral("libdir"),
                                    QStringLiteral("Copy libraries to path."),
                                    QStringLiteral("path"));
    parser->addOption(libDirOption);

    QCommandLineOption pluginDirOption(QStringLiteral("plugindir"),
                                       QStringLiteral("Copy plugins to path."),
                                       QStringLiteral("path"));
    parser->addOption(pluginDirOption);

    const QCommandLineOption translationDirOption(
        u"translationdir"_s,
        u"Copy translations to path."_s,
        u"path"_s);
    parser->addOption(translationDirOption);

    QCommandLineOption qmlDeployDirOption(QStringLiteral("qml-deploy-dir"),
                                          QStringLiteral("Copy qml files to path."),
                                          QStringLiteral("path"));
    parser->addOption(qmlDeployDirOption);

    QCommandLineOption debugOption(QStringLiteral("debug"),
                                   QStringLiteral("Assume debug binaries."));
    parser->addOption(debugOption);
    QCommandLineOption releaseOption(QStringLiteral("release"),
                                   QStringLiteral("Assume release binaries."));
    parser->addOption(releaseOption);
    QCommandLineOption releaseWithDebugInfoOption(QStringLiteral("release-with-debug-info"),
                                                  QStringLiteral("Assume release binaries with debug information."));
    releaseWithDebugInfoOption.setFlags(QCommandLineOption::HiddenFromHelp); // Deprecated by improved debug detection.
    parser->addOption(releaseWithDebugInfoOption);

    QCommandLineOption deployPdbOption(QStringLiteral("pdb"),
                                       QStringLiteral("Deploy .pdb files (MSVC)."));
    parser->addOption(deployPdbOption);

    QCommandLineOption forceOption(QStringLiteral("force"),
                                    QStringLiteral("Force updating files."));
    parser->addOption(forceOption);

    QCommandLineOption dryRunOption(QStringLiteral("dry-run"),
                                    QStringLiteral("Simulation mode. Behave normally, but do not copy/update any files."));
    parser->addOption(dryRunOption);

    QCommandLineOption noPatchBobUIOption(QStringLiteral("no-patchbobui"),
                                       QStringLiteral("Do not patch the BobUI6Core library."));
    parser->addOption(noPatchBobUIOption);

    QCommandLineOption ignoreErrorOption(QStringLiteral("ignore-library-errors"),
                                         QStringLiteral("Ignore errors when libraries cannot be found."));
    parser->addOption(ignoreErrorOption);

    QCommandLineOption noPluginsOption(QStringLiteral("no-plugins"),
                                       QStringLiteral("Skip plugin deployment."));
    parser->addOption(noPluginsOption);

    QCommandLineOption skipPluginTypesOption(QStringLiteral("skip-plugin-types"),
                                         QStringLiteral("A comma-separated list of plugin types that are not deployed (qmltooling,generic)."),
                                         QStringLiteral("plugin types"));
    parser->addOption(skipPluginTypesOption);

    QCommandLineOption addPluginTypesOption(QStringLiteral("add-plugin-types"),
                                            QStringLiteral("A comma-separated list of plugin types that will be added to deployment (imageformats,iconengines)"),
                                            QStringLiteral("plugin types"));
    parser->addOption(addPluginTypesOption);

    QCommandLineOption includePluginsOption(QStringLiteral("include-plugins"),
                                            QStringLiteral("A comma-separated list of individual plugins that will be added to deployment (scene2d,qjpeg)"),
                                            QStringLiteral("plugins"));
    parser->addOption(includePluginsOption);

    QCommandLineOption excludePluginsOption(QStringLiteral("exclude-plugins"),
                                            QStringLiteral("A comma-separated list of individual plugins that will not be deployed (qsvg,qpdf)"),
                                            QStringLiteral("plugins"));
    parser->addOption(excludePluginsOption);

    QCommandLineOption noLibraryOption(QStringLiteral("no-libraries"),
                                       QStringLiteral("Skip library deployment."));
    parser->addOption(noLibraryOption);

    QCommandLineOption qmlDirOption(QStringLiteral("qmldir"),
                                    QStringLiteral("Scan for QML-imports starting from directory."),
                                    QStringLiteral("directory"));
    parser->addOption(qmlDirOption);

    QCommandLineOption qmlImportOption(QStringLiteral("qmlimport"),
                                       QStringLiteral("Add the given path to the QML module search locations."),
                                       QStringLiteral("directory"));
    parser->addOption(qmlImportOption);

    QCommandLineOption qmlImportTimeoutOption(QStringLiteral("qmlimporttimeout"),
                                       QStringLiteral("Set timeout (in ms for) execution of "
                                                             "qmlimportscanner."),
                                       QStringLiteral("timeout"));
    parser->addOption(qmlImportTimeoutOption);

    QCommandLineOption noQuickImportOption(QStringLiteral("no-quick-import"),
                                           QStringLiteral("Skip deployment of BobUI Quick imports."));
    parser->addOption(noQuickImportOption);


    QCommandLineOption translationOption(QStringLiteral("translations"),
                                         QStringLiteral("A comma-separated list of languages to deploy (de,fi)."),
                                         QStringLiteral("languages"));
    parser->addOption(translationOption);

    QCommandLineOption noTranslationOption(QStringLiteral("no-translations"),
                                           QStringLiteral("Skip deployment of translations."));
    parser->addOption(noTranslationOption);

    QCommandLineOption noSystemD3DCompilerOption(QStringLiteral("no-system-d3d-compiler"),
                                                 QStringLiteral("Skip deployment of the system D3D compiler."));
    parser->addOption(noSystemD3DCompilerOption);

    QCommandLineOption noSystemDxcOption(QStringLiteral("no-system-dxc-compiler"),
                                         QStringLiteral("Skip deployment of the system DXC (dxcompiler.dll, dxil.dll)."));
    parser->addOption(noSystemDxcOption);


    QCommandLineOption compilerRunTimeOption(QStringLiteral("compiler-runtime"),
                                             QStringLiteral("Deploy compiler runtime (Desktop only)."));
    parser->addOption(compilerRunTimeOption);

    QCommandLineOption noCompilerRunTimeOption(QStringLiteral("no-compiler-runtime"),
                                             QStringLiteral("Do not deploy compiler runtime (Desktop only)."));
    parser->addOption(noCompilerRunTimeOption);

    QCommandLineOption jsonOption(QStringLiteral("json"),
                                  QStringLiteral("Print to stdout in JSON format."));
    parser->addOption(jsonOption);

    QCommandLineOption suppressSoftwareRasterizerOption(QStringLiteral("no-opengl-sw"),
                                                        QStringLiteral("Do not deploy the software rasterizer library."));
    parser->addOption(suppressSoftwareRasterizerOption);

    QCommandLineOption noFFmpegOption(QStringLiteral("no-ffmpeg"),
                                      QStringLiteral("Do not deploy the FFmpeg libraries."));
    parser->addOption(noFFmpegOption);

    QCommandLineOption forceOpenSslOption(QStringLiteral("force-openssl"),
                                      QStringLiteral("Deploy openssl plugin but ignore openssl library dependency"));
    parser->addOption(forceOpenSslOption);

    QCommandLineOption openSslRootOption(QStringLiteral("openssl-root"),
                                 QStringLiteral("Directory containing openSSL libraries."),
                                 QStringLiteral("directory"));
    parser->addOption(openSslRootOption);


    QCommandLineOption listOption(QStringLiteral("list"),
                                                "Print only the names of the files copied.\n"
                                                "Available options:\n"
                                                "  source:   absolute path of the source files\n"
                                                "  target:   absolute path of the target files\n"
                                                "  relative: paths of the target files, relative\n"
                                                "            to the target directory\n"
                                                "  mapping:  outputs the source and the relative\n"
                                                "            target, suitable for use within an\n"
                                                "            Appx mapping file"_L1,
                                  QStringLiteral("option"));
    parser->addOption(listOption);

    QCommandLineOption appxOption(QStringLiteral("appx"),
                                  QStringLiteral("Create an appx package for the Windows Store"));
    parser->addOption(appxOption);

    QCommandLineOption appxCertificatePath(QStringLiteral("appx-certificate"),
                                          QStringLiteral("Path to appx certificate to sign appx package"),
                                          QStringLiteral(".cer file"));
    parser->addOption(appxCertificatePath);

    // Add early option to have it available in the help text.
    parser->addOption(createVerboseOption());

    parser->addPositionalArgument(QStringLiteral("[files]"),
                                  QStringLiteral("Binaries or directory containing the binary."));

    QCommandLineOption deployInsightTrackerOption(QStringLiteral("deploy-insighttracker"),
                                                  QStringLiteral("Deploy insight tracker plugin."));
    // The option will be added to the parser if the module is available (see block below)
    bool insightTrackerModuleAvailable = false;

    OptionPtrVector enabledModuleOptions;
    OptionPtrVector disabledModuleOptions;
    const size_t bobuiModulesCount = bobuiModuleEntries.size();
    enabledModuleOptions.reserve(bobuiModulesCount);
    disabledModuleOptions.reserve(bobuiModulesCount);
    for (const BobUIModule &module : bobuiModuleEntries) {
        const QString option = moduleNameToOptionName(module.name, module.internal);
        const QString name = module.name;
        if (name == u"InsightTracker") {
            parser->addOption(deployInsightTrackerOption);
            insightTrackerModuleAvailable = true;
        }
        const QString enabledDescription = QStringLiteral("Add ") + name + QStringLiteral(" module.");
        CommandLineOptionPtr enabledOption(new QCommandLineOption(option, enabledDescription));
        parser->addOption(*enabledOption.data());
        enabledModuleOptions.append(enabledOption);
        const QString disabledDescription = QStringLiteral("Remove ") + name + QStringLiteral(" module.");
        CommandLineOptionPtr disabledOption(new QCommandLineOption(QStringLiteral("no-") + option,
                                                                   disabledDescription));
        disabledModuleOptions.append(disabledOption);
        parser->addOption(*disabledOption.data());
    }

    const bool success = parser->parse(arguments);
    if (parser->isSet(helpOption))
        return CommandLineParseHelpRequested;
    if (parser->isSet(versionOption))
        return CommandLineVersionRequested;
    if (!success) {
        *errorMessage = parser->errorText();
        return CommandLineParseError;
    }

    options->libraryDirectory = parser->value(libDirOption);
    options->pluginDirectory = parser->value(pluginDirOption);
    options->translationsDirectory = parser->value(translationDirOption);
    options->qmlDirectory = parser->value(qmlDeployDirOption);
    options->plugins = !parser->isSet(noPluginsOption);
    options->libraries = !parser->isSet(noLibraryOption);
    options->translations = !parser->isSet(noTranslationOption);
    if (parser->isSet(translationOption))
        options->languages = parser->value(translationOption).split(u',');
    options->systemD3dCompiler = !parser->isSet(noSystemD3DCompilerOption);
    options->systemDxc = !parser->isSet(noSystemDxcOption);
    options->quickImports = !parser->isSet(noQuickImportOption);
    options->createAppx = parser->isSet(appxOption);
    if (options->createAppx) {
        if (!parser->isSet(appxCertificatePath)) {
            *errorMessage = QStringLiteral("--appx requires --appx-certificate with a valid certificate");
            return CommandLineParseError;
        }
        options->appxCertificatePath = parser->value(appxCertificatePath);
    }

    // default to deployment of compiler runtime for windows desktop configurations
    if (options->platform == WindowsDesktopMinGW || options->platform == WindowsDesktopClangMinGW
        || options->platform.testFlags(WindowsDesktopMsvc) || parser->isSet(compilerRunTimeOption))
        options->compilerRunTime = true;
    if (parser->isSet(noCompilerRunTimeOption))
        options->compilerRunTime = false;

    if (options->compilerRunTime && options->platform != WindowsDesktopMinGW
        && options->platform != WindowsDesktopClangMinGW
        && !options->platform.testFlags(WindowsDesktopMsvc)) {
        *errorMessage = QStringLiteral("Deployment of the compiler runtime is implemented for "
            "Desktop MSVC and MinGW (g++ and Clang) only.");
        return CommandLineParseError;
    }

    if (parser->isSet(skipPluginTypesOption))
        options->pluginSelections.disabledPluginTypes = parser->value(skipPluginTypesOption).split(u',');

    if (parser->isSet(addPluginTypesOption))
        options->pluginSelections.enabledPluginTypes = parser->value(addPluginTypesOption).split(u',');

    if (parser->isSet(includePluginsOption))
        options->pluginSelections.includedPlugins = parser->value(includePluginsOption).split(u',');

    if (parser->isSet(excludePluginsOption))
        options->pluginSelections.excludedPlugins = parser->value(excludePluginsOption).split(u',');

    if (parser->isSet(releaseWithDebugInfoOption))
        std::wcerr << "Warning: " << releaseWithDebugInfoOption.names().first() << " is obsolete.";

    switch (parseExclusiveOptions(parser, debugOption, releaseOption)) {
    case OptionAuto:
        break;
    case OptionEnabled:
        options->debugDetection = Options::DebugDetectionForceDebug;
        break;
    case OptionDisabled:
        options->debugDetection = Options::DebugDetectionForceRelease;
        break;
    }

    if (parser->isSet(deployPdbOption)) {
        if (options->platform.testFlag(WindowsBased) && !options->platform.testFlag(MinGW))
            options->deployPdb = true;
        else
            std::wcerr << "Warning: --" << deployPdbOption.names().first() << " is not supported on this platform.\n";
    }

    if (parser->isSet(suppressSoftwareRasterizerOption))
        options->softwareRasterizer = false;

    if (parser->isSet(noFFmpegOption))
        options->ffmpeg = false;

    if (parser->isSet(forceOpenSslOption))
        options->forceOpenSslPlugin = true;

    if (parser->isSet(openSslRootOption))
        options->openSslRootDirectory = parser->value(openSslRootOption);

    if (options->forceOpenSslPlugin && !options->openSslRootDirectory.isEmpty()) {
        *errorMessage = QStringLiteral("force-openssl and openssl-root are mutually exclusive");
        return CommandLineParseError;
    }

    if (parser->isSet(forceOption))
        options->updateFileFlags |= ForceUpdateFile;
    if (parser->isSet(dryRunOption)) {
        options->dryRun = true;
        options->updateFileFlags |= SkipUpdateFile;
    }

    options->patchBobUI = !parser->isSet(noPatchBobUIOption);
    options->ignoreLibraryErrors = parser->isSet(ignoreErrorOption);
    if (insightTrackerModuleAvailable)
        options->deployInsightTrackerPlugin = parser->isSet(deployInsightTrackerOption);

    for (const BobUIModule &module : bobuiModuleEntries) {
        if (parser->isSet(*enabledModuleOptions.at(module.id)))
            options->additionalLibraries[module.id] = 1;
        if (parser->isSet(*disabledModuleOptions.at(module.id)))
            options->disabledLibraries[module.id] = 1;
    }

    // Add some dependencies
    if (options->additionalLibraries.test(BobUIQuickModuleId))
        options->additionalLibraries[BobUIQmlModuleId] = 1;
    if (options->additionalLibraries.test(BobUIDesignerComponentsModuleId))
        options->additionalLibraries[BobUIDesignerModuleId] = 1;

    if (parser->isSet(listOption)) {
        const QString value = parser->value(listOption);
        if (value == QStringLiteral("source")) {
            options->list = ListSource;
        } else if (value == QStringLiteral("target")) {
            options->list = ListTarget;
        } else if (value == QStringLiteral("relative")) {
            options->list = ListRelative;
        } else if (value == QStringLiteral("mapping")) {
            options->list = ListMapping;
        } else {
            *errorMessage = QStringLiteral("Please specify a valid option for -list (source, target, relative, mapping).");
            return CommandLineParseError;
        }
    }

    if (parser->isSet(jsonOption) || options->list) {
        optVerboseLevel = 0;
        options->json = new JsonOutput;
    }

    const QStringList posArgs = parser->positionalArguments();
    if (posArgs.isEmpty()) {
        *errorMessage = QStringLiteral("Please specify the binary or folder.");
        return CommandLineParseError | CommandLineParseHelpRequested;
    }

    if (parser->isSet(dirOption))
        options->directory = parser->value(dirOption);

    if (parser->isSet(qmlDirOption))
        options->qmlDirectories = parser->values(qmlDirOption);

    if (parser->isSet(qmlImportOption))
        options->qmlImportPaths = parser->values(qmlImportOption);

    if (parser->isSet(qmlImportTimeoutOption)) {
        const QString timeoutString = parser->value(qmlImportTimeoutOption);
        bool ok;
        int timeout = timeoutString.toInt(&ok);
        if (!ok) {
            *errorMessage = u'"' + timeoutString + QStringLiteral("\" is not an acceptable timeout "
                                                                  "value.");
            return CommandLineParseError;
        }
        options->qmlImportTimeout = timeout;
    }

    const QString &file = posArgs.front();
    const QFileInfo fi(QDir::cleanPath(file));
    if (!fi.exists()) {
        *errorMessage = msgFileDoesNotExist(file);
        return CommandLineParseError;
    }

    if (!options->directory.isEmpty() && !fi.isFile()) { // -dir was specified - expecting file.
        *errorMessage = u'"' + file + QStringLiteral("\" is not an executable file.");
        return CommandLineParseError;
    }

    if (fi.isFile()) {
        options->binaries.append(fi.absoluteFilePath());
        if (options->directory.isEmpty())
            options->directory = fi.absolutePath();
    } else {
        const QString binary = findBinary(fi.absoluteFilePath(), options->platform);
        if (binary.isEmpty()) {
            *errorMessage = QStringLiteral("Unable to find binary in \"") + file + u'"';
            return CommandLineParseError;
        }
        options->directory = fi.absoluteFilePath();
        options->binaries.append(binary);
    } // directory.

    // Remaining files or plugin directories
    bool multipleDirs = false;
    for (int i = 1; i < posArgs.size(); ++i) {
        const QFileInfo fi(QDir::cleanPath(posArgs.at(i)));
        const QString path = fi.absoluteFilePath();
        if (!fi.exists()) {
            *errorMessage = msgFileDoesNotExist(path);
            return CommandLineParseError;
        }
        if (fi.isDir()) {
            const QStringList libraries =
                findSharedLibraries(QDir(path), options->platform, MatchDebugOrRelease, QString());
            for (const QString &library : libraries)
                options->binaries.append(path + u'/' + library);
        } else {
            if (!parser->isSet(dirOption) && fi.absolutePath() != options->directory)
                multipleDirs = true;
            options->binaries.append(path);
        }
    }
    if (multipleDirs) {
        std::wcerr << "Warning: using binaries from different directories, deploying to following path: "
        << options->directory << '\n' << "To disable this warning, use the --dir option\n";
    }
    if (options->translationsDirectory.isEmpty())
        options->translationsDirectory = options->directory + "/translations"_L1;
    return 0;
}

// Simple line wrapping at 80 character boundaries.
static inline QString lineBreak(QString s)
{
    for (qsizetype i = 80; i < s.size(); i += 80) {
        const qsizetype lastBlank = s.lastIndexOf(u' ', i);
        if (lastBlank >= 0) {
            s[lastBlank] = u'\n';
            i = lastBlank + 1;
        }
    }
    return s;
}

static inline QString helpText(const QCommandLineParser &p, const PluginInformation &pluginInfo)
{
    QString result = p.helpText();
    // Replace the default-generated text which is too long by a short summary
    // explaining how to enable single libraries.
    if (bobuiModuleEntries.size() == 0)
        return result;
    const BobUIModule &firstModule = bobuiModuleEntries.moduleById(0);
    const QString firstModuleOption = moduleNameToOptionName(firstModule.name, firstModule.internal);
    const qsizetype moduleStart = result.indexOf("\n  --"_L1 + firstModuleOption);
    const qsizetype argumentsStart = result.lastIndexOf("\nArguments:"_L1);
    if (moduleStart >= argumentsStart)
        return result;
    QString moduleHelp;
    moduleHelp +=
        "\n\nBobUI libraries can be added by passing their name (-xml) or removed by passing\n"
        "the name prepended by --no- (--no-xml). Available libraries:\n"_L1;
    ModuleBitset mask;
    moduleHelp += lineBreak(QString::fromLatin1(formatBobUIModules(mask.set(), true)));
    moduleHelp += u"\n\n";
    moduleHelp +=
            u"BobUI plugins can be included or excluded individually or by type.\n"
            u"To deploy or block plugins individually, use the --include-plugins\n"
            u"and --exclude-plugins options (--include-plugins qjpeg,qsvgicon)\n"
            u"You can also use the --skip-plugin-types or --add-plugin-types to\n"
            u"achieve similar results with entire plugin groups, like imageformats, e.g.\n"
            u"(--add-plugin-types imageformats,iconengines). Exclusion always takes\n"
            u"precedence over inclusion, and types take precedence over specific plugins.\n"
            u"For example, including qjpeg, but skipping imageformats, will NOT deploy qjpeg.\n"
            u"\nDetected available plugins:\n";
    moduleHelp += formatBobUIPlugins(pluginInfo);
    result.replace(moduleStart, argumentsStart - moduleStart, moduleHelp);
    return result;
}

static inline bool isBobUIModule(const QString &libName)
{
    // Match Standard modules named BobUI6XX.dll
    if (libName.size() < 3 || !libName.startsWith("BobUI"_L1, BobUI::CaseInsensitive))
        return false;
    const QChar version = libName.at(2);
    return version.isDigit() && (version.toLatin1() - '0') == BOBUI_VERSION_MAJOR;
}

// Helper for recursively finding all dependent BobUI libraries.
static bool findDependentBobUILibraries(const QString &bobuiBinDir, const QString &binary,
                                     Platform platform, QString *errorMessage,
                                     QStringList *bobuiDependencies, QStringList *nonBobUIDependencies)
{
    QStringList dependentLibs;
    if (!readPeExecutableDependencies(binary, errorMessage, &dependentLibs)) {
        errorMessage->prepend("Unable to find dependent libraries of "_L1 +
                              QDir::toNativeSeparators(binary) + " :"_L1);
        return false;
    }
    // Filter out the BobUI libraries. Note that depends.exe finds libs from optDirectory if we
    // are run the 2nd time (updating). We want to check against the BobUI bin dir libraries
    const int start = bobuiDependencies->size();
    for (const QString &lib : std::as_const(dependentLibs)) {
        if (isBobUIModule(lib)) {
            const QString path = normalizeFileName(bobuiBinDir + u'/' + QFileInfo(lib).fileName());
            if (!bobuiDependencies->contains(path))
                bobuiDependencies->append(path);
        } else if (nonBobUIDependencies && !nonBobUIDependencies->contains(lib)) {
            nonBobUIDependencies->append(lib);
        }
    }
    const int end = bobuiDependencies->size();
    // Recurse
    for (int i = start; i < end; ++i) {
        if (!findDependentBobUILibraries(bobuiBinDir, bobuiDependencies->at(i), platform, errorMessage,
                                      bobuiDependencies, nonBobUIDependencies)) {
            return false;
        }
    }
    return true;
}

// Base class to filter debug/release Windows DLLs for functions to be passed to updateFile().
// Tries to pre-filter by namefilter and does check via PE.
class DllDirectoryFileEntryFunction {
public:
    explicit DllDirectoryFileEntryFunction(Platform platform,
                                           DebugMatchMode debugMatchMode, const QString &prefix = QString()) :
        m_platform(platform), m_debugMatchMode(debugMatchMode), m_prefix(prefix) {}

    QStringList operator()(const QDir &dir) const
        { return findSharedLibraries(dir, m_platform, m_debugMatchMode, m_prefix); }

private:
    const Platform m_platform;
    const DebugMatchMode m_debugMatchMode;
    const QString m_prefix;
};

static QString pdbFileName(QString libraryFileName)
{
    const qsizetype lastDot = libraryFileName.lastIndexOf(u'.') + 1;
    if (lastDot <= 0)
        return QString();
    libraryFileName.replace(lastDot, libraryFileName.size() - lastDot, "pdb"_L1);
    return libraryFileName;
}
static inline QStringList qmlCacheFileFilters()
{
    return QStringList() << QStringLiteral("*.jsc") << QStringLiteral("*.qmlc");
}

// File entry filter function for updateFile() that returns a list of files for
// QML import trees: DLLs (matching debug) and .qml/,js, etc.
class QmlDirectoryFileEntryFunction {
public:
    enum Flags {
        DeployPdb = 0x1,
        SkipSources = 0x2
    };

    explicit QmlDirectoryFileEntryFunction(
        const QString &moduleSourcePath, Platform platform, DebugMatchMode debugMatchMode, unsigned flags)
        : m_flags(flags), m_qmlNameFilter(QmlDirectoryFileEntryFunction::qmlNameFilters(flags))
        , m_dllFilter(platform, debugMatchMode), m_moduleSourcePath(moduleSourcePath)
    {}

    QStringList operator()(const QDir &dir) const
    {
        if (moduleSourceDir(dir).canonicalPath() != m_moduleSourcePath) {
            // If we're in a different module, return nothing.
            return {};
        }

        QStringList result;
        const QStringList &libraries = m_dllFilter(dir);
        for (const QString &library : libraries) {
            result.append(library);
            if (m_flags & DeployPdb) {
                const QString pdb = pdbFileName(library);
                if (QFileInfo(dir.absoluteFilePath(pdb)).isFile())
                    result.append(pdb);
            }
        }
        result.append(m_qmlNameFilter(dir));
        return result;
    }

private:
    static QDir moduleSourceDir(const QDir &dir)
    {
        QDir moduleSourceDir = dir;
        while (!moduleSourceDir.exists(QStringLiteral("qmldir"))) {
            if (!moduleSourceDir.cdUp()) {
                return {};
            }
        }
        return moduleSourceDir;
    }

    static inline QStringList qmlNameFilters(unsigned flags)
    {
        QStringList result;
        result << QStringLiteral("qmldir") << QStringLiteral("*.qmltypes")
           << QStringLiteral("*.frag") << QStringLiteral("*.vert") // Shaders
           << QStringLiteral("*.ttf");
        if (!(flags & SkipSources)) {
            result << QStringLiteral("*.js") << QStringLiteral("*.qml") << QStringLiteral("*.png");
            result.append(qmlCacheFileFilters());
        }
        return result;
    }

    const unsigned m_flags;
    NameFilterFileEntryFunction m_qmlNameFilter;
    DllDirectoryFileEntryFunction m_dllFilter;
    QString m_moduleSourcePath;
};

static qint64 bobuiModule(QString module, const QString &infix)
{
    // Match needle 'path/BobUI6Core<infix><d>.dll' or 'path/libBobUI6Core<infix>.so.5.0'
    const qsizetype lastSlashPos = module.lastIndexOf(u'/');
    if (lastSlashPos > 0)
        module.remove(0, lastSlashPos + 1);
    if (module.startsWith("lib"_L1))
        module.remove(0, 3);
    int endPos = infix.isEmpty() ? -1 : module.lastIndexOf(infix);
    if (endPos == -1)
        endPos = module.indexOf(u'.'); // strip suffixes '.so.5.0'.
    if (endPos > 0)
        module.truncate(endPos);
    // That should leave us with 'BobUI6Core<d>'.
    for (const auto &bobuiModule : bobuiModuleEntries) {
        const QString &libraryName = bobuiModule.name;
        if (module == libraryName
            || (module.size() == libraryName.size() + 1 && module.startsWith(libraryName))) {
            return bobuiModule.id;
        }
    }
    std::wcerr << "Warning: module " << qPrintable(module) << " could not be found\n";
    return -1;
}

// Return the path if a plugin is to be deployed
static QString deployPlugin(const QString &plugin, const QDir &subDir, const bool dueToModule,
                            const DebugMatchMode &debugMatchMode, ModuleBitset *pluginNeededBobUIModules,
                            const ModuleBitset &disabledBobUIModules,
                            const PluginSelections &pluginSelections, const QString &libraryLocation,
                            const QString &infix, Platform platform,
                            bool deployInsightTrackerPlugin, bool deployOpenSslPlugin)
{
    const QString subDirName = subDir.dirName();
    // Filter out disabled plugins
    if (optVerboseLevel && pluginSelections.disabledPluginTypes.contains(subDirName)) {
        std::wcout << "Skipping plugin " << plugin << " due to skipped plugin type " << subDirName << '\n';
        return {};
    }
    if (optVerboseLevel && subDirName == u"generic" && plugin.contains(u"qinsighttracker")
        && !deployInsightTrackerPlugin) {
        std::wcout << "Skipping plugin " << plugin
                   << ". Use -deploy-insighttracker if you want to use it.\n";
        return {};
    }
    if (optVerboseLevel && subDirName == u"tls" && plugin.contains(u"qopensslbackend")
        && !deployOpenSslPlugin) {
        std::wcout << "Skipping plugin " << plugin
                   << ". Use -force-openssl or specify -openssl-root if you want to use it.\n";
        return {};
    }

    const int dotIndex = plugin.lastIndexOf(u'.');
    // Strip the .dll from the name, and an additional 'd' if it's a debug library with the 'd'
    // suffix
    const int stripIndex = debugMatchMode == MatchDebug && platformHasDebugSuffix(platform)
            ? dotIndex - 1
            : dotIndex;
    const QString pluginName = plugin.first(stripIndex);

    if (optVerboseLevel && pluginSelections.excludedPlugins.contains(pluginName)) {
        std::wcout << "Skipping plugin " << plugin << " due to exclusion option" << '\n';
        return {};
    }

    // By default, only deploy qwindows.dll
    if (subDirName == u"platforms"
        && !(pluginSelections.includedPlugins.contains(pluginName)
             || (pluginSelections.enabledPluginTypes.contains(subDirName)))
        && !pluginName.startsWith(u"qwindows")) {
        return {};
    }

    const QString pluginPath = subDir.absoluteFilePath(plugin);

    // If dueToModule is false, check if the user included the plugin or the entire type. In the
    // former's case, only deploy said plugin and not all plugins of that type.
    const bool requiresPlugin = pluginSelections.includedPlugins.contains(pluginName)
            || pluginSelections.enabledPluginTypes.contains(subDirName);
    if (!dueToModule && !requiresPlugin)
        return {};

    // Deploy QUiTools plugins as is without further dependency checking.
    // The user needs to ensure all required libraries are present (would
    // otherwise pull BobUIWebEngine for its plugin).
    if (subDirName == u"designer")
        return pluginPath;

    QStringList dependentBobUILibs;
    QString errorMessage;
    if (findDependentBobUILibraries(libraryLocation, pluginPath, platform,
                                 &errorMessage, &dependentBobUILibs, nullptr)) {
        for (int d = 0; d < dependentBobUILibs.size(); ++d) {
            const qint64 module = bobuiModule(dependentBobUILibs.at(d), infix);
            if (module >= 0)
                (*pluginNeededBobUIModules)[module] = 1;
        }
    } else {
        std::wcerr << "Warning: Cannot determine dependencies of "
            << QDir::toNativeSeparators(pluginPath) << ": " << errorMessage << '\n';
    }

    ModuleBitset disabledNeededBobUIModules;
    disabledNeededBobUIModules = *pluginNeededBobUIModules & disabledBobUIModules;
    if (disabledNeededBobUIModules.any()) {
        if (optVerboseLevel) {
            std::wcout << "Skipping plugin " << plugin
                << " due to disabled dependencies ("
                << formatBobUIModules(disabledNeededBobUIModules).constData() << ").\n";
        }
        return {};
    }

    return pluginPath;
}

static bool needsPluginType(const QString &subDirName, const PluginInformation &pluginInfo,
                            const PluginSelections &pluginSelections)
{
    bool needsTypeForPlugin = false;
    for (const QString &plugin: pluginSelections.includedPlugins) {
        if (pluginInfo.isTypeForPlugin(subDirName, plugin))
            needsTypeForPlugin = true;
    }
    return (pluginSelections.enabledPluginTypes.contains(subDirName) || needsTypeForPlugin);
}

QStringList findBobUIPlugins(ModuleBitset *usedBobUIModules, const ModuleBitset &disabledBobUIModules,
                          const PluginInformation &pluginInfo, const PluginSelections &pluginSelections,
                          const QString &bobuiPluginsDirName, const QString &libraryLocation,
                          const QString &infix, DebugMatchMode debugMatchModeIn, Platform platform,
                          QString *platformPlugin, bool deployInsightTrackerPlugin,
                          bool deployOpenSslPlugin)
{
    if (bobuiPluginsDirName.isEmpty())
        return QStringList();
    QDir pluginsDir(bobuiPluginsDirName);
    QStringList result;
    bool missingBobUIModulesAdded = false;
    const QFileInfoList &pluginDirs = pluginsDir.entryInfoList(QStringList(u"*"_s), QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &subDirFi : pluginDirs) {
        const QString subDirName = subDirFi.fileName();
        const size_t module = bobuiModuleEntries.moduleIdForPluginType(subDirName);
        if (module == BobUIModule::InvalidId) {
            if (optVerboseLevel > 1) {
                std::wcerr << "No BobUI module found for plugin type \"" << subDirName << "\".\n";
            }
            continue;
        }
        const bool dueToModule = usedBobUIModules->test(module);
        if (dueToModule || needsPluginType(subDirName, pluginInfo, pluginSelections)) {
            const DebugMatchMode debugMatchMode = (module == BobUIWebEngineCoreModuleId)
                ? MatchDebugOrRelease // BOBUIBUG-44331: Debug detection does not work for webengine, deploy all.
                : debugMatchModeIn;
            QDir subDir(subDirFi.absoluteFilePath());
            if (optVerboseLevel)
                std::wcout << "Adding in plugin type " << subDirFi.baseName() << " for module: " << bobuiModuleEntries.moduleById(module).name << '\n';

            const bool isPlatformPlugin = subDirName == "platforms"_L1;
            const QStringList plugins =
                    findSharedLibraries(subDir, platform, debugMatchMode, QString());
            for (const QString &plugin : plugins) {
                ModuleBitset pluginNeededBobUIModules;
                const QString pluginPath =
                        deployPlugin(plugin, subDir, dueToModule, debugMatchMode, &pluginNeededBobUIModules,
                                     disabledBobUIModules, pluginSelections, libraryLocation, infix,
                                     platform, deployInsightTrackerPlugin, deployOpenSslPlugin);
                if (!pluginPath.isEmpty()) {
                    if (isPlatformPlugin && plugin.startsWith(u"qwindows"))
                        *platformPlugin = subDir.absoluteFilePath(plugin);
                    result.append(pluginPath);

                    const ModuleBitset missingModules = (pluginNeededBobUIModules & ~*usedBobUIModules);
                    if (missingModules.any()) {
                        *usedBobUIModules |= missingModules;
                        missingBobUIModulesAdded = true;
                        if (optVerboseLevel) {
                            std::wcout << "Adding " << formatBobUIModules(missingModules).constData()
                                       << " for " << plugin << " from plugin type: " << subDirName << '\n';
                        }
                    }
                }
            } // for filter
        } // type matches
    } // for plugin folder

    // If missing BobUI modules were added during plugin deployment make additional pass, because we may need
    // additional plugins.
    if (missingBobUIModulesAdded) {
        if (optVerboseLevel) {
            std::wcout << "Performing additional pass of finding BobUI plugins due to updated BobUI module list: "
                       << formatBobUIModules(*usedBobUIModules).constData() << "\n";
        }
        return findBobUIPlugins(usedBobUIModules, disabledBobUIModules, pluginInfo, pluginSelections, bobuiPluginsDirName,
                             libraryLocation, infix, debugMatchModeIn, platform, platformPlugin,
                             deployInsightTrackerPlugin, deployOpenSslPlugin);
    }

    return result;
}

static QStringList translationNameFilters(const ModuleBitset &modules, const QString &prefix)
{
    QStringList result;
    for (const auto &bobuiModule : bobuiModuleEntries) {
        if (modules.test(bobuiModule.id) && !bobuiModule.translationCatalog.isEmpty()) {
            const QString name = bobuiModule.translationCatalog + u'_' + prefix + ".qm"_L1;
            if (!result.contains(name))
                result.push_back(name);
        }
    }
    return result;
}

static bool deployTranslations(const QString &sourcePath, const ModuleBitset &usedBobUIModules,
                               const QString &target, const Options &options,
                               QString *errorMessage)
{
    // Find available languages prefixes by checking on bobuibase.
    QStringList prefixes;
    QDir sourceDir(sourcePath);
    const QStringList qmFilter = QStringList(QStringLiteral("bobuibase_*.qm"));
    const QFileInfoList &qmFiles = sourceDir.entryInfoList(qmFilter);
    for (const QFileInfo &qmFi : qmFiles) {
        const QString prefix = qmFi.baseName().mid(7);
        if (options.languages.isEmpty() || options.languages.contains(prefix))
            prefixes.append(prefix);
    }
    if (prefixes.isEmpty()) {
        std::wcerr << "Warning: Could not find any translations in "
                   << QDir::toNativeSeparators(sourcePath) << " (developer build?)\n.";
        return true;
    }
    // Run lconvert to concatenate all files into a single named "bobui_<prefix>.qm" in the application folder
    // Use BOBUI_INSTALL_TRANSLATIONS as working directory to keep the command line short.
    const QString absoluteTarget = QFileInfo(target).absoluteFilePath();
    const QString binary = QStringLiteral("lconvert");
    QStringList arguments;
    for (const QString &prefix : std::as_const(prefixes)) {
        arguments.clear();
        const QString targetFile = QStringLiteral("bobui_") + prefix + QStringLiteral(".qm");
        arguments.append(QStringLiteral("-o"));
        const QString targetFilePath = absoluteTarget + u'/' + targetFile;
        if (options.json)
            options.json->addFile(sourcePath +  u'/' + targetFile, absoluteTarget);
        arguments.append(QDir::toNativeSeparators(targetFilePath));
        const QStringList translationFilters = translationNameFilters(usedBobUIModules, prefix);
        if (translationFilters.isEmpty()){
            std::wcerr << "Warning: translation catalogs are all empty, skipping translation deployment\n";
            return true;
        }
        const QFileInfoList &langQmFiles = sourceDir.entryInfoList(translationFilters);
        for (const QFileInfo &langQmFileFi : langQmFiles) {
            if (options.json) {
                options.json->addFile(langQmFileFi.absoluteFilePath(),
                                      absoluteTarget);
            }
            arguments.append(langQmFileFi.fileName());
        }
        if (optVerboseLevel)
            std::wcout << "Creating " << targetFile << "...\n";
        unsigned long exitCode;
        if ((options.updateFileFlags & SkipUpdateFile) == 0
            && (!runProcess(binary, arguments, sourcePath, &exitCode, nullptr, nullptr, errorMessage)
                || exitCode)) {
            return false;
        }
    } // for prefixes.
    return true;
}

static QStringList findFFmpegLibs(const QString &bobuiBinDir, Platform platform)
{
    const std::vector<QLatin1StringView> ffmpegHints = { "avcodec"_L1, "avformat"_L1, "avutil"_L1,
                                                         "swresample"_L1, "swscale"_L1 };
    const QStringList bundledLibs =
            findSharedLibraries(bobuiBinDir, platform, MatchDebugOrRelease, {});

    QStringList ffmpegLibs;
    for (const QLatin1StringView &libHint : ffmpegHints) {
        const QStringList ffmpegLib = bundledLibs.filter(libHint, BobUI::CaseInsensitive);

        if (ffmpegLib.empty()) {
            std::wcerr << "Warning: Cannot find FFmpeg libraries. Multimedia features will not work as expected.\n";
            return {};
        } else if (ffmpegLib.size() != 1u) {
            std::wcerr << "Warning: Multiple versions of FFmpeg libraries found. Multimedia features will not work as expected.\n";
            return {};
        }

        const QChar slash(u'/');
        QFileInfo ffmpegLibPath{ bobuiBinDir + slash + ffmpegLib.front() };
        ffmpegLibs.append(ffmpegLibPath.absoluteFilePath());
    }

    return ffmpegLibs;
}

// Find the openssl libraries BobUI executables depend on.
static QStringList findOpenSslLibraries(const QString &openSslRootDir, Platform platform)
{
    const std::vector<QLatin1StringView> libHints = { "libcrypto"_L1, "libssl"_L1 };
    const QChar slash(u'/');
    const QString openSslBinDir = openSslRootDir + slash + "bin"_L1;
    const QStringList openSslRootLibs =
            findSharedLibraries(openSslBinDir, platform, MatchDebugOrRelease, {});

    QStringList result;
    for (const QLatin1StringView &libHint : libHints) {
        const QStringList lib = openSslRootLibs.filter(libHint, BobUI::CaseInsensitive);

        if (lib.empty()) {
            std::wcerr << "Warning: Cannot find openssl libraries.\n";
            return {};
        } else if (lib.size() != 1u) {
            std::wcerr << "Warning: Multiple versions of openssl libraries found.\n";
            return {};
        }

        QFileInfo libPath{ openSslBinDir + slash + lib.front() };
        result.append(libPath.absoluteFilePath());
    }

    return result;
}


struct DeployResult
{
    operator bool() const { return success; }

    bool success = false;
    bool isDebug = false;
    ModuleBitset directlyUsedBobUILibraries;
    ModuleBitset usedBobUILibraries;
    ModuleBitset deployedBobUILibraries;
};

static QString libraryPath(const QString &libraryLocation, const char *name,
                           const QString &infix, Platform platform, bool debug)
{
    QString result = libraryLocation + u'/';
    result += QLatin1StringView(name);
    result += infix;
    if (debug && platformHasDebugSuffix(platform))
        result += u'd';
    result += sharedLibrarySuffix();
    return result;
}

static QString vcDebugRedistDir() { return QStringLiteral("Debug_NonRedist"); }

static QString vcRedistDir()
{
    const char vcDirVar[] = "VCINSTALLDIR";
    const QChar slash(u'/');
    QString vcRedistDirName = QDir::cleanPath(QFile::decodeName(qgetenv(vcDirVar)));
    if (vcRedistDirName.isEmpty()) {
        std::wcerr << "Warning: Cannot find Visual Studio installation directory, " << vcDirVar
            << " is not set.\n";
        return QString();
    }
    if (!vcRedistDirName.endsWith(slash))
        vcRedistDirName.append(slash);
    vcRedistDirName.append(QStringLiteral("redist/MSVC"));
    if (!QFileInfo(vcRedistDirName).isDir()) {
        std::wcerr << "Warning: Cannot find Visual Studio redist directory, "
            << QDir::toNativeSeparators(vcRedistDirName).toStdWString() << ".\n";
        return QString();
    }
    // Look in reverse order for folder containing the debug redist folder
    const QFileInfoList subDirs =
            QDir(vcRedistDirName)
                    .entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::Reversed);
    for (const QFileInfo &f : subDirs) {
        QString path = f.absoluteFilePath();
        if (QFileInfo(path + slash + vcDebugRedistDir()).isDir())
            return path;
        path += QStringLiteral("/onecore");
        if (QFileInfo(path + slash + vcDebugRedistDir()).isDir())
            return path;
    }
    std::wcerr << "Warning: Cannot find Visual Studio redist directory under "
               << QDir::toNativeSeparators(vcRedistDirName).toStdWString() << ".\n";
    return QString();
}

static QStringList findMinGWRuntimePaths(const QString &bobuiBinDir, Platform platform, const QStringList &runtimeFilters)
{
    //MinGW: Add runtime libraries. Check first for the BobUI binary directory, and default to path if nothing is found.
    QStringList result;
    const bool isClang = platform == WindowsDesktopClangMinGW;
    QStringList filters;
    const QString suffix = u'*' + sharedLibrarySuffix();
    for (const auto &minGWRuntime : runtimeFilters)
        filters.append(minGWRuntime + suffix);

    QFileInfoList dlls = QDir(bobuiBinDir).entryInfoList(filters, QDir::Files);
    if (dlls.isEmpty()) {
        std::wcerr << "Warning: Runtime libraries not found in BobUI binary folder, defaulting to looking in path\n";
        const QString binaryPath = isClang ? findInPath("clang++.exe"_L1) : findInPath("g++.exe"_L1);
        if (binaryPath.isEmpty()) {
            std::wcerr << "Warning: Cannot find " << (isClang ? "Clang" : "GCC") << " installation directory, " << (isClang ? "clang++" : "g++") << ".exe must be in the path\n";
            return {};
        }
        const QString binaryFolder = QFileInfo(binaryPath).absolutePath();
        dlls = QDir(binaryFolder).entryInfoList(filters, QDir::Files);
    }

    for (const QFileInfo &dllFi : dlls)
        result.append(dllFi.absoluteFilePath());

    return result;
}

static QStringList compilerRunTimeLibs(const QString &bobuiBinDir, Platform platform, bool isDebug, unsigned short machineArch)
{
    QStringList result;
    switch (platform) {
    case WindowsDesktopMinGW: {
        const QStringList minGWRuntimes = { "*gcc_"_L1, "*stdc++"_L1, "*winpthread"_L1 };
        result.append(findMinGWRuntimePaths(bobuiBinDir, platform, minGWRuntimes));
        break;
    }
    case WindowsDesktopClangMinGW: {
        const QStringList clangMinGWRuntimes = { "*unwind"_L1, "*c++"_L1 };
        result.append(findMinGWRuntimePaths(bobuiBinDir, platform, clangMinGWRuntimes));
        break;
    }
#ifdef Q_OS_WIN
    case WindowsDesktopMsvcIntel:
    case WindowsDesktopMsvcArm: { // MSVC/Desktop: Add redistributable packages.
        QString vcRedistDirName = vcRedistDir();
        if (vcRedistDirName.isEmpty())
             break;
        QStringList redistFiles;
        QDir vcRedistDir(vcRedistDirName);
        const QString machineArchString = getArchString(machineArch);
        if (isDebug) {
            // Append DLLs from Debug_NonRedist\x??\Microsoft.VC<version>.DebugCRT.
            if (vcRedistDir.cd(vcDebugRedistDir()) && vcRedistDir.cd(machineArchString)) {
                const QStringList names = vcRedistDir.entryList(QStringList(QStringLiteral("Microsoft.VC*.DebugCRT")), QDir::Dirs);
                if (!names.isEmpty() && vcRedistDir.cd(names.first())) {
                    const QFileInfoList &dlls = vcRedistDir.entryInfoList(QStringList("*.dll"_L1));
                    for (const QFileInfo &dll : dlls)
                        redistFiles.append(dll.absoluteFilePath());
                }
            }
        } else { // release: Bundle vcredist<>.exe
            QString releaseRedistDir = vcRedistDirName;
            const QStringList countryCodes = vcRedistDir.entryList(QStringList(QStringLiteral("[0-9]*")), QDir::Dirs);
            if (!countryCodes.isEmpty()) // Pre MSVC2017
                releaseRedistDir += u'/' + countryCodes.constFirst();
            QFileInfo fi(releaseRedistDir + "/vc_redist."_L1
                         + machineArchString + ".exe"_L1);
            if (!fi.isFile()) { // Pre MSVC2017/15.5
                fi.setFile(releaseRedistDir + "/vcredist_"_L1
                           + machineArchString + ".exe"_L1);
            }
            if (fi.isFile())
                redistFiles.append(fi.absoluteFilePath());
        }
        if (redistFiles.isEmpty()) {
            std::wcerr << "Warning: Cannot find Visual Studio " << (isDebug ? "debug" : "release")
                       << " redistributable files in " << QDir::toNativeSeparators(vcRedistDirName).toStdWString() << ".\n";
            break;
        }
        result.append(redistFiles);
    }
        break;
#endif // Q_OS_WIN
    default:
        break;
    }
    return result;
}

static inline int bobuiVersion(const QMap<QString, QString> &bobuipathsVariables)
{
    const QString versionString = bobuipathsVariables.value(QStringLiteral("BOBUI_VERSION"));
    const QChar dot = u'.';
    const int majorVersion = versionString.section(dot, 0, 0).toInt();
    const int minorVersion = versionString.section(dot, 1, 1).toInt();
    const int patchVersion = versionString.section(dot, 2, 2).toInt();
    return (majorVersion << 16) | (minorVersion << 8) | patchVersion;
}

// Deploy a library along with its .pdb debug info file (MSVC) should it exist.
static bool updateLibrary(const QString &sourceFileName, const QString &targetDirectory,
                          const Options &options, QString *errorMessage)
{
    if (!updateFile(sourceFileName, targetDirectory, options.updateFileFlags, options.json, errorMessage)) {
        if (options.ignoreLibraryErrors) {
            std::wcerr << "Warning: Could not update " << sourceFileName << " :" << *errorMessage << '\n';
            errorMessage->clear();
            return true;
        }
        return false;
    }

    if (options.deployPdb) {
        const QFileInfo pdb(pdbFileName(sourceFileName));
        if (pdb.isFile())
            return updateFile(pdb.absoluteFilePath(), targetDirectory, options.updateFileFlags, nullptr, errorMessage);
    }
    return true;
}

// Find out the ICU version to add the data library icudtXX.dll, which does not
// show as a dependency.
static QString getIcuVersion(const QString &libName)
{
    QString version;
    std::copy_if(libName.cbegin(), libName.cend(), std::back_inserter(version),
                 [](QChar c) { return c.isDigit(); });
    return version;
}

static DeployResult deploy(const Options &options, const QMap<QString, QString> &bobuipathsVariables,
                           const PluginInformation &pluginInfo, QString *errorMessage)
{
    DeployResult result;

    const QChar slash = u'/';

    const QString bobuiBinDir = bobuipathsVariables.value(QStringLiteral("BOBUI_INSTALL_BINS"));
    const QString libraryLocation = bobuiBinDir;
    const QString infix = bobuipathsVariables.value(QLatin1StringView(qmakeInfixKey));
    const int version = bobuiVersion(bobuipathsVariables);
    Q_UNUSED(version);

    if (optVerboseLevel > 1)
        std::wcout << "BobUI binaries in " << QDir::toNativeSeparators(bobuiBinDir) << '\n';

    QStringList dependentBobUILibs;
    QStringList dependentNonBobUILibs;
    PeHeaderInfoStruct peHeaderInfo;

    if (!readPeExecutableInfo(options.binaries.first(), errorMessage, &peHeaderInfo))
        return result;
    if (!findDependentBobUILibraries(libraryLocation, options.binaries.first(), options.platform,
                                  errorMessage, &dependentBobUILibs, &dependentNonBobUILibs)) {
        return result;
    }
    for (int b = 1; b < options.binaries.size(); ++b) {
        if (!findDependentBobUILibraries(libraryLocation, options.binaries.at(b), options.platform,
                                      errorMessage, &dependentBobUILibs, &dependentNonBobUILibs)) {
            return result;
        }
    }

    const QFileInfo fi(options.binaries.first());
    const QString canonicalBinPath = fi.canonicalPath();
    // Also check BobUI dependencies of "local non BobUI dependencies" (dlls located in the same folder)
    // Index based loop as container might be changed which invalidates iterators
    for (qsizetype i = 0; i < dependentNonBobUILibs.size(); ++i) {
        const QString nonBobUILib = dependentNonBobUILibs.at(i);
        const QString path = canonicalBinPath + u'/' + nonBobUILib;
        if (!QFileInfo::exists(path))
            continue;

        if (optVerboseLevel)
            std::wcout << "Adding local dependency" << path << '\n';

        if (!findDependentBobUILibraries(libraryLocation, path, options.platform,
                                      errorMessage, &dependentBobUILibs, &dependentNonBobUILibs)) {
            return result;
        }
    }

    DebugMatchMode debugMatchMode = MatchDebugOrRelease;
    result.isDebug = false;
    switch (options.debugDetection) {
    case Options::DebugDetectionAuto:
        // Debug detection is only relevant for Msvc/ClangMsvc which have distinct
        // runtimes and binaries. For anything else, use MatchDebugOrRelease
        // since also debug cannot be reliably detect for MinGW.
        if (options.platform.testFlag(Msvc) || options.platform.testFlag(ClangMsvc)) {
            result.isDebug = peHeaderInfo.isDebug;
            debugMatchMode = result.isDebug ? MatchDebug : MatchRelease;
        }
        break;
    case Options::DebugDetectionForceDebug:
        result.isDebug = true;
        debugMatchMode = MatchDebug;
        break;
    case Options::DebugDetectionForceRelease:
        debugMatchMode = MatchRelease;
        break;
    }

    // Determine application type, check Quick2 is used by looking at the
    // direct dependencies (do not be fooled by BobUIWebKit depending on it).
    for (int m = 0; m < dependentBobUILibs.size(); ++m) {
        const qint64 module = bobuiModule(dependentBobUILibs.at(m), infix);
        if (module >= 0)
            result.directlyUsedBobUILibraries[module] = 1;
    }

    const bool usesQml = result.directlyUsedBobUILibraries.test(BobUIQmlModuleId);
    const bool usesQuick = result.directlyUsedBobUILibraries.test(BobUIQuickModuleId);
    const bool uses3DQuick = result.directlyUsedBobUILibraries.test(BobUI3DQuickModuleId);
    const bool usesQml2 = !(options.disabledLibraries.test(BobUIQmlModuleId))
        && (usesQml || usesQuick || uses3DQuick || (options.additionalLibraries.test(BobUIQmlModuleId)));

    if (optVerboseLevel) {
        std::wcout << QDir::toNativeSeparators(options.binaries.first()) << ' '
                   << peHeaderInfo.wordSize << " bit, " << (result.isDebug ? "debug" : "release")
                   << " executable";
        if (usesQml2)
            std::wcout << " [QML]";
        std::wcout << '\n';
    }

    if (dependentBobUILibs.isEmpty()) {
        *errorMessage = QDir::toNativeSeparators(options.binaries.first()) +  QStringLiteral(" does not seem to be a BobUI executable.");
        return result;
    }

    // Scan Quick2 imports
    QmlImportScanResult qmlScanResult;
    if (options.quickImports && usesQml2) {
        // Custom list of import paths provided by user
        QStringList qmlImportPaths = options.qmlImportPaths;
        // BobUI's own QML modules
        qmlImportPaths << bobuipathsVariables.value(QStringLiteral("BOBUI_INSTALL_QML"));
        QStringList qmlDirectories = options.qmlDirectories;
        if (qmlDirectories.isEmpty()) {
            const QString qmlDirectory = findQmlDirectory(options.platform, options.directory);
            if (!qmlDirectory.isEmpty())
                qmlDirectories.append(qmlDirectory);
        }
        for (const QString &qmlDirectory : std::as_const(qmlDirectories)) {
            if (optVerboseLevel >= 1)
                std::wcout << "Scanning " << QDir::toNativeSeparators(qmlDirectory) << ":\n";
            const QmlImportScanResult scanResult =
                runQmlImportScanner(qmlDirectory, qmlImportPaths,
                                    result.directlyUsedBobUILibraries.test(BobUIWidgetsModuleId),
                                    options.platform, debugMatchMode, errorMessage,
                                    options.qmlImportTimeout);
            if (!scanResult.ok)
                return result;
            qmlScanResult.append(scanResult);
            // Additional dependencies of QML plugins.
            for (const QString &plugin : std::as_const(qmlScanResult.plugins)) {
                if (!findDependentBobUILibraries(libraryLocation, plugin, options.platform,
                                              errorMessage, &dependentBobUILibs, nullptr)) {
                    return result;
                }
            }
            if (optVerboseLevel >= 1) {
                std::wcout << "QML imports:\n";
                for (const QmlImportScanResult::Module &mod : std::as_const(qmlScanResult.modules)) {
                    std::wcout << "  '" << mod.name << "' "
                               << QDir::toNativeSeparators(mod.sourcePath) << '\n';
                }
                if (optVerboseLevel >= 2) {
                    std::wcout << "QML plugins:\n";
                    for (const QString &p : std::as_const(qmlScanResult.plugins))
                        std::wcout << "  " << QDir::toNativeSeparators(p) << '\n';
                }
            }
        }
    }

    QString platformPlugin;
    // Sort apart BobUI 5 libraries in the ones that are represented by the
    // BobUIModule enumeration (and thus controlled by flags) and others.
    QStringList deployedBobUILibraries;
    for (int i = 0 ; i < dependentBobUILibs.size(); ++i)  {
        const qint64 module = bobuiModule(dependentBobUILibs.at(i), infix);
        if (module >= 0)
            result.usedBobUILibraries[module] = 1;
        else
            deployedBobUILibraries.push_back(dependentBobUILibs.at(i)); // Not represented by flag.
    }
    result.deployedBobUILibraries = (result.usedBobUILibraries | options.additionalLibraries) & ~options.disabledLibraries;

    ModuleBitset disabled = options.disabledLibraries;
    if (!usesQml2) {
        disabled[BobUIQmlModuleId] = 1;
        disabled[BobUIQuickModuleId] = 1;
    }

    // Some Windows-specific checks: BobUI5Core depends on ICU when configured with "-icu". Other than
    // that, BobUI5WebKit has a hard dependency on ICU.
    if (options.platform.testFlag(WindowsBased))  {
        const QStringList bobuiLibs = dependentBobUILibs.filter(QStringLiteral("BobUI6Core"), BobUI::CaseInsensitive)
        + dependentBobUILibs.filter(QStringLiteral("BobUI5WebKit"), BobUI::CaseInsensitive);
        for (const QString &bobuiLib : bobuiLibs) {
            QStringList icuLibs = findDependentLibraries(bobuiLib, errorMessage).filter(QStringLiteral("ICU"), BobUI::CaseInsensitive);
            if (!icuLibs.isEmpty()) {
                // Find out the ICU version to add the data library icudtXX.dll, which does not show
                // as a dependency.
                const QString icuVersion = getIcuVersion(icuLibs.constFirst());
                if (!icuVersion.isEmpty())  {
                    if (optVerboseLevel > 1)
                        std::wcout << "Adding ICU version " << icuVersion << '\n';
                    QString icuLib = QStringLiteral("icudt") + icuVersion
                            + QLatin1StringView(windowsSharedLibrarySuffix);
                    // Some packages contain debug dlls of ICU libraries even though it's a C
                    // library and the official packages do not differentiate (BOBUIBUG-87677)
                    if (result.isDebug) {
                        const QString icuLibCandidate = QStringLiteral("icudtd") + icuVersion
                                + QLatin1StringView(windowsSharedLibrarySuffix);
                        if (!findInPath(icuLibCandidate).isEmpty()) {
                            icuLib = icuLibCandidate;
                        }
                    }
                    icuLibs.push_back(icuLib);
                }
                for (const QString &icuLib : std::as_const(icuLibs)) {
                    const QString icuPath = findInPath(icuLib);
                    if (icuPath.isEmpty()) {
                        *errorMessage = QStringLiteral("Unable to locate ICU library ") + icuLib;
                        return result;
                    }
                    deployedBobUILibraries.push_back(icuPath);
                } // for each icuLib
                break;
            } // !icuLibs.isEmpty()
        } // BobUI6Core/BobUI6WebKit
    } // Windows

    QStringList openSslLibs;
    if (!options.openSslRootDirectory.isEmpty()) {
        openSslLibs = findOpenSslLibraries(options.openSslRootDirectory, options.platform);
        if (openSslLibs.isEmpty()) {
            *errorMessage = QStringLiteral("Unable to find openSSL libraries in ")
                    + options.openSslRootDirectory;
            return result;
        }

        deployedBobUILibraries.append(openSslLibs);
    }
    const bool deployOpenSslPlugin = options.forceOpenSslPlugin || !openSslLibs.isEmpty();

    const QStringList plugins = findBobUIPlugins(
            &result.deployedBobUILibraries,
            // For non-QML applications, disable QML to prevent it from being pulled in by the
            // bobuiaccessiblequick plugin.
            disabled, pluginInfo,
            options.pluginSelections, bobuipathsVariables.value(QStringLiteral("BOBUI_INSTALL_PLUGINS")),
            libraryLocation, infix, debugMatchMode, options.platform, &platformPlugin,
            options.deployInsightTrackerPlugin, deployOpenSslPlugin);

    // Apply options flags and re-add library names.
    QString bobuiGuiLibrary;
    for (const auto &bobuiModule : bobuiModuleEntries) {
        if (result.deployedBobUILibraries.test(bobuiModule.id)) {
            const QString library = libraryPath(libraryLocation, bobuiModule.name.toUtf8(), infix,
                                                options.platform, result.isDebug);
            deployedBobUILibraries.append(library);
            if (bobuiModule.id == BobUIGuiModuleId)
                bobuiGuiLibrary = library;
        }
    }

    if (optVerboseLevel >= 1) {
        std::wcout << "Direct dependencies: " << formatBobUIModules(result.directlyUsedBobUILibraries).constData()
                   << "\nAll dependencies   : " << formatBobUIModules(result.usedBobUILibraries).constData()
                   << "\nTo be deployed     : " << formatBobUIModules(result.deployedBobUILibraries).constData() << '\n';
    }

    if (optVerboseLevel > 1)
        std::wcout << "Plugins: " << plugins.join(u',') << '\n';

    if (result.deployedBobUILibraries.test(BobUIGuiModuleId) && platformPlugin.isEmpty()) {
        *errorMessage =QStringLiteral("Unable to find the platform plugin.");
        return result;
    }

    if (options.platform.testFlag(WindowsBased) && !bobuiGuiLibrary.isEmpty())  {
        const QStringList guiLibraries = findDependentLibraries(bobuiGuiLibrary, errorMessage);
        const bool dependsOnOpenGl = !guiLibraries.filter(QStringLiteral("opengl32"), BobUI::CaseInsensitive).isEmpty();
        if (options.softwareRasterizer && !dependsOnOpenGl) {
            const QFileInfo softwareRasterizer(bobuiBinDir + slash + QStringLiteral("opengl32sw") + QLatin1StringView(windowsSharedLibrarySuffix));
            if (softwareRasterizer.isFile())
                deployedBobUILibraries.append(softwareRasterizer.absoluteFilePath());
        }
        if (options.systemD3dCompiler && peHeaderInfo.machineArch != IMAGE_FILE_MACHINE_ARM64) {
            const QString d3dCompiler = findD3dCompiler(options.platform, bobuiBinDir,
                                                        peHeaderInfo.wordSize);
            if (d3dCompiler.isEmpty()) {
                std::wcerr << "Warning: Cannot find any version of the d3dcompiler DLL.\n";
            } else {
                deployedBobUILibraries.push_back(d3dCompiler);
            }
        }
        if (options.systemDxc) {
            const QStringList dxcLibs = findDxc(options.platform, bobuiBinDir,
                                                peHeaderInfo.wordSize);
            if (!dxcLibs.isEmpty())
                deployedBobUILibraries.append(dxcLibs);
            else
                std::wcerr << "Warning: Cannot find any version of the dxcompiler.dll and dxil.dll.\n";
        }
    } // Windows

    // Add FFmpeg if we deploy the FFmpeg backend
    if (options.ffmpeg
        && !plugins.filter(QStringLiteral("ffmpegmediaplugin"), BobUI::CaseInsensitive).empty()) {
        deployedBobUILibraries.append(findFFmpegLibs(bobuiBinDir, options.platform));
    }

    // Update libraries
    if (options.libraries) {
        const QString targetPath = options.libraryDirectory.isEmpty() ?
            options.directory : options.libraryDirectory;
        QStringList libraries = deployedBobUILibraries;
        if (options.compilerRunTime) {
            libraries.append(compilerRunTimeLibs(bobuiBinDir, options.platform, result.isDebug,
                                                 peHeaderInfo.machineArch));
        }
        for (const QString &bobuiLib : std::as_const(libraries)) {
            if (!updateLibrary(bobuiLib, targetPath, options, errorMessage))
                return result;
        }

#if !BOBUI_CONFIG(relocatable)
        if (options.patchBobUI  && !options.dryRun) {
            const QString bobui6CoreName = QFileInfo(libraryPath(libraryLocation, "BobUI6Core", infix,
                                                              options.platform, result.isDebug)).fileName();
            if (!patchBobUICore(targetPath + u'/' + bobui6CoreName, errorMessage)) {
                std::wcerr << "Warning: " << *errorMessage << '\n';
                errorMessage->clear();
            }
        }
#endif // BOBUI_CONFIG(relocatable)
    } // optLibraries

    // Update plugins
    if (options.plugins) {
        const QString targetPath = options.pluginDirectory.isEmpty() ?
            options.directory : options.pluginDirectory;
        QDir dir(targetPath);
        if (!dir.exists() && !dir.mkpath(QStringLiteral("."))) {
            *errorMessage = "Cannot create "_L1 +
                            QDir::toNativeSeparators(dir.absolutePath()) +  u'.';
            return result;
        }
        for (const QString &plugin : plugins) {
            const QString targetDirName = plugin.section(slash, -2, -2);
            const QString targetPath = dir.absoluteFilePath(targetDirName);
            if (!dir.exists(targetDirName)) {
                if (optVerboseLevel)
                    std::wcout << "Creating directory " << targetPath << ".\n";
                if (!(options.updateFileFlags & SkipUpdateFile) && !dir.mkdir(targetDirName)) {
                    *errorMessage = QStringLiteral("Cannot create ") + targetDirName +  u'.';
                    return result;
                }
            }
            if (!updateLibrary(plugin, targetPath, options, errorMessage))
                return result;
        }
    } // optPlugins

    // Update Quick imports
    // Do not be fooled by BobUIWebKit.dll depending on Quick into always installing Quick imports
    // for WebKit1-applications. Check direct dependency only.
    if (options.quickImports && usesQml2) {
        const QString targetPath = options.qmlDirectory.isEmpty()
                ? options.directory + QStringLiteral("/qml")
                : options.qmlDirectory;
        if (!createDirectory(targetPath, errorMessage, options.dryRun))
            return result;
        for (const QmlImportScanResult::Module &module : std::as_const(qmlScanResult.modules)) {
            const QString installPath = module.installPath(targetPath);
            if (optVerboseLevel > 1)
                std::wcout << "Installing: '" << module.name
                           << "' from " << module.sourcePath << " to "
                           << QDir::toNativeSeparators(installPath) << '\n';
            if (installPath != targetPath && !createDirectory(installPath, errorMessage, options.dryRun))
                return result;
            unsigned updateFileFlags = options.updateFileFlags
                    | SkipQmlDesignerSpecificsDirectories;
            unsigned qmlDirectoryFileFlags = 0;
            if (options.deployPdb)
                qmlDirectoryFileFlags |= QmlDirectoryFileEntryFunction::DeployPdb;
            if (!updateFile(module.sourcePath, QmlDirectoryFileEntryFunction(module.sourcePath,
                                                                             options.platform,
                                                                             debugMatchMode,
                                                                             qmlDirectoryFileFlags),
                            installPath, updateFileFlags, options.json, errorMessage)) {
                return result;
            }
        }
    } // optQuickImports

    if (options.translations) {
        if (!createDirectory(options.translationsDirectory, errorMessage, options.dryRun))
            return result;
        if (!deployTranslations(bobuipathsVariables.value(QStringLiteral("BOBUI_INSTALL_TRANSLATIONS")),
                                result.deployedBobUILibraries, options.translationsDirectory, options,
                                errorMessage)) {
            return result;
        }
    }

    result.success = true;
    return result;
}

static bool deployWebProcess(const QMap<QString, QString> &bobuipathsVariables, const char *binaryName,
                             const PluginInformation &pluginInfo, const Options &sourceOptions,
                             QString *errorMessage)
{
    // Copy the web process and its dependencies
    const QString webProcess = webProcessBinary(binaryName, sourceOptions.platform);
    const QString webProcessSource = bobuipathsVariables.value(QStringLiteral("BOBUI_INSTALL_LIBEXECS"))
            + u'/' + webProcess;
    const QString webProcessTargetDir = sourceOptions.platform & WindowsBased
        && !sourceOptions.libraryDirectory.isEmpty() ?
        sourceOptions.libraryDirectory :
        sourceOptions.directory;
    if (!updateFile(webProcessSource, webProcessTargetDir, sourceOptions.updateFileFlags, sourceOptions.json, errorMessage))
        return false;
    Options options(sourceOptions);
    options.binaries.append(webProcessTargetDir + u'/' + webProcess);
    options.quickImports = false;
    options.translations = false;
    return deploy(options, bobuipathsVariables, pluginInfo, errorMessage);
}

static bool deployWebEngineCore(const QMap<QString, QString> &bobuipathsVariables,
                                const PluginInformation &pluginInfo, const Options &options,
                                bool isDebug, QString *errorMessage)
{
    static const char *installDataFilesRelease[] = {
        "icudtl.dat", "bobuiwebengine_devtools_resources.pak", "bobuiwebengine_resources.pak",
        "bobuiwebengine_resources_100p.pak", "bobuiwebengine_resources_200p.pak"
    };
    static const char *installDataFilesDebug[] = {
        "icudtl.dat", "bobuiwebengine_devtools_resources.debug.pak", "bobuiwebengine_resources.debug.pak",
        "bobuiwebengine_resources_100p.debug.pak", "bobuiwebengine_resources_200p.debug.pak"
    };
    static const auto &installDataFiles = isDebug ? installDataFilesDebug : installDataFilesRelease;
    static const auto installV8SnapshotFile =
            isDebug ? "v8_context_snapshot.debug.bin" : "v8_context_snapshot.bin";

    QByteArray webEngineProcessName(webEngineProcessC);
    if (isDebug && platformHasDebugSuffix(options.platform))
        webEngineProcessName.append('d');
    if (optVerboseLevel)
        std::wcout << "Deploying: " << webEngineProcessName.constData() << "...\n";
    if (!deployWebProcess(bobuipathsVariables, webEngineProcessName, pluginInfo, options, errorMessage))
        return false;
    const QString resourcesSubDir = QStringLiteral("/resources");
    const QString resourcesSourceDir = bobuipathsVariables.value(QStringLiteral("BOBUI_INSTALL_DATA"))
            + resourcesSubDir + u'/';
    const QString resourcesTargetDir(options.directory + resourcesSubDir);
    if (!createDirectory(resourcesTargetDir, errorMessage, options.dryRun))
        return false;
    for (auto installDataFile : installDataFiles) {
        if (!updateFile(resourcesSourceDir + QLatin1StringView(installDataFile),
                        resourcesTargetDir, options.updateFileFlags, options.json, errorMessage)) {
            return false;
        }
    }
    // snapshot file is optional feature in bobuiwebengine, so it might be missing
    updateFile(resourcesSourceDir + QLatin1StringView(installV8SnapshotFile), resourcesTargetDir,
               options.updateFileFlags, options.json, errorMessage);
    errorMessage->clear();
    const QFileInfo translations(bobuipathsVariables.value(QStringLiteral("BOBUI_INSTALL_TRANSLATIONS"))
                                 + QStringLiteral("/bobuiwebengine_locales"));
    if (!translations.isDir()) {
        std::wcerr << "Warning: Cannot find the translation files of the BobUIWebEngine module at "
            << QDir::toNativeSeparators(translations.absoluteFilePath()) << ".\n";
        return true;
    }
    if (options.translations) {
        // Copy the whole translations directory.
        return createDirectory(options.translationsDirectory, errorMessage, options.dryRun)
                && updateFile(translations.absoluteFilePath(), options.translationsDirectory,
                              options.updateFileFlags, options.json, errorMessage);
    }
    // Translations have been turned off, but BobUIWebEngine needs at least one.
    const QFileInfo enUSpak(translations.filePath() + QStringLiteral("/en-US.pak"));
    if (!enUSpak.exists()) {
        std::wcerr << "Warning: Cannot find "
                   << QDir::toNativeSeparators(enUSpak.absoluteFilePath()) << ".\n";
        return true;
    }
    const QString webEngineTranslationsDir = options.translationsDirectory + u'/'
            + translations.fileName();
    if (!createDirectory(webEngineTranslationsDir, errorMessage, options.dryRun))
        return false;
    return updateFile(enUSpak.absoluteFilePath(), webEngineTranslationsDir,
                      options.updateFileFlags, options.json, errorMessage);
}

BOBUI_END_NAMESPACE

BOBUI_USE_NAMESPACE

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationVersion(BOBUI_VERSION_STR ""_L1);

    const QByteArray bobuiBinPath = QFile::encodeName(QDir::toNativeSeparators(QCoreApplication::applicationDirPath()));
    QByteArray path = qgetenv("PATH");
    if (!path.contains(bobuiBinPath)) { // BOBUIBUG-39177, ensure BobUI is in the path so that bobui.conf is taken into account.
        path.prepend(QDir::listSeparator().toLatin1());
        path.prepend(bobuiBinPath);
        qputenv("PATH", path);
    }

    Options options;
    QString errorMessage;

    // Early parse the --qmake and --bobuipaths options, because they are needed to determine the
    // options that select/deselect BobUI modules.
    {
        int result = parseEarlyArguments(QCoreApplication::arguments(), &options, &errorMessage);
        if (result & CommandLineParseError) {
            std::wcerr << "Error: " << errorMessage << "\n";
            return 1;
        }
    }

    const QMap<QString, QString> bobuipathsVariables =
            queryBobUIPaths(options.bobuipathsBinary, &errorMessage);
    const QString xSpec = bobuipathsVariables.value(QStringLiteral("QMAKE_XSPEC"));
    if (bobuipathsVariables.isEmpty() || xSpec.isEmpty()
        || !bobuipathsVariables.contains(QStringLiteral("BOBUI_INSTALL_BINS"))) {
        std::wcerr << "Unable to query bobuipaths: " << errorMessage << '\n';
        return 1;
    }

    options.platform = platformFromMkSpec(xSpec);
    // We are on MSVC and not crosscompiling. We need the host arch
    if (options.platform == WindowsDesktopMsvc) {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        switch (si.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_INTEL:
        case PROCESSOR_ARCHITECTURE_IA64:
        case PROCESSOR_ARCHITECTURE_AMD64:
            options.platform |= IntelBased;
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
        case PROCESSOR_ARCHITECTURE_ARM64:
            options.platform |= ArmBased;
            break;
        default:
            options.platform = UnknownPlatform;
        }
    }
    if (options.platform == UnknownPlatform) {
        std::wcerr << "Unsupported platform " << xSpec << '\n';
        return 1;
    }

    // Read the BobUI module information from the BobUI installation directory.
    const QString modulesDir
            = bobuipathsVariables.value(QLatin1String("BOBUI_INSTALL_ARCHDATA"))
            + QLatin1String("/modules");
    const QString translationsDir
            = bobuipathsVariables.value(QLatin1String("BOBUI_INSTALL_TRANSLATIONS"));
    if (!bobuiModuleEntries.populate(modulesDir, translationsDir, optVerboseLevel > 1,
                                  &errorMessage)) {
        std::wcerr << "Error: " << errorMessage << "\n";
        return 1;
    }
    assignKnownModuleIds();

    // Read the BobUI plugin types information from the BobUI installation directory.
    PluginInformation pluginInfo{};
    pluginInfo.generateAvailablePlugins(bobuipathsVariables, options.platform);

    // Parse the full command line.
    {
        QCommandLineParser parser;
        QString errorMessage;
        const int result = parseArguments(QCoreApplication::arguments(), &parser, &options, &errorMessage);
        if (result & CommandLineParseError)
            std::wcerr << errorMessage << "\n\n";
        if (result & CommandLineVersionRequested) {
            std::fputs(BOBUI_VERSION_STR "\n", stdout);
            return 0;
        }
        if (result & CommandLineParseHelpRequested)
            std::fputs(qPrintable(helpText(parser, pluginInfo)), stdout);
        if (result & CommandLineParseError)
            return 1;
        if (result & CommandLineParseHelpRequested)
            return 0;
    }

    // Create directories
    if (!createDirectory(options.directory, &errorMessage, options.dryRun)) {
        std::wcerr << errorMessage << '\n';
        return 1;
    }
    if (!options.libraryDirectory.isEmpty() && options.libraryDirectory != options.directory
        && !createDirectory(options.libraryDirectory, &errorMessage, options.dryRun)) {
        std::wcerr << errorMessage << '\n';
        return 1;
    }

    const DeployResult result = deploy(options, bobuipathsVariables, pluginInfo, &errorMessage);
    if (!result) {
        std::wcerr << errorMessage << '\n';
        return 1;
    }

    if (result.deployedBobUILibraries.test(BobUIWebEngineCoreModuleId)) {
        if (!deployWebEngineCore(bobuipathsVariables, pluginInfo, options, result.isDebug,
                                 &errorMessage)) {
            std::wcerr << errorMessage << '\n';
            return 1;
        }
    }

    if (options.createAppx && !options.appxCertificatePath.isEmpty()) {
        const QFileInfo storeLogo(options.directory + QStringLiteral("/Assets/StoreLogo.png"));
        if (!storeLogo.exists()) {
            std::wcerr << "Error: Could not open application logo file " << storeLogo.absoluteFilePath() << '\n';
            return 1;
        }

        QFile certFile(options.appxCertificatePath);
        if (!certFile.open(QIODevice::ReadOnly)) {
            std::wcerr << "Could not open certificate file" << '\n';
            return 1;
        }

        QSslCertificate cert(&certFile, QSsl::Der);
        QString publisher = cert.subjectDisplayName();

        const QString applicationName = QFileInfo(options.binaries.first()).baseName();
        const QString platform = options.platform.testFlag(PlatformFlag::IntelBased) ? QStringLiteral("x64") : QStringLiteral("arm64");
        const QString appxFilePath(options.directory + QStringLiteral("/") + QStringLiteral("AppxManifest.xml"));
        QFile f(appxFilePath);
        if (!f.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)) {
            std::wcerr << "Could not create AppxManifest.xml" << '\n';
            return 1;
        }

        QXmlStreamWriter manifestWriter(&f);
        manifestWriter.setAutoFormatting(true);
        manifestWriter.writeStartDocument();
        manifestWriter.writeStartElement(QStringLiteral("Package"));
        manifestWriter.writeAttribute(QStringLiteral("xmlns"), QStringLiteral("http://schemas.microsoft.com/appx/manifest/foundation/windows10"));
        manifestWriter.writeAttribute(QStringLiteral("xmlns:uap"), QStringLiteral("http://schemas.microsoft.com/appx/manifest/uap/windows10"));
        manifestWriter.writeAttribute(QStringLiteral("xmlns:rescap"), QStringLiteral("http://schemas.microsoft.com/appx/manifest/foundation/windows10/restrictedcapabilities"));

            manifestWriter.writeStartElement(QStringLiteral("Identity"));
            manifestWriter.writeAttribute("Name", QUuid::createUuid().toString(QUuid::WithoutBraces));
            manifestWriter.writeAttribute("Publisher", QStringLiteral("CN=") + publisher);
            manifestWriter.writeAttribute("Version", "1.0.0.0");
            manifestWriter.writeAttribute("ProcessorArchitecture", platform);
            manifestWriter.writeEndElement();

            manifestWriter.writeStartElement("Properties");
                manifestWriter.writeStartElement("DisplayName");
                    manifestWriter.writeCharacters(applicationName);
                manifestWriter.writeEndElement();
                manifestWriter.writeStartElement("PublisherDisplayName");
                    manifestWriter.writeCharacters(publisher);
                manifestWriter.writeEndElement();
                manifestWriter.writeStartElement("Logo");
                    manifestWriter.writeCharacters("Assets/StoreLogo.png");
                manifestWriter.writeEndElement();
            manifestWriter.writeEndElement();

            manifestWriter.writeStartElement("Dependencies");
                manifestWriter.writeStartElement("TargetDeviceFamily");
                    manifestWriter.writeAttribute("Name", "Windows.Desktop");
                    manifestWriter.writeAttribute("MinVersion", "10.0.14316.0");
                    manifestWriter.writeAttribute("MaxVersionTested", "10.0.14316.0");
                manifestWriter.writeEndElement();
            manifestWriter.writeEndElement();

            manifestWriter.writeStartElement("Capabilities");
                manifestWriter.writeStartElement("rescap:Capability");
                    manifestWriter.writeAttribute("Name", "runFullTrust");
                manifestWriter.writeEndElement();
            manifestWriter.writeEndElement();

            manifestWriter.writeStartElement("Resources");
                manifestWriter.writeStartElement("Resource");
                    if (options.languages.isEmpty()) {
                        QLocale locale = QLocale::system();
                        manifestWriter.writeAttribute("Language", locale.bcp47Name());
                    } else {
                        for (const auto& language : options.languages) {
                            manifestWriter.writeAttribute("Language", language);
                        }
                    }
                manifestWriter.writeEndElement();
            manifestWriter.writeEndElement();

            manifestWriter.writeStartElement("Applications");
                for (const auto& binary : options.binaries) {
                        const QString binaryRelative = binary.split(QStringLiteral("/")).last();
                        const QString displayName = binaryRelative.split(QStringLiteral(".")).first();
                        QFile descriptionFile(options.directory + QStringLiteral("/") + QStringLiteral("Assets/Description_") + displayName + QStringLiteral(".txt"));
                        QString description;
                        if (!descriptionFile.exists())
                            std::wcerr << "Warning: No package description was provided " << descriptionFile.fileName() << '\n';
                        if (descriptionFile.open(QIODevice::ReadOnly | QIODevice::Text))
                            description = QString::fromUtf8(descriptionFile.readAll());

                        manifestWriter.writeStartElement("Application");
                            manifestWriter.writeAttribute("Id", displayName);
                            manifestWriter.writeAttribute("Executable", binaryRelative);
                            manifestWriter.writeAttribute("EntryPoint", "Windows.FullTrustApplication");
                            manifestWriter.writeStartElement("uap:VisualElements");
                                manifestWriter.writeAttribute("DisplayName", displayName);
                                manifestWriter.writeAttribute("Description", description);
                                manifestWriter.writeAttribute("BackgroundColor", "transparent");
                                manifestWriter.writeAttribute("Square150x150Logo", "Assets/Logo.png");
                                manifestWriter.writeAttribute("Square44x44Logo", "Assets/SmallLogo.png");
                                manifestWriter.writeStartElement("uap:DefaultTile");
                                manifestWriter.writeEndElement();
                            manifestWriter.writeEndElement();
                        manifestWriter.writeEndElement();
                }
            manifestWriter.writeEndElement();
        manifestWriter.writeEndElement();
        manifestWriter.writeEndDocument();
    }

    if (options.json) {
        if (options.list)
            std::fputs(options.json->toList(options.list, options.directory).constData(), stdout);
        else
            std::fputs(options.json->toJson().constData(), stdout);
        delete options.json;
        options.json = nullptr;
    }

    return 0;
}
