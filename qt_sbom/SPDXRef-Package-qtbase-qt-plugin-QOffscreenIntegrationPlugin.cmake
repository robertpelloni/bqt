
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-plugin-QOffscreenIntegrationPlugin")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-plugin-QOffscreenIntegrationPlugin" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: QOffscreenIntegrationPlugin
SPDXID: SPDXRef-Package-qtbase-qt-plugin-QOffscreenIntegrationPlugin
PackageDownloadLocation: git://code.qt.io/qt/qtbase.git@ce5f43eaf95f13997977a321f14b4913a193cd5d
PackageVersion: 6.12.0
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
PackageLicenseDeclared: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=QOffscreenIntegrationPlugin#src/plugins/platforms/offscreen
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-QOffscreenIntegrationPlugin@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=QOffscreenIntegrationPlugin#src/plugins/platforms/offscreen
PackageCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: QOffscreenIntegrationPlugin
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qt:6.12.0:*:*:*:*:*:*:*
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qtbase:6.12.0:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-plugin-QOffscreenIntegrationPlugin}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-plugin-QOffscreenIntegrationPlugin}
Relationship: SPDXRef-Package-qtbase-qt-plugin-QOffscreenIntegrationPlugin DEPENDS_ON SPDXRef-Package-qtbase-qt-module-Core
Relationship: SPDXRef-Package-qtbase-qt-plugin-QOffscreenIntegrationPlugin DEPENDS_ON SPDXRef-Package-qtbase-qt-module-Gui
Relationship: SPDXRef-Package-qtbase-qt-plugin-QOffscreenIntegrationPlugin DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformPluginInternal
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-plugin-QOffscreenIntegrationPlugin
"
        )
