
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-md4c")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-md4c" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Gui_Attribution_md4c
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-md4c
PackageDownloadLocation: https://github.com/mity/md4c/releases/tag/release-0.5.2
PackageVersion: 0.5.2
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: MIT
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/mity/md4c@release-0.5.2
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Gui_Attribution_md4c#src/gui
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Gui_Attribution_md4c@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Gui_Attribution_md4c#src/gui
PackageCopyrightText: <text>Copyright © 2016-2024 Martin Mitáš</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Gui_Attribution_md4c
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/md4c/qt_attribution.json
    Entry index: 0
    Id: md4c
    Name: MD4C
    Description: A CommonMark-compliant Markdown parser.
    Qt usage: Optionally used in QTextDocument if configured with textmarkdownreader.
    License: MIT License
    License file: LICENSE.md
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:md4c_project:md4c:0.5.2:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-md4c}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-md4c}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-md4c
"
        )
