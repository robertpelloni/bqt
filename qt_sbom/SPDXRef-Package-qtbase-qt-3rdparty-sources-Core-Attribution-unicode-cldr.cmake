
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-unicode-cldr")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-unicode-cldr" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Core_Attribution_unicode-cldr
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-unicode-cldr
PackageDownloadLocation: https://cldr.unicode.org/
PackageVersion: v48.1
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: Unicode-3.0
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Core_Attribution_unicode-cldr#src/corelib
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Core_Attribution_unicode-cldr@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Core_Attribution_unicode-cldr#src/corelib
PackageCopyrightText: <text>Copyright (C) 2004-2025 Unicode, Inc.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Core_Attribution_unicode-cldr
    Information extracted from:
     /src_dir/qtbase/src/corelib/text/qt_attribution.json
    Entry index: 1
    Id: unicode-cldr
    Name: Unicode Common Locale Data Repository (CLDR)
    Description: The Unicode CLDR provides key building blocks for software to support the
 world's languages, with the largest and most extensive standard repository of locale data
 available.
    Qt usage: Used in Qt Core (QTimeZone, QLocale).
    License: Unicode License v3
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-unicode-cldr}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-unicode-cldr}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-unicode-cldr
"
        )
