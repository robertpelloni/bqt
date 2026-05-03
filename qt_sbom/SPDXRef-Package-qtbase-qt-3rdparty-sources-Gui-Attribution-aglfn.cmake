
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-aglfn")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-aglfn" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Gui_Attribution_aglfn
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-aglfn
PackageDownloadLocation: https://github.com/adobe-type-tools/agl-aglfn
PackageVersion: 1.7
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: BSD-3-Clause
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/adobe-type-tools/agl-aglfn@1.7
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Gui_Attribution_aglfn#src/gui
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Gui_Attribution_aglfn@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Gui_Attribution_aglfn#src/gui
PackageCopyrightText: <text>Copyright 2002, 2003, 2005, 2006, 2008, 2010, 2015 Adobe Systems</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Gui_Attribution_aglfn
    Information extracted from:
     /src_dir/qtbase/src/gui/text/qt_attribution.json
    Entry index: 0
    Id: aglfn
    Name: Adobe Glyph List For New Fonts
    Description: Provides standardized names for glyphs.
    Qt usage: Used by PDF generator to make it easier for reader applications to resolve  the original contents of rendered text.
    License: BSD 3-Clause \"New\" or \"Revised\" License
    License file: LICENSE.AGLFN.txt
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-aglfn}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-aglfn}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-aglfn
"
        )
