
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-QSQLiteDriverPlugin-Attribution-sqlite")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-QSQLiteDriverPlugin-Attribution-sqlite" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: QSQLiteDriverPlugin_Attribution_sqlite
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-QSQLiteDriverPlugin-Attribution-sqlite
PackageDownloadLocation: https://www.sqlite.org/2026/sqlite-amalgamation-3510200.zip
PackageVersion: 3.51.2
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: blessing
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/sqlite/sqlite@version-3.51.2
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=QSQLiteDriverPlugin_Attribution_sqlite#src/plugins/sqldrivers/sqlite
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-QSQLiteDriverPlugin_Attribution_sqlite@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=QSQLiteDriverPlugin_Attribution_sqlite#src/plugins/sqldrivers/sqlite
PackageCopyrightText: <text>The authors disclaim copyright to the source code. However, a license can be obtained if needed.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: QSQLiteDriverPlugin_Attribution_sqlite
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/sqlite/qt_attribution.json
    Entry index: 0
    Id: sqlite
    Name: SQLite
    Description: SQLite is a small C library that implements a self-contained, embeddable, zero-configuration SQL database engine.
    Qt usage: Used in Qt SQL Lite plugin. Configure Qt with -system-sqlite or -no-sqlite to avoid.
    License: SQLite Blessing
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:sqlite:sqlite:3.51.2:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-QSQLiteDriverPlugin-Attribution-sqlite}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-QSQLiteDriverPlugin-Attribution-sqlite}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-QSQLiteDriverPlugin-Attribution-sqlite
"
        )
