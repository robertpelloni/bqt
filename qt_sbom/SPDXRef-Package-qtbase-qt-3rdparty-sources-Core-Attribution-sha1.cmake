
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha1")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha1" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Core_Attribution_sha1
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha1
PackageDownloadLocation: http://www.dominik-reichl.de/projects/csha1/
PackageVersion: unknown
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: LicenseRef-SHA1-Public-Domain
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:generic/csha1?download_url=https://www.dominik-reichl.de/projects/csha1/
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Core_Attribution_sha1#src/corelib
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Core_Attribution_sha1@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Core_Attribution_sha1#src/corelib
PackageCopyrightText: <text>Copyright (C) Dominik Reichl <dominik.reichl@t-online.de>
Copyright (C) 2016 The Qt Company Ltd</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Core_Attribution_sha1
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/sha1/qt_attribution.json
    Entry index: 0
    Id: sha1
    Name: Secure Hash Algorithm SHA-1
    Description: Implements the Secure Hash Algorithms SHA 1
    Qt usage: Used in Qt Core (QCryptographicHash).
    License: Public Domain
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha1}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha1}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha1
"
        )
