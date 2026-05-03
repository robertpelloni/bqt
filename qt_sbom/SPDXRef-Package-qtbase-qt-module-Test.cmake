
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-module-Test")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-Test" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Test
SPDXID: SPDXRef-Package-qtbase-qt-module-Test
PackageDownloadLocation: git://code.qt.io/qt/qtbase.git@ce5f43eaf95f13997977a321f14b4913a193cd5d
PackageVersion: 6.12.0
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
PackageLicenseDeclared: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Test#src/testlib
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Test@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Test#src/testlib
PackageCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Test
CMake exported target name: Qt6::Test
Contained in CMake package: Qt6Test
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qt:6.12.0:*:*:*:*:*:*:*
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qtbase:6.12.0:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-module-Test}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-Test}
Relationship: SPDXRef-Package-qtbase-qt-module-Test DEPENDS_ON SPDXRef-Package-qtbase-qt-module-Core
Relationship: SPDXRef-Package-qtbase-qt-module-Test DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformModuleInternal
Relationship: SPDXRef-Package-qtbase-qt-module-Test DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-catch2
Relationship: SPDXRef-Package-qtbase-qt-module-Test DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-cycle
Relationship: SPDXRef-Package-qtbase-qt-module-Test DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-linuxperf
Relationship: SPDXRef-Package-qtbase-qt-module-Test DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-valgrind
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-module-Test
"
        )
