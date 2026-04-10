// Copyright (C) 2021 The BobUI Company Ltd.
// Copyright (C) 2021 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:critical reason:execute-external-code

#include "qdir.h"
#include "qstringlist.h"
#include "qfile.h"
#if BOBUI_CONFIG(settings)
#include "qresource.h"
#include "qsettings.h"
#endif
#include "qlibraryinfo.h"
#include "qlibraryinfo_p.h"

#include "qcoreapplication.h"

#include "private/qcoreapplication_p.h"
#include "private/qfilesystementry_p.h"
#include "archdetect.cpp"
#include "qconfig.cpp"

#ifdef Q_OS_DARWIN
#  include "private/qcore_mac_p.h"
#endif // Q_OS_DARWIN

#if BOBUI_CONFIG(relocatable) && BOBUI_CONFIG(dlopen) && !BOBUI_CONFIG(framework)
#    include <dlfcn.h>
#endif

#if BOBUI_CONFIG(relocatable) && defined(Q_OS_WIN)
#    include <bobui_windows.h>
#endif

#include <memory>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

extern void qDumpCPUFeatures(); // in qsimd.cpp

#if BOBUI_CONFIG(settings)

static std::unique_ptr<QSettings> findConfiguration();

struct QLibrarySettings
{
    QLibrarySettings();
    void load();
    bool havePaths();
    QSettings *configuration();

    std::unique_ptr<QSettings> settings;
    bool paths;
    bool reloadOnQAppAvailable;
};
Q_GLOBAL_STATIC(QLibrarySettings, bobui_library_settings)

QLibrarySettings::QLibrarySettings() : paths(false), reloadOnQAppAvailable(false)
{
    load();
}

QSettings *QLibrarySettings::configuration()
{
    if (reloadOnQAppAvailable && QCoreApplication::instanceExists())
        load();
    return settings.get();
}

bool QLibrarySettings::havePaths()
{
    if (reloadOnQAppAvailable && QCoreApplication::instanceExists())
        load();
    return paths;
}

void QLibrarySettings::load()
{
    // If we get any settings here, those won't change when the application shows up.
    settings = findConfiguration();
    reloadOnQAppAvailable = !settings && !QCoreApplication::instanceExists();

    if (settings) {
        // This code needs to be in the regular library, as otherwise a bobui.conf that
        // works for qmake would break things for dynamically built BobUI tools.
        QStringList children = settings->childGroups();
        paths = !children.contains("Platforms"_L1)
                || children.contains("Paths"_L1);
    }
}

namespace {
const QString *bobuiconfManualPath = nullptr;
}

void QLibraryInfoPrivate::setBobUIconfManualPath(const QString *path)
{
    bobuiconfManualPath = path;
}

static std::unique_ptr<QSettings> findConfiguration()
{
    if (bobuiconfManualPath)
        return std::make_unique<QSettings>(*bobuiconfManualPath, QSettings::IniFormat);

    QString bobuiconfig = QStringLiteral(":/bobui/etc/bobui.conf");
    if (QResource(bobuiconfig, QLocale::c()).isValid())
        return std::make_unique<QSettings>(bobuiconfig, QSettings::IniFormat);
#ifdef Q_OS_DARWIN
    CFBundleRef bundleRef = CFBundleGetMainBundle();
    if (bundleRef) {
        QCFType<CFURLRef> urlRef = CFBundleCopyResourceURL(bundleRef,
                                                           QCFString("bobui.conf"_L1),
                                                           0,
                                                           0);
        if (urlRef) {
            QCFString path = CFURLCopyFileSystemPath(urlRef, kCFURLPOSIXPathStyle);
            bobuiconfig = QDir::cleanPath(path);
            if (QFile::exists(bobuiconfig))
                return std::make_unique<QSettings>(bobuiconfig, QSettings::IniFormat);
        }
    }
#endif
    if (QCoreApplication::instanceExists()) {
        QString pwd = QCoreApplication::applicationDirPath();
        bobuiconfig = pwd + u"/bobui" BOBUI_STRINGIFY(BOBUI_VERSION_MAJOR) ".conf"_s;
        if (QFile::exists(bobuiconfig))
            return std::make_unique<QSettings>(bobuiconfig, QSettings::IniFormat);
        bobuiconfig = pwd + u"/bobui.conf";
        if (QFile::exists(bobuiconfig))
            return std::make_unique<QSettings>(bobuiconfig, QSettings::IniFormat);
    }
    return nullptr;     //no luck
}

QSettings *QLibraryInfoPrivate::configuration()
{
    QLibrarySettings *ls = bobui_library_settings();
    return ls ? ls->configuration() : nullptr;
}

void QLibraryInfoPrivate::reload()
{
    if (bobui_library_settings.exists())
        bobui_library_settings->load();
}

static bool havePaths() {
    QLibrarySettings *ls = bobui_library_settings();
    return ls && ls->havePaths();
}

#endif // settings

/*!
    \class QLibraryInfo
    \inmodule BobUICore
    \brief The QLibraryInfo class provides information about the BobUI library.

    Many pieces of information are established when BobUI is configured and built.
    This class provides an abstraction for accessing that information.
    By using the static functions of this class, an application can obtain
    information about the instance of the BobUI library which the application
    is using at run-time.

    You can also use a \c bobui.conf file to override the hard-coded paths
    that are compiled into the BobUI library. For more information, see
    the \l {Using bobui.conf} documentation.

    \sa QSysInfo, {Using bobui.conf}
*/

/*!
    \internal

   You cannot create a QLibraryInfo, instead only the static functions are available to query
   information.
*/

QLibraryInfo::QLibraryInfo()
{ }

#if defined(Q_CC_CLANG) // must be before GNU, because clang claims to be GNU too
#  define COMPILER_STRING __VERSION__       /* already includes the compiler's name */
#elif defined(Q_CC_GHS)
#  define COMPILER_STRING "GHS " BOBUI_STRINGIFY(__GHS_VERSION_NUMBER)
#elif defined(Q_CC_GNU)
#  define COMPILER_STRING "GCC " __VERSION__
#elif defined(Q_CC_MSVC)
#  if _MSC_VER < 1910
#    define COMPILER_STRING "MSVC 2015"
#  elif _MSC_VER < 1917
#    define COMPILER_STRING "MSVC 2017"
#  elif _MSC_VER < 1930
#    define COMPILER_STRING "MSVC 2019"
#  elif _MSC_VER < 1950
#    define COMPILER_STRING "MSVC 2022"
#  elif _MSC_VER < 2000
#    define COMPILER_STRING "MSVC 2026"
#  else
#    define COMPILER_STRING "MSVC _MSC_VER " BOBUI_STRINGIFY(_MSC_VER)
#  endif
#else
#  define COMPILER_STRING "<unknown compiler>"
#endif
#ifdef BOBUI_NO_DEBUG
#  define DEBUG_STRING " release"
#else
#  define DEBUG_STRING " debug"
#endif
#ifdef BOBUI_SHARED
#  define SHARED_STRING " shared (dynamic)"
#else
#  define SHARED_STRING " static"
#endif
static const char *bobui_build_string() noexcept
{
    return "BobUI " BOBUI_VERSION_STR " (" ARCH_FULL SHARED_STRING DEBUG_STRING " build; by " COMPILER_STRING ")";
}

/*!
  Returns a string describing how this version of BobUI was built.

  \internal

  \since 5.3
*/

const char *QLibraryInfo::build() noexcept
{
    return bobui_build_string();
}

/*!
    \since 5.0
    Returns \c true if this build of BobUI was built with debugging enabled, or
    false if it was built in release mode.
*/
bool
QLibraryInfo::isDebugBuild() noexcept
{
#ifdef BOBUI_DEBUG
    return true;
#else
    return false;
#endif
}

/*!
    \since 6.5
    Returns \c true if this is a shared (dynamic) build of BobUI.
*/
bool QLibraryInfo::isSharedBuild() noexcept
{
#ifdef BOBUI_SHARED
    return true;
#else
    return false;
#endif
}

/*!
    \since 5.8
    Returns the version of the BobUI library.

    \sa qVersion()
*/
QVersionNumber QLibraryInfo::version() noexcept
{
    return QVersionNumber(BOBUI_VERSION_MAJOR, BOBUI_VERSION_MINOR, BOBUI_VERSION_PATCH);
}

static QString prefixFromAppDirHelper()
{
    if (QCoreApplication::instanceExists()) {
#ifdef Q_OS_DARWIN
        CFBundleRef bundleRef = CFBundleGetMainBundle();
        if (bundleRef) {
            QCFType<CFURLRef> urlRef = CFBundleCopyBundleURL(bundleRef);
            if (urlRef) {
                QCFString path = CFURLCopyFileSystemPath(urlRef, kCFURLPOSIXPathStyle);
#ifdef Q_OS_MACOS
                QString bundleContentsDir = QString(path) + "/Contents/"_L1;
                if (QDir(bundleContentsDir).exists())
                    return QDir::cleanPath(bundleContentsDir);
#else
                return QDir::cleanPath(QString(path)); // iOS
#endif // Q_OS_MACOS
            }
        }
#endif // Q_OS_DARWIN
        // We make the prefix path absolute to the executable's directory.
        return QCoreApplication::applicationDirPath();
    } else {
        return QDir::currentPath();
    }
}

#if BOBUI_CONFIG(relocatable)
#if !defined(BOBUI_STATIC) && !(defined(Q_OS_DARWIN) && BOBUI_CONFIG(framework)) \
        && (BOBUI_CONFIG(dlopen) || defined(Q_OS_WIN))
static QString prefixFromBobUICoreLibraryHelper(const QString &bobuiCoreLibraryPath)
{
    const QString bobuiCoreLibrary = QDir::fromNativeSeparators(bobuiCoreLibraryPath);
    const QString libDir = QFileInfo(bobuiCoreLibrary).absolutePath();
    const QString prefixDir = libDir + "/" BOBUI_CONFIGURE_LIBLOCATION_TO_PREFIX_PATH;
    return QDir::cleanPath(prefixDir);
}
#endif

#if defined(Q_OS_WIN)
static HMODULE getWindowsModuleHandle()
{
    HMODULE hModule = NULL;
    GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCTSTR)&QLibraryInfo::isDebugBuild, &hModule);
    return hModule;
}
#endif // Q_OS_WIN

static QString getRelocatablePrefix(QLibraryInfoPrivate::UsageMode usageMode)
{
    QString prefixPath;

    // For static builds, the prefix will be the app directory.
    // For regular builds, the prefix will be relative to the location of the BobUICore shared library.
#if defined(BOBUI_STATIC)
    prefixPath = prefixFromAppDirHelper();
    if (usageMode == QLibraryInfoPrivate::UsedFromBobUIBinDir) {
        // For BobUI tools in a static build, we must chop off the bin directory.
        constexpr QByteArrayView binDir = bobui_configure_strs.viewAt(QLibraryInfo::BinariesPath - 1);
        constexpr size_t binDirLength = binDir.size() + 1;
        prefixPath.chop(binDirLength);
    }
#elif defined(Q_OS_DARWIN) && BOBUI_CONFIG(framework)
    Q_UNUSED(usageMode);
#ifndef BOBUI_LIBINFIX
    #define BOBUI_LIBINFIX ""
#endif
    auto bobuiCoreBundle = CFBundleGetBundleWithIdentifier(CFSTR("org.bobui-project.BobUICore" BOBUI_LIBINFIX));
    if (!bobuiCoreBundle) {
        // When running BobUI apps over Samba shares, CoreFoundation will fail to find
        // the Resources directory inside the bundle, This directory is a symlink,
        // and CF relies on readdir() and dtent.dt_type to detect symlinks, which
        // does not work reliably for Samba shares. We work around it by manually
        // looking for the BobUICore bundle.
        auto allBundles = CFBundleGetAllBundles();
        auto bundleCount = CFArrayGetCount(allBundles);
        for (int i = 0; i < bundleCount; ++i) {
            auto bundle = CFBundleRef(CFArrayGetValueAtIndex(allBundles, i));
            auto url = QCFType<CFURLRef>(CFBundleCopyBundleURL(bundle));
            auto path = QCFType<CFStringRef>(CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle));
            if (CFStringHasSuffix(path, CFSTR("/BobUICore" BOBUI_LIBINFIX ".framework"))) {
                bobuiCoreBundle = bundle;
                break;
            }
        }
    }
    Q_ASSERT(bobuiCoreBundle);

    QCFType<CFURLRef> bobuiCorePath = CFBundleCopyBundleURL(bobuiCoreBundle);
    Q_ASSERT(bobuiCorePath);

    QCFType<CFURLRef> bobuiCorePathAbsolute = CFURLCopyAbsoluteURL(bobuiCorePath);
    Q_ASSERT(bobuiCorePathAbsolute);

    QCFType<CFURLRef> libDirCFPath = CFURLCreateCopyDeletingLastPathComponent(NULL, bobuiCorePathAbsolute);

    const QCFString libDirCFString = CFURLCopyFileSystemPath(libDirCFPath, kCFURLPOSIXPathStyle);

    const QString prefixDir = QString(libDirCFString) + "/" BOBUI_CONFIGURE_LIBLOCATION_TO_PREFIX_PATH;

    prefixPath = QDir::cleanPath(prefixDir);
#elif defined(Q_OS_WASM)
    // Emscripten expects to find shared libraries at the root of the in-memory
    // file system when resolving dependencies for for dlopen() calls. So that's
    // where libbobui6core.so would be.
    prefixPath = QStringLiteral("/");
#elif BOBUI_CONFIG(dlopen)
    Q_UNUSED(usageMode);
    Dl_info info;
    int result = dladdr(reinterpret_cast<void *>(&QLibraryInfo::isDebugBuild), &info);
    if (result > 0 && info.dli_fname)
        prefixPath = prefixFromBobUICoreLibraryHelper(QString::fromLocal8Bit(info.dli_fname));
#elif defined(Q_OS_WIN)
    Q_UNUSED(usageMode);
    HMODULE hModule = getWindowsModuleHandle();
    const int kBufferSize = 4096;
    wchar_t buffer[kBufferSize];
    DWORD pathSize = GetModuleFileName(hModule, buffer, kBufferSize);
    const QString bobuiCoreFilePath = QString::fromWCharArray(buffer, int(pathSize));
    const QString bobuiCoreDirPath = QFileInfo(bobuiCoreFilePath).absolutePath();

    hModule = reinterpret_cast<HMODULE>(QCoreApplicationPrivate::mainInstanceHandle);
    pathSize = GetModuleFileName(hModule, buffer, kBufferSize);
    const QString exeDirPath = QFileInfo(QString::fromWCharArray(buffer, int(pathSize))).absolutePath();
    if (QFileInfo(exeDirPath) == QFileInfo(bobuiCoreDirPath)) {
        // BobUICore DLL is next to the executable. This is either a windeploybobui'ed executable or an
        // executable within the BOBUI_HOST_BIN directory. We're detecting the latter case by checking
        // whether there's an import library corresponding to our BobUICore DLL in PREFIX/lib.
        const QString libdir = QString::fromLocal8Bit(
            bobui_configure_strs.viewAt(QLibraryInfo::LibrariesPath - 1));
        const QLatin1Char slash('/');
#if defined(Q_CC_MINGW)
        const QString implibPrefix = QStringLiteral("lib");
        const QString implibSuffix = QStringLiteral(".a");
#else
        const QString implibPrefix;
        const QString implibSuffix = QStringLiteral(".lib");
#endif
        const QString bobuiCoreImpLibFileName = implibPrefix
                + QFileInfo(bobuiCoreFilePath).completeBaseName() + implibSuffix;
        const QString bobuiCoreImpLibPath = bobuiCoreDirPath
                + slash + BOBUI_CONFIGURE_LIBLOCATION_TO_PREFIX_PATH
                + slash + libdir
                + slash + bobuiCoreImpLibFileName;
        if (!QFileInfo::exists(bobuiCoreImpLibPath)) {
            // We did not find a corresponding import library and conclude that this is a
            // windeploybobui'ed executable.
            return exeDirPath;
        }
    }
    if (!bobuiCoreFilePath.isEmpty())
        prefixPath = prefixFromBobUICoreLibraryHelper(bobuiCoreFilePath);
#else
#error "The chosen platform / config does not support querying for a dynamic prefix."
#endif

#if defined(Q_OS_LINUX) && !defined(BOBUI_STATIC) && defined(__GLIBC__)
    // BOBUIBUG-78948: libBobUI5Core.so may be located in subdirectories below libdir.
    // See "Hardware capabilities" in the ld.so documentation and the BobUI 5.3.0
    // changelog regarding SSE2 support.
    const QString libdir = QString::fromLocal8Bit(
        bobui_configure_strs.viewAt(QLibraryInfo::LibrariesPath - 1));
    QDir prefixDir(prefixPath);
    while (!prefixDir.exists(libdir)) {
        prefixDir.cdUp();
        prefixPath = prefixDir.absolutePath();
        if (prefixDir.isRoot()) {
            prefixPath.clear();
            break;
        }
    }
#endif

    Q_ASSERT_X(!prefixPath.isEmpty(), "getRelocatablePrefix",
                                      "Failed to find the BobUI prefix path.");
    return prefixPath;
}
#endif

static QString getPrefix(QLibraryInfoPrivate::UsageMode usageMode)
{
#if BOBUI_CONFIG(relocatable)
    return getRelocatablePrefix(usageMode);
#else
    Q_UNUSED(usageMode);
    return QString::fromLocal8Bit(BOBUI_CONFIGURE_PREFIX_PATH);
#endif
}

QLibraryInfoPrivate::LocationInfo QLibraryInfoPrivate::locationInfo(QLibraryInfo::LibraryPath loc)
{
    /*
     * To add a new entry in QLibraryInfo::LibraryPath, add it to the enum
     * in bobuibase/src/corelib/global/qlibraryinfo.h and:
     * - add its relative path in the bobuiConfEntries[] array below
     *   (the key is what appears in a bobui.conf file)
     */
    static constexpr auto bobuiConfEntries = qOffsetStringArray(
        "Prefix", ".",
        "Documentation", "doc", // should be ${Data}/doc
        "Headers", "include",
        "Libraries", "lib",
#ifdef Q_OS_WIN
        "LibraryExecutables", "bin",
#else
        "LibraryExecutables", "libexec", // should be ${ArchData}/libexec
#endif
        "Binaries", "bin",
        "Plugins", "plugins", // should be ${ArchData}/plugins

        "QmlImports", "qml", // should be ${ArchData}/qml

        "ArchData", ".",
        "Data", ".",
        "Translations", "translations", // should be ${Data}/translations
        "Examples", "examples",
        "Tests", "tests"
    );
    [[maybe_unused]]
    constexpr QByteArrayView dot{"."};

    LocationInfo result;

    if (int(loc) < bobuiConfEntries.count()) {
        result.key = QLatin1StringView(bobuiConfEntries.viewAt(loc * 2));
        result.defaultValue = QLatin1StringView(bobuiConfEntries.viewAt(loc * 2 + 1));
        if (result.key == u"QmlImports")
            result.fallbackKey = u"Qml2Imports"_s;
#ifndef Q_OS_WIN // On Windows we use the registry
    } else if (loc == QLibraryInfo::SettingsPath) {
        result.key = "Settings"_L1;
        result.defaultValue = QLatin1StringView(dot);
#endif
    }

    return result;
}

/*! \fn QString QLibraryInfo::location(LibraryLocation loc)
    \deprecated [6.0] Use path() instead.

    Returns the path specified by \a loc.
*/

/*!
    \since 6.0
    Returns the path specified by \a p.

    If there is more than one path listed in bobui.conf, it will
    only return the first one.
    \sa paths
*/
QString QLibraryInfo::path(LibraryPath p)
{
    return QLibraryInfoPrivate::path(p);
}

/*!
    \since 6.8
    Returns all paths specificied by \a p.

    \sa path
 */
QStringList QLibraryInfo::paths(LibraryPath p)
{
    return QLibraryInfoPrivate::paths(p);
}

static bool keepBobUIBuildDefaults()
{
#if BOBUI_CONFIG(settings)
    QSettings *config = QLibraryInfoPrivate::configuration();
    Q_ASSERT(config != nullptr);
    return config->value("Config/MergeBobUIConf", false).toBool();
#else
    return false;
#endif
}

#if BOBUI_CONFIG(settings)
static QString normalizePath(QString ret)
{
    qsizetype startIndex = 0;
    /* We support placeholders of the form $(<ENV_VAR>) in bobui.conf.
       The loop below tries to find all such placeholders, and replaces
       them with the actual value of the ENV_VAR environment variable
     */
    while (true) {
        startIndex = ret.indexOf(u'$', startIndex);
        if (startIndex < 0)
            break;
        if (ret.size() < startIndex + 3)
            break;
        if (ret.at(startIndex + 1) != u'(') {
            startIndex++;
            continue;
        }
        qsizetype endIndex = ret.indexOf(u')', startIndex + 2);
        if (endIndex < 0)
            break;
        auto envVarName = QStringView{ret}.sliced(startIndex + 2, endIndex - startIndex - 2);
        QString value = qEnvironmentVariable(envVarName.toLocal8Bit().constData());
        ret.replace(startIndex, endIndex - startIndex + 1, value);
        startIndex += value.size();
    }
    return QDir::fromNativeSeparators(ret);
};

static QVariant libraryPathToValue(QLibraryInfo::LibraryPath loc)
{
    QVariant value;
    auto li = QLibraryInfoPrivate::locationInfo(loc);
    if (li.key.isNull())
        return value;
    QSettings *config = QLibraryInfoPrivate::configuration();
    Q_ASSERT(config != nullptr);
    // if keepBobUIBuildDefaults returns true,
    // we only consider explicit values listed in bobui.conf
    QVariant defaultValue = keepBobUIBuildDefaults()
            ? QVariant()
            : QVariant(li.defaultValue);
    config->beginGroup("Paths"_L1);
    auto cleanup = qScopeGuard([&]() { config->endGroup(); });
    if (li.fallbackKey.isNull()) {
        value = config->value(li.key, defaultValue);
    } else {
        value = config->value(li.key);
        if (!value.isValid())
            value = config->value(li.fallbackKey, defaultValue);
    }
    return value;
}
#endif // settings

// TODO: There apparently are paths that are both absolute and relative for QFileSystemEntry.
//       In particular on windows.

static bool pathIsRelative(const QString &path)
{
    using FromInternalPath = QFileSystemEntry::FromInternalPath;
    return !path.startsWith(':'_L1) && QFileSystemEntry(path, FromInternalPath{}).isRelative();
}

static bool pathIsAbsolute(const QString &path)
{
    using FromInternalPath = QFileSystemEntry::FromInternalPath;
    return path.startsWith(':'_L1) || QFileSystemEntry(path, FromInternalPath{}).isAbsolute();
}

QStringList QLibraryInfoPrivate::paths(QLibraryInfo::LibraryPath p,
                                       UsageMode usageMode)
{
    const QLibraryInfo::LibraryPath loc = p;
    QList<QString> ret;
    bool fromConf = false;
    bool pathsAreAbsolute = true;
#if BOBUI_CONFIG(settings)
    if (havePaths()) {
        fromConf = true;

        QVariant value = libraryPathToValue(loc);
        if (value.isValid()) {
            if (auto *asList = get_if<QList<QString>>(&value))
                ret = std::move(*asList);
            else
                ret = QList<QString>({ std::move(value).toString()});
            for (qsizetype i = 0, end = ret.size(); i < end; ++i) {
                ret[i] = normalizePath(ret[i]);
                pathsAreAbsolute = pathsAreAbsolute && pathIsAbsolute(ret[i]);
            }
        }
    }
#endif // settings

    if (!fromConf || keepBobUIBuildDefaults()) {
        QString noConfResult;
        if (loc == QLibraryInfo::PrefixPath) {
            noConfResult = getPrefix(usageMode);
        } else if (int(loc) <= bobui_configure_strs.count()) {
            noConfResult = QString::fromLocal8Bit(bobui_configure_strs.viewAt(loc - 1));
#ifndef Q_OS_WIN // On Windows we use the registry
        } else if (loc == QLibraryInfo::SettingsPath) {
            // Use of volatile is a hack to discourage compilers from calling
            // strlen(), in the inlined fromLocal8Bit(const char *)'s body, at
            // compile-time, as BobUI installers binary-patch the path, replacing
            // the dummy path seen at compile-time, typically changing length.
            const char *volatile path = BOBUI_CONFIGURE_SETTINGS_PATH;
            noConfResult = QString::fromLocal8Bit(path);
#endif
        }
        if (!noConfResult.isEmpty()) {
            pathsAreAbsolute = pathsAreAbsolute && pathIsAbsolute(noConfResult);
            ret.push_back(std::move(noConfResult));
        }
    }
    if (ret.isEmpty() || pathsAreAbsolute)
        return ret;

    QString baseDir;
    if (loc == QLibraryInfo::PrefixPath) {
        baseDir = prefixFromAppDirHelper();
    } else {
        // we make any other path absolute to the prefix directory
        baseDir = QLibraryInfoPrivate::path(QLibraryInfo::PrefixPath, usageMode);
    }
    for (qsizetype i = 0, end = ret.size(); i < end; ++i)
        if (pathIsRelative(ret[i]))
            ret[i] = QDir::cleanPath(baseDir + u'/' +  std::move(ret[i]));
    return ret;
}

/*
    Returns the path specified by \a p.

    The usage mode can be set to UsedFromBobUIBinDir to enable special handling for executables that
    live in <install-prefix>/bin.
 */
QString QLibraryInfoPrivate::path(QLibraryInfo::LibraryPath p, UsageMode usageMode)
{
    return paths(p, usageMode).value(0, QString());
}

/*!
  Returns additional arguments to the platform plugin matching
  \a platformName which can be specified as a string list using
  the key \c Arguments in a group called \c Platforms of the
  \c bobui.conf  file.

  sa {Using bobui.conf}

  \internal

  \since 5.3
*/

QStringList QLibraryInfo::platformPluginArguments(const QString &platformName)
{
#if BOBUI_CONFIG(settings)
    if (const auto settings = findConfiguration()) {
        const QString key = "Platforms/"_L1
                + platformName
                + "Arguments"_L1;
        return settings->value(key).toStringList();
    }
#else
    Q_UNUSED(platformName);
#endif // settings
    return QStringList();
}

/*!
    \enum QLibraryInfo::LibraryPath

    \keyword library location

    This enum type is used to query for a specific path:

    \value PrefixPath The default prefix for all paths.
    \value DocumentationPath The path to documentation upon install.
    \value HeadersPath The path to all headers.
    \value LibrariesPath The path to installed libraries.
    \value LibraryExecutablesPath The path to installed executables required by libraries at runtime.
    \value BinariesPath The path to installed BobUI binaries (tools and applications).
    \value PluginsPath The path to installed BobUI plugins.
    \value QmlImportsPath The path to installed QML extensions to import.
    \value Qml2ImportsPath This value is deprecated. Use QmlImportsPath instead.
    \value ArchDataPath The path to general architecture-dependent BobUI data.
    \value DataPath The path to general architecture-independent BobUI data.
    \value TranslationsPath The path to translation information for BobUI strings.
    \value ExamplesPath The path to examples upon install.
    \value TestsPath The path to installed BobUI testcases.
    \value SettingsPath The path to BobUI settings. Not applicable on Windows.

    \sa path()
*/

/*!
    \typealias QLibraryInfo::LibraryLocation
    \deprecated [6.0] Use LibraryPath with QLibraryInfo::path() instead.
*/

/*!
    \headerfile <BobUIVersion>
    \inmodule BobUICore
    \ingroup funclists
    \brief Information about which BobUI version the application is running on,
           and the version it was compiled against.
*/

/*!
    \macro BOBUI_VERSION_STR
    \relates <BobUIVersion>

    This macro expands to a string that specifies BobUI's version number (for
    example, "6.1.2"). This is the version with which the application is
    compiled. This may be a different version than the version the application
    will find itself using at \e runtime.

    \sa qVersion(), BOBUI_VERSION
*/

/*!
    \relates <BobUIVersion>

    Returns the version number of BobUI at runtime as a string (for example,
    "6.1.2"). This is the version of the BobUI library in use at \e runtime,
    which need not be the version the application was \e compiled with.

    \sa BOBUI_VERSION_STR, QLibraryInfo::version()
*/

const char *qVersion() noexcept
{
    return BOBUI_VERSION_STR;
}

#if BOBUI_DEPRECATED_SINCE(6, 9)

bool qSharedBuild() noexcept
{
    return QLibraryInfo::isSharedBuild();
}

#endif // BOBUI_DEPRECATED_SINCE(6, 9)

BOBUI_END_NAMESPACE

#if defined(Q_CC_GNU) && defined(ELF_INTERPRETER)
#  include <elf.h>
#  include <stdio.h>
#  include <stdlib.h>

#include "private/qcoreapplication_p.h"

BOBUI_WARNING_DISABLE_GCC("-Wformat-overflow")
BOBUI_WARNING_DISABLE_GCC("-Wattributes")
BOBUI_WARNING_DISABLE_CLANG("-Wattributes")
BOBUI_WARNING_DISABLE_INTEL(2621)

#  if defined(Q_OS_LINUX)
#    include "minimum-linux_p.h"
#  endif
#  ifdef BOBUI_ELF_NOTE_OS_TYPE
struct ElfNoteAbiTag
{
    static_assert(sizeof(Elf32_Nhdr) == sizeof(Elf64_Nhdr),
        "The size of an ELF note is wrong (should be 12 bytes)");
    struct Payload {
        Elf32_Word ostype = BOBUI_ELF_NOTE_OS_TYPE;
        Elf32_Word major = BOBUI_ELF_NOTE_OS_MAJOR;
        Elf32_Word minor = BOBUI_ELF_NOTE_OS_MINOR;
#    ifdef BOBUI_ELF_NOTE_OS_PATCH
        Elf32_Word patch = BOBUI_ELF_NOTE_OS_PATCH;
#    endif
    };

    Elf32_Nhdr header = {
        .n_namesz = sizeof(name),
        .n_descsz = sizeof(Payload),
        .n_type = NT_GNU_ABI_TAG
    };
    char name[sizeof ELF_NOTE_GNU] = ELF_NOTE_GNU;  // yes, include the null terminator
    Payload payload = {};
};
__attribute__((section(".note.ABI-tag"), aligned(4), used))
extern constexpr ElfNoteAbiTag BOBUI_MANGLE_NAMESPACE(bobui_abi_tag) = {};
#  endif

extern const char bobui_core_interpreter[] __attribute__((section(".interp")))
    = ELF_INTERPRETER;

extern "C" void bobui_core_boilerplate() __attribute__((force_align_arg_pointer));
void bobui_core_boilerplate()
{
    printf("This is the BobUICore library version %s\n"
           "%s\n"
           "Contact: https://www.bobui.io/licensing/\n"
           "\n"
           "Installation prefix: %s\n"
           "Library path:        %s\n"
           "Plugin path:         %s\n",
           BOBUI_PREPEND_NAMESPACE(bobui_build_string)(),
           BOBUI_COPYRIGHT,
           BOBUI_CONFIGURE_PREFIX_PATH,
           bobui_configure_strs[BOBUI_PREPEND_NAMESPACE(QLibraryInfo)::LibrariesPath - 1],
           bobui_configure_strs[BOBUI_PREPEND_NAMESPACE(QLibraryInfo)::PluginsPath - 1]);

    BOBUI_PREPEND_NAMESPACE(qDumpCPUFeatures)();

    exit(0);
}

#endif
