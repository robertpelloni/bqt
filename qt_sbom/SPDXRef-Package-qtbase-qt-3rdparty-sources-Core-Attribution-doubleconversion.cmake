
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-doubleconversion")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-doubleconversion" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Core_Attribution_doubleconversion
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-doubleconversion
PackageDownloadLocation: https://github.com/google/double-conversion/releases/tag/v3.4.0
PackageVersion: 3.4.0
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: BSD-3-Clause
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/google/double-conversion@v3.4.0
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Core_Attribution_doubleconversion#src/corelib
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Core_Attribution_doubleconversion@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Core_Attribution_doubleconversion#src/corelib
PackageCopyrightText: <text>Copyright 2006-2012, the V8 project authors</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Core_Attribution_doubleconversion
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/double-conversion/qt_attribution.json
    Entry index: 0
    Id: doubleconversion
    Name: Efficient Binary-Decimal and Decimal-Binary Conversion Routines for IEEE Doubles
    Qt usage: Used in Qt Core. Configure with -system-doubleconversion or -no-doubleconversion to avoid.
    License: BSD 3-clause \"New\" or \"Revised\" License
    License file: LICENSE
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:google:double-conversion:3.4.0:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-doubleconversion}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-doubleconversion}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-doubleconversion
"
        )
