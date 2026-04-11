
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-rfc6234")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-rfc6234" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Core_Attribution_rfc6234
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-rfc6234
PackageDownloadLocation: NOASSERTION
PackageVersion: unknown
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: BSD-3-Clause
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Core_Attribution_rfc6234#src/corelib
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Core_Attribution_rfc6234@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Core_Attribution_rfc6234#src/corelib
PackageCopyrightText: <text>Copyright (c) 2011 IETF Trust and the persons identified as authors of the code.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Core_Attribution_rfc6234
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/rfc6234/qt_attribution.json
    Entry index: 0
    Id: rfc6234
    Name: Secure Hash Algorithms SHA-384 and SHA-512
    Description: Implements the Secure Hash Algorithms SHA 384 and SHA-521
    Qt usage: Used in Qt Core (QCryptographicHash and QMessageAuthenticationCode)
    License: BSD 3-clause \"New\" or \"Revised\" License
    License file: LICENSE
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-rfc6234}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-rfc6234}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-rfc6234
"
        )
