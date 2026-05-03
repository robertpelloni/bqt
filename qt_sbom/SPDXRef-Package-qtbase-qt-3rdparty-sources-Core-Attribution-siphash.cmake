
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-siphash")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-siphash" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Core_Attribution_siphash
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-siphash
PackageDownloadLocation: https://raw.githubusercontent.com/veorq/SipHash/adcbf09b1684a718f594faa650ffc56bacdb0777/siphash24.c
PackageVersion: unknown
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: CC0-1.0
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/veorq/SipHash@adcbf09b1684a718f594faa650ffc56bacdb0777
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Core_Attribution_siphash#src/corelib
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Core_Attribution_siphash@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Core_Attribution_siphash#src/corelib
PackageCopyrightText: <text>Copyright (C) 2012-2014 Jean-Philippe Aumasson
Copyright (C) 2012-2014 Daniel J. Bernstein <djb@cr.yp.to>
Copyright (C) 2016 Intel Corporation</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Core_Attribution_siphash
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/siphash/qt_attribution.json
    Entry index: 0
    Id: siphash
    Name: SipHash Algorithm
    Description: Implements the SipHash algorithm.
    Qt usage: Used in Qt Core (QHash)
    License: Creative Commons Zero v1.0 Universal
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-siphash}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-siphash}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-siphash
"
        )
