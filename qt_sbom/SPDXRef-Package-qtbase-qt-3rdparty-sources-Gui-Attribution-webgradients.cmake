
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-webgradients")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-webgradients" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Gui_Attribution_webgradients
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-webgradients
PackageDownloadLocation: https://webgradients.com/
PackageVersion: unknown
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: MIT
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:generic/webgradients?download_url=https://webgradients.com/
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Gui_Attribution_webgradients#src/gui
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Gui_Attribution_webgradients@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Gui_Attribution_webgradients#src/gui
PackageCopyrightText: <text>Copyright (c) 2017 itmeo</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Gui_Attribution_webgradients
    Information extracted from:
     /src_dir/qtbase/util/gradientgen/qt_attribution.json
    Entry index: 0
    Id: webgradients
    Name: WebGradients
    Description: WebGradients is a free collection of 180 linear gradients.
    Qt usage: Used in Qt GUI to provide presets for QGradient.
    License: MIT License
    License file: LICENSE.WEBGRADIENTS.txt
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-webgradients}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-webgradients}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-webgradients
"
        )
