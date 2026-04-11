
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-emoji-segmenter")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-emoji-segmenter" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Gui_Attribution_emoji-segmenter
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-emoji-segmenter
PackageDownloadLocation: https://github.com/google/emoji-segmenter/releases/tag/0.4.0
PackageVersion: 0.4.0
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: Apache-2.0
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/google/emoji-segmenter@0.4.0
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Gui_Attribution_emoji-segmenter#src/gui
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Gui_Attribution_emoji-segmenter@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Gui_Attribution_emoji-segmenter#src/gui
PackageCopyrightText: <text>Copyright 2019 Google LLC</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Gui_Attribution_emoji-segmenter
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/emoji-segmenter/qt_attribution.json
    Entry index: 0
    Id: emoji-segmenter
    Name: Emoji Segmenter
    Description: A parser for emoji sequences.
    Qt usage: Used in QtGui for parsing complex emoji sequences. Can be configured using the -emojisegmenter option.
    License: Apache License 2.0
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-emoji-segmenter}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-emoji-segmenter}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-emoji-segmenter
"
        )
