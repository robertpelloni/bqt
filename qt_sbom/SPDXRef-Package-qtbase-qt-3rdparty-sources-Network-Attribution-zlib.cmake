
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Network-Attribution-zlib")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Network-Attribution-zlib" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Network_Attribution_zlib
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Network-Attribution-zlib
PackageDownloadLocation: https://github.com/madler/zlib/releases/download/v1.3.1/zlib-1.3.1.tar.gz
PackageVersion: 1.3.1
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: Zlib
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/madler/zlib@v1.3.1
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Network_Attribution_zlib#src/network
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Network_Attribution_zlib@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Network_Attribution_zlib#src/network
PackageCopyrightText: <text>(C) 1995-2024 Jean-loup Gailly and Mark Adler</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Network_Attribution_zlib
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/zlib/qt_attribution.json
    Entry index: 0
    Id: zlib
    Name: Data Compression Library (zlib)
    Description: zlib is a general purpose data compression library.
    Qt usage: Optionally used in Qt Core and development tools. Configure with -system-zlib to avoid.
    License: zlib License
    License file: LICENSE
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:zlib:zlib:1.3.1:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Network-Attribution-zlib}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Network-Attribution-zlib}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Network-Attribution-zlib
"
        )
