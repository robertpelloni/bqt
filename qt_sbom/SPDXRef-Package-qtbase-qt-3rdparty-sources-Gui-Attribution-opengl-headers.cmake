
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-opengl-headers")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-opengl-headers" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Gui_Attribution_opengl-headers
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-opengl-headers
PackageDownloadLocation: https://www.khronos.org/
PackageVersion: Revision 27684
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: MIT
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:generic/khronos-opengl-headers?download_url=https://www.khronos.org/?revision=27684
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Gui_Attribution_opengl-headers#src/gui
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Gui_Attribution_opengl-headers@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Gui_Attribution_opengl-headers#src/gui
PackageCopyrightText: <text>Copyright (c) 2013-2014 The Khronos Group Inc.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Gui_Attribution_opengl-headers
    Information extracted from:
     /src_dir/qtbase/src/gui/opengl/qt_attribution.json
    Entry index: 0
    Id: opengl-headers
    Name: OpenGL Headers
    Description: OpenGL header generated from the Khronos OpenGL / OpenGL ES XML API Registry.
    Qt usage: Used on Windows and Linux in the OpenGL related headers of Qt GUI.
    License: MIT License
    License file: LICENSE.KHRONOS.txt
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-opengl-headers}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-opengl-headers}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-opengl-headers
"
        )
