
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-catch2")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-catch2" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Test_Attribution_catch2
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-catch2
PackageDownloadLocation: https://github.com/catchorg/Catch2
PackageVersion: 2.13.10
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: BSL-1.0
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/catchorg/Catch2@v2.13.10
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Test_Attribution_catch2#src/testlib
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Test_Attribution_catch2@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Test_Attribution_catch2#src/testlib
PackageCopyrightText: <text>Copyright (c) 2022 Two Blue Cubes Ltd. All rights reserved.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Test_Attribution_catch2
    Information extracted from:
     /src_dir/qtbase/src/testlib/3rdparty/catch2/qt_attribution.json
    Entry index: 0
    Id: catch2
    Name: Catch2
    Description: Catch2 is a multi-paradigm test framework for C++.
    Qt usage: Used for testing of the Qt Test module.
    License: Boost Software License 1.0
    License file: LICENSE.txt
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-catch2}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-catch2}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-catch2
"
        )
