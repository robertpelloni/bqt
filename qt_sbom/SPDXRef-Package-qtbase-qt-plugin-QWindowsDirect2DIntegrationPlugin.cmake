
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-plugin-QWindowsDirect2DIntegrationPlugin")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-plugin-QWindowsDirect2DIntegrationPlugin" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: QWindowsDirect2DIntegrationPlugin
SPDXID: SPDXRef-Package-qtbase-qt-plugin-QWindowsDirect2DIntegrationPlugin
PackageDownloadLocation: git://code.qt.io/qt/qtbase.git@ce5f43eaf95f13997977a321f14b4913a193cd5d
PackageVersion: 6.12.0
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
PackageLicenseDeclared: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=QWindowsDirect2DIntegrationPlugin#src/plugins/platforms/direct2d
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-QWindowsDirect2DIntegrationPlugin@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=QWindowsDirect2DIntegrationPlugin#src/plugins/platforms/direct2d
PackageCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: QWindowsDirect2DIntegrationPlugin
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qt:6.12.0:*:*:*:*:*:*:*
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qtbase:6.12.0:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-plugin-QWindowsDirect2DIntegrationPlugin}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-plugin-QWindowsDirect2DIntegrationPlugin}
Relationship: SPDXRef-Package-qtbase-qt-plugin-QWindowsDirect2DIntegrationPlugin DEPENDS_ON SPDXRef-Package-qtbase-qt-module-Core
Relationship: SPDXRef-Package-qtbase-qt-plugin-QWindowsDirect2DIntegrationPlugin DEPENDS_ON SPDXRef-Package-qtbase-qt-module-Gui
Relationship: SPDXRef-Package-qtbase-qt-plugin-QWindowsDirect2DIntegrationPlugin DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformPluginInternal
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-plugin-QWindowsDirect2DIntegrationPlugin
"
        )
