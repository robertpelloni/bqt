
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-tlexpected")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-tlexpected" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Core_Attribution_tlexpected
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-tlexpected
PackageDownloadLocation: https://github.com/TartanLlama/expected/blob/41d3e1f48d682992a2230b2a715bca38b848b269/include/tl/expected.hpp
PackageVersion: 41d3e1f48d682992a2230b2a715bca38b848b269
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: CC0-1.0
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Core_Attribution_tlexpected#src/corelib
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Core_Attribution_tlexpected@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Core_Attribution_tlexpected#src/corelib
PackageCopyrightText: <text>To the extent possible under law, Sy Brand has waived all copyright and related or neighboring rights to the expected library. This work is published from: United Kingdom.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Core_Attribution_tlexpected
    Information extracted from:
     /src_dir/qtbase/src/corelib/global/qt_attribution.json
    Entry index: 0
    Id: tlexpected
    Name: tl::expected
    Description: Single header implementation of std::expected with functional-style extensions.
    Qt usage: Available as a private type in all Qt modules
    License: Creative Commons Zero v1.0 Universal
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-tlexpected}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-tlexpected}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-tlexpected
"
        )
