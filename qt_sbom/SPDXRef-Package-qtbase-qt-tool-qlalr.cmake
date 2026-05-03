
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-tool-qlalr")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-tool-qlalr" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: qlalr
SPDXID: SPDXRef-Package-qtbase-qt-tool-qlalr
PackageDownloadLocation: git://code.qt.io/qt/qtbase.git@ce5f43eaf95f13997977a321f14b4913a193cd5d
PackageVersion: 6.12.0
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
PackageLicenseDeclared: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=qlalr#src/tools/qlalr
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-qlalr@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=qlalr#src/tools/qlalr
PackageCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
PrimaryPackagePurpose: APPLICATION
PackageComment: <text>
CMake target name: qlalr
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qt:6.12.0:*:*:*:*:*:*:*
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qtbase:6.12.0:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-tool-qlalr}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-tool-qlalr}
Relationship: SPDXRef-Package-qtbase-qt-tool-qlalr DEPENDS_ON SPDXRef-Package-qtbase-qt-module-Core
Relationship: SPDXRef-Package-qtbase-qt-tool-qlalr DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformToolInternal
Relationship: SPDXRef-Package-qtbase-qt-tool-qlalr DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformCommonInternal
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-tool-qlalr
"
        )
