
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-module-EntryPointPrivate")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-EntryPointPrivate" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: EntryPointPrivate
SPDXID: SPDXRef-Package-qtbase-qt-module-EntryPointPrivate
PackageDownloadLocation: git://code.qt.io/qt/qtbase.git@ce5f43eaf95f13997977a321f14b4913a193cd5d
PackageVersion: 6.12.0
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: LicenseRef-Qt-Commercial OR BSD-3-Clause
PackageLicenseDeclared: LicenseRef-Qt-Commercial OR BSD-3-Clause
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=EntryPointPrivate#src/entrypoint
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-EntryPointPrivate@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=EntryPointPrivate#src/entrypoint
PackageCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: EntryPointPrivate
CMake exported target name: Qt6::EntryPoint
Contained in CMake package: Qt6EntryPointPrivate
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qt:6.12.0:*:*:*:*:*:*:*
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qtbase:6.12.0:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-module-EntryPointPrivate}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-EntryPointPrivate}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-module-EntryPointPrivate
"
        )
