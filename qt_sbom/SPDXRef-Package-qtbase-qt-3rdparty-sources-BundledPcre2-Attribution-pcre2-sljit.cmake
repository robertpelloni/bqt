
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-BundledPcre2-Attribution-pcre2-sljit")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-BundledPcre2-Attribution-pcre2-sljit" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: BundledPcre2_Attribution_pcre2-sljit
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-BundledPcre2-Attribution-pcre2-sljit
PackageDownloadLocation: https://github.com/PCRE2Project/pcre2/releases/download/pcre2-10.47/pcre2-10.47.tar.bz2
PackageVersion: 10.47
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: BSD-2-Clause
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/PCRE2Project/pcre2@pcre2-10.47
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=BundledPcre2_Attribution_pcre2-sljit#src/3rdparty/pcre2
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-BundledPcre2_Attribution_pcre2-sljit@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=BundledPcre2_Attribution_pcre2-sljit#src/3rdparty/pcre2
PackageCopyrightText: <text>Copyright Zoltan Herczeg</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: BundledPcre2_Attribution_pcre2-sljit
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/pcre2/qt_attribution.json
    Entry index: 1
    Id: pcre2-sljit
    Name: PCRE2 - Stack-less Just-In-Time Compiler
    Description: The PCRE library is a set of functions that implement regular expression pattern matching using the same syntax and semantics as Perl 5.
    Qt usage: Used in Qt Core (QRegularExpression).
    License: BSD 2-clause \"Simplified\" License
    License file: LICENSE-SLJIT
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:pcre:pcre2:10.47:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-BundledPcre2-Attribution-pcre2-sljit}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-BundledPcre2-Attribution-pcre2-sljit}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-BundledPcre2-Attribution-pcre2-sljit
"
        )
