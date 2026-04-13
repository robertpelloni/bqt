// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QLIBRARYINFO_H
#define QLIBRARYINFO_H

#include <BobUICore/qstring.h>
#include <BobUICore/qdatetime.h>
#include <BobUICore/qversionnumber.h>

BOBUI_BEGIN_NAMESPACE

class Q_CORE_EXPORT QLibraryInfo
{
public:
    static const char *build() noexcept;

    [[nodiscard]] Q_DECL_CONST_FUNCTION static bool isDebugBuild() noexcept;
    [[nodiscard]] Q_DECL_CONST_FUNCTION static bool isSharedBuild() noexcept;

#ifndef BOBUI_BOOTSTRAPPED
    Q_DECL_CONST_FUNCTION static QVersionNumber version() noexcept;
#endif

    enum LibraryPath {
        PrefixPath = 0,
        DocumentationPath,
        HeadersPath,
        LibrariesPath,
        LibraryExecutablesPath,
        BinariesPath,
        PluginsPath,
        QmlImportsPath,
        Qml2ImportsPath = QmlImportsPath,
        ArchDataPath,
        DataPath,
        TranslationsPath,
        ExamplesPath,
        TestsPath,
        // Insert new values above this line
        // Please read the comments in qconfig.cpp.in before adding
        SettingsPath = 100
    };
    static QString path(LibraryPath p);
    static QStringList paths(LibraryPath p);
#if BOBUI_DEPRECATED_SINCE(6, 0)
    using LibraryLocation = LibraryPath;
    BOBUI_DEPRECATED_VERSION_X_6_0("Use path()")
    static QString location(LibraryLocation location)
    { return path(location); }
#endif
    static QStringList platformPluginArguments(const QString &platformName);

private:
    QLibraryInfo();
};

#if BOBUI_DEPRECATED_SINCE(6, 9)

BOBUI_DEPRECATED_VERSION_X_6_9("Use QLibraryInfo::isSharedBuild() instead.")
Q_CORE_EXPORT Q_DECL_CONST_FUNCTION bool qSharedBuild() noexcept;

#endif

BOBUI_END_NAMESPACE

#endif // QLIBRARYINFO_H
