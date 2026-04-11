
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-plugin-QSQLiteDriverPlugin")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-plugin-QSQLiteDriverPlugin" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: QSQLiteDriverPlugin
SPDXID: SPDXRef-Package-qtbase-qt-plugin-QSQLiteDriverPlugin
PackageDownloadLocation: git://code.qt.io/qt/qtbase.git@ce5f43eaf95f13997977a321f14b4913a193cd5d
PackageVersion: 6.12.0
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
PackageLicenseDeclared: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=QSQLiteDriverPlugin#src/plugins/sqldrivers/sqlite
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-QSQLiteDriverPlugin@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=QSQLiteDriverPlugin#src/plugins/sqldrivers/sqlite
PackageCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: QSQLiteDriverPlugin
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qt:6.12.0:*:*:*:*:*:*:*
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qtbase:6.12.0:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-plugin-QSQLiteDriverPlugin}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-plugin-QSQLiteDriverPlugin}
Relationship: SPDXRef-Package-qtbase-qt-plugin-QSQLiteDriverPlugin DEPENDS_ON SPDXRef-Package-qtbase-qt-module-Core
Relationship: SPDXRef-Package-qtbase-qt-plugin-QSQLiteDriverPlugin DEPENDS_ON SPDXRef-Package-qtbase-qt-module-Sql
Relationship: SPDXRef-Package-qtbase-qt-plugin-QSQLiteDriverPlugin DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformPluginInternal
Relationship: SPDXRef-Package-qtbase-qt-plugin-QSQLiteDriverPlugin DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-QSQLiteDriverPlugin-Attribution-sqlite
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-plugin-QSQLiteDriverPlugin
"
        )
