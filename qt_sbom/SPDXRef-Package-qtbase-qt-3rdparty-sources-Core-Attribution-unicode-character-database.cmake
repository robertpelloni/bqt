
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-unicode-character-database")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-unicode-character-database" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Core_Attribution_unicode-character-database
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-unicode-character-database
PackageDownloadLocation: https://www.unicode.org/ucd/
PackageVersion: 34
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: Unicode-3.0
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Core_Attribution_unicode-character-database#src/corelib
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Core_Attribution_unicode-character-database@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Core_Attribution_unicode-character-database#src/corelib
PackageCopyrightText: <text>Copyright (C) 1991-2022 Unicode, Inc.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Core_Attribution_unicode-character-database
    Information extracted from:
     /src_dir/qtbase/src/corelib/text/qt_attribution.json
    Entry index: 0
    Id: unicode-character-database
    Name: Unicode Character Database (UCD)
    Description: The Unicode Character Database (UCD) is a set of files that
 define the Unicode character properties and internal mappings.
    Qt usage: Qt Core uses data obtained from UCD files for working with characters and strings.
    License: Unicode License Agreement - Data Files and Software (2016)
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-unicode-character-database}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-unicode-character-database}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-unicode-character-database
"
        )
