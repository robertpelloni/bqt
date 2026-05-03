
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-tinycbor")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-tinycbor" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Core_Attribution_tinycbor
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-tinycbor
PackageDownloadLocation: https://github.com/intel/tinycbor/archive/v0.6.1/tinycbor-0.6.1.tar.gz
PackageVersion: 0.6.1
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: MIT
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/intel/tinycbor@v0.6.1
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Core_Attribution_tinycbor#src/corelib
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Core_Attribution_tinycbor@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Core_Attribution_tinycbor#src/corelib
PackageCopyrightText: <text>Copyright (C) 2015-2021 Intel Corporation</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Core_Attribution_tinycbor
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/tinycbor/qt_attribution.json
    Entry index: 0
    Id: tinycbor
    Name: TinyCBOR
    Description: Concise Binary Object Representation (CBOR) Library
    Qt usage: Used for QCborStreamReader and QCborStreamWriter.
    License: MIT License
    License file: LICENSE
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:tinycbor:tinycbor:0.6.1:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-tinycbor}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-tinycbor}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-tinycbor
"
        )
