
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Network-Attribution-libpsl")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Network-Attribution-libpsl" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Network_Attribution_libpsl
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Network-Attribution-libpsl
PackageDownloadLocation: https://github.com/rockdaboot/libpsl
PackageVersion: 664f3dc85259ec65e30248a61fa1c45b7b0e4c3f
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: BSD-3-Clause
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/rockdaboot/libpsl@664f3dc85259ec65e30248a61fa1c45b7b0e4c3f
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Network_Attribution_libpsl#src/network
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Network_Attribution_libpsl@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Network_Attribution_libpsl#src/network
PackageCopyrightText: <text>Copyright 2014-2016 The Chromium Authors. All rights reserved.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Network_Attribution_libpsl
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/libpsl/qt_attribution.json
    Entry index: 1
    Id: libpsl
    Name: libpsl - C library to handle the Public Suffix List
    Description: Libpsl allows checking domains against the Public Suffix List.
    Qt usage: Used to compress the embedded copy of publicsuffix list and
to lookup entries in it.
    License: BSD 3-clause \"New\" or \"Revised\" License
    License file: src/LICENSE.chromium
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Network-Attribution-libpsl}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Network-Attribution-libpsl}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Network-Attribution-libpsl
"
        )
