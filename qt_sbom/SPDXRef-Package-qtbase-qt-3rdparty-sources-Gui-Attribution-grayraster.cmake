
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-grayraster")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-grayraster" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Gui_Attribution_grayraster
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-grayraster
PackageDownloadLocation: http://www.freetype.org
PackageVersion: unknown
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: FTL OR GPL-2.0-only
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:generic/freetype-grayraster?download_url=https://www.freetype.org/
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Gui_Attribution_grayraster#src/gui
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Gui_Attribution_grayraster@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Gui_Attribution_grayraster#src/gui
PackageCopyrightText: <text>Copyright 2000-2016 by David Turner, Robert Wilhelm, and Werner Lemberg.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Gui_Attribution_grayraster
    Information extracted from:
     /src_dir/qtbase/src/gui/painting/qt_attribution.json
    Entry index: 0
    Id: grayraster
    Name: Anti-aliasing rasterizer from FreeType 2
    Description: FreeType is a freely available software library to render fonts.
    Qt usage: Used in Qt GUI.
    License: Freetype Project License or GNU General Public License v2.0 only
    License file: ../../3rdparty/freetype/LICENSE.txt
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-grayraster}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-grayraster}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-grayraster
"
        )
