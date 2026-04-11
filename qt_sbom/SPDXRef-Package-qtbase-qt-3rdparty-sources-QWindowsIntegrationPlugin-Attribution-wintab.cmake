
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-QWindowsIntegrationPlugin-Attribution-wintab")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-QWindowsIntegrationPlugin-Attribution-wintab" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: QWindowsIntegrationPlugin_Attribution_wintab
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-QWindowsIntegrationPlugin-Attribution-wintab
PackageDownloadLocation: NOASSERTION
PackageVersion: unknown
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: LicenseRef-Lcs-Telegraphics
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=QWindowsIntegrationPlugin_Attribution_wintab#src/plugins/platforms/windows
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-QWindowsIntegrationPlugin_Attribution_wintab@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=QWindowsIntegrationPlugin_Attribution_wintab#src/plugins/platforms/windows
PackageCopyrightText: <text>Copyright 1991-1998 by LCS/Telegraphics.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: QWindowsIntegrationPlugin_Attribution_wintab
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/wintab/qt_attribution.json
    Entry index: 0
    Id: wintab
    Name: Wintab API
    Description: Wintab is a de facto API for pointing devices on Windows.
    Qt usage: Used in the Qt platform plugin for Windows. Configure with -no-feature-tabletevent to avoid.
    License: LCS-Telegraphics License
</text>\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-QWindowsIntegrationPlugin-Attribution-wintab}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-QWindowsIntegrationPlugin-Attribution-wintab}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-QWindowsIntegrationPlugin-Attribution-wintab
"
        )
