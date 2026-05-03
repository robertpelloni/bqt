
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-tool-qdbusxml2cpp")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-tool-qdbusxml2cpp" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: qdbusxml2cpp
SPDXID: SPDXRef-Package-qtbase-qt-tool-qdbusxml2cpp
PackageDownloadLocation: git://code.qt.io/qt/qtbase.git@ce5f43eaf95f13997977a321f14b4913a193cd5d
PackageVersion: 6.12.0
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
PackageLicenseDeclared: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=qdbusxml2cpp#src/tools/qdbusxml2cpp
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-qdbusxml2cpp@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=qdbusxml2cpp#src/tools/qdbusxml2cpp
PackageCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
PrimaryPackagePurpose: APPLICATION
PackageComment: <text>
CMake target name: qdbusxml2cpp
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qt:6.12.0:*:*:*:*:*:*:*
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qtbase:6.12.0:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-tool-qdbusxml2cpp}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-tool-qdbusxml2cpp}
Relationship: SPDXRef-Package-qtbase-qt-tool-qdbusxml2cpp DEPENDS_ON SPDXRef-Package-qtbase-qt-module-Core
Relationship: SPDXRef-Package-qtbase-qt-tool-qdbusxml2cpp DEPENDS_ON SPDXRef-Package-qtbase-qt-module-DBus
Relationship: SPDXRef-Package-qtbase-qt-tool-qdbusxml2cpp DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformToolInternal
Relationship: SPDXRef-Package-qtbase-qt-tool-qdbusxml2cpp DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformCommonInternal
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-tool-qdbusxml2cpp
"
        )
