
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-xserverhelper")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-xserverhelper" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Gui_Attribution_xserverhelper
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-xserverhelper
PackageDownloadLocation: https://www.x.org/
PackageVersion: unknown
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: X11 AND HPND
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:generic/xserverhelper?download_url=https://www.x.org/
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Gui_Attribution_xserverhelper#src/gui
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Gui_Attribution_xserverhelper@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Gui_Attribution_xserverhelper#src/gui
PackageCopyrightText: <text>Copyright (c) 1987, 1988 X Consortium
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Gui_Attribution_xserverhelper
    Information extracted from:
     /src_dir/qtbase/src/gui/painting/qt_attribution.json
    Entry index: 2
    Id: xserverhelper
    Name: X Server helper
    Description: Code from X11's region.h, Region.c, poly.h, and PolyReg.c
    Qt usage: Used in Qt GUI (QRegion).
    License: X11 License and Historical Permission Notice and Disclaimer
    License file: LICENSE.XCONSORTIUM.txt
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-xserverhelper}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-xserverhelper}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-xserverhelper
"
        )
