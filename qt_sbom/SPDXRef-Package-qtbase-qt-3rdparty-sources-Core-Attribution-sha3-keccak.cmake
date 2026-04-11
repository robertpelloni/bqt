
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha3-keccak")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha3-keccak" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Core_Attribution_sha3_keccak
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha3-keccak
PackageDownloadLocation: NOASSERTION
PackageVersion: 3.2
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: CC0-1.0
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:generic/sha3_keccak@3.2?download_url=https://keccak.team/obsolete/KeccakReferenceAndOptimized-3.2.zip
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Core_Attribution_sha3_keccak#src/corelib
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Core_Attribution_sha3_keccak@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Core_Attribution_sha3_keccak#src/corelib
PackageCopyrightText: <text>Guido Bertoni, Joan Daemen, Michaël Peeters and Gilles Van Assche.
To the extent possible under law, the implementers have waived all copyright
and related or neighboring rights to the source code in this file.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Core_Attribution_sha3_keccak
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/sha3/qt_attribution.json
    Entry index: 1
    Id: sha3_keccak
    Name: Secure Hash Algorithm SHA-3 - Keccak
    Description: SHA-3, originally known as Keccak, is a cryptographic hash function.
    Qt usage: Used in Qt Core (QCryptographicHash).
    License: Creative Commons Zero v1.0 Universal
    License file: CC0_LICENSE
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha3-keccak}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha3-keccak}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha3-keccak
"
        )
