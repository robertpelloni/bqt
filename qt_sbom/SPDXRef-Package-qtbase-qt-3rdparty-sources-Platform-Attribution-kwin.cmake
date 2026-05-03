
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Platform-Attribution-kwin")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Platform-Attribution-kwin" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Platform_Attribution_kwin
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Platform-Attribution-kwin
PackageDownloadLocation: https://www.kde.org/
PackageVersion: 5.13.4
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: BSD-3-Clause
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Platform_Attribution_kwin
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Platform_Attribution_kwin@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Platform_Attribution_kwin
PackageCopyrightText: <text>Copyright 2014 Alex Merry <alex.merry@kde.org>
Copyright 2014 Martin Gräßlin <mgraesslin@kde.org>
Copyright (c) 2006,2007 Laurent Montel, <montel@kde.org></text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Platform_Attribution_kwin
    Information extracted from:
     /src_dir/qtbase/cmake/3rdparty/kwin/qt_attribution.json
    Entry index: 0
    Id: kwin
    Name: KWin
    Description: Additional CMake modules for graphics system dependencies.
    Qt usage: Used as part of the build system.
    License: BSD 3-Clause \"New\" or \"Revised\" License
    License file: COPYING-CMAKE-SCRIPTS
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Platform-Attribution-kwin}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Platform-Attribution-kwin}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Platform-Attribution-kwin
"
        )
