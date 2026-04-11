
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-TestInternalsPrivate-Attribution-cmake-runcmake-test-modules")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-TestInternalsPrivate-Attribution-cmake-runcmake-test-modules" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: TestInternalsPrivate_Attribution_cmake-runcmake-test-modules
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-TestInternalsPrivate-Attribution-cmake-runcmake-test-modules
PackageDownloadLocation: https://cmake.org/
PackageVersion: 3.31.5
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: BSD-3-Clause
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=TestInternalsPrivate_Attribution_cmake-runcmake-test-modules#src/testinternal
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-TestInternalsPrivate_Attribution_cmake-runcmake-test-modules@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=TestInternalsPrivate_Attribution_cmake-runcmake-test-modules#src/testinternal
PackageCopyrightText: <text>Copyright © 2000-2024 Kitware, Inc. and Contributors</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: TestInternalsPrivate_Attribution_cmake-runcmake-test-modules
    Information extracted from:
     /src_dir/qtbase/src/testinternal/3rdparty/cmake/qt_attribution.json
    Entry index: 0
    Id: cmake-runcmake-test-modules
    Name: cmake-runcmake-test-modules
    Description: CMake helpers for running CMake tests.
    Qt usage: Used as part of the build system.
    License: BSD 3-Clause \"New\" or \"Revised\" License
    License file: Copyright.txt
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-TestInternalsPrivate-Attribution-cmake-runcmake-test-modules}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-TestInternalsPrivate-Attribution-cmake-runcmake-test-modules}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-TestInternalsPrivate-Attribution-cmake-runcmake-test-modules
"
        )
