
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha3-endian")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha3-endian" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Core_Attribution_sha3_endian
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha3-endian
PackageDownloadLocation: NOASSERTION
PackageVersion: 1.0.0
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: BSD-2-Clause
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/BrianGladman/sha@1.0.0
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Core_Attribution_sha3_endian#src/corelib
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Core_Attribution_sha3_endian@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Core_Attribution_sha3_endian#src/corelib
PackageCopyrightText: <text>Copyright (c) 1998-2013, Brian Gladman, Worcester, UK. All rights reserved.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Core_Attribution_sha3_endian
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/sha3/qt_attribution.json
    Entry index: 0
    Id: sha3_endian
    Name: Secure Hash Algorithm SHA-3 - brg_endian
    Description: SHA-3, originally known as Keccak, is a cryptographic hash function.
    Qt usage: Used in Qt Core (QCryptographicHash).
    License: BSD 2-clause \"Simplified\" License
    License file: BRG_ENDIAN_LICENSE
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha3-endian}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha3-endian}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha3-endian
"
        )
