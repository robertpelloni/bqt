
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-cycle")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-cycle" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Test_Attribution_cycle
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-cycle
PackageDownloadLocation: http://fftw.org/
PackageVersion: 3.3.10
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: MIT
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/FFTW/fftw3@fftw-3.3.10
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Test_Attribution_cycle#src/testlib
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Test_Attribution_cycle@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Test_Attribution_cycle#src/testlib
PackageCopyrightText: <text>Copyright (c) 2003, 2007-14 Matteo Frigo
Copyright (c) 2003, 2007-14 Massachusetts Institute of Technology</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Test_Attribution_cycle
    Information extracted from:
     /src_dir/qtbase/src/testlib/3rdparty/cycle/qt_attribution.json
    Entry index: 0
    Id: cycle
    Name: Cycle
    Description: Enables access to the CPU's cycle counters.
    Qt usage: Used in the Qt Test module.
    License: MIT License
    License file: LICENSE.txt
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-cycle}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-cycle}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Test-Attribution-cycle
"
        )
