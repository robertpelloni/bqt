
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledPcre2")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledPcre2" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: BundledPcre2
SPDXID: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledPcre2
PackageDownloadLocation: https://github.com/PCRE2Project/pcre2/releases/download/pcre2-10.47/pcre2-10.47.tar.bz2
PackageVersion: 10.47
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: LicenseRef-BSD-3-Clause-with-PCRE2-Binary-Like-Packages-Exception
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/PCRE2Project/pcre2@pcre2-10.47
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=BundledPcre2#src/3rdparty/pcre2
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-BundledPcre2@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=BundledPcre2#src/3rdparty/pcre2
PackageCopyrightText: <text>Copyright (c) 1997-2007 University of Cambridge
Copyright (c) 2007-2024 Philip Hazel
Copyright (c) 2010-2024 Zoltan Herczeg</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: BundledPcre2
CMake exported target name: Qt6::BundledPcre2
Contained in CMake package: Qt6BundledPcre2
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/pcre2/qt_attribution.json
    Entry index: 0
    Id: pcre2
    Name: PCRE2
    Description: The PCRE library is a set of functions that implement regular expression pattern matching using the same syntax and semantics as Perl 5.
    Qt usage: Used in Qt Core (QRegularExpression).
    License: BSD 3-clause \"New\" or \"Revised\" License with PCRE2 binary-like Packages Exception
    License file: LICENCE.md
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:pcre:pcre2:10.47:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledPcre2}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledPcre2}
Relationship: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledPcre2 DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformModuleInternal
Relationship: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledPcre2 DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-BundledPcre2-Attribution-pcre2-sljit
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledPcre2
"
        )
