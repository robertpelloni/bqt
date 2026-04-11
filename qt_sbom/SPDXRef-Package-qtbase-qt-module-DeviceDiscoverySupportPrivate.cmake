
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-module-DeviceDiscoverySupportPrivate")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-DeviceDiscoverySupportPrivate" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: DeviceDiscoverySupportPrivate
SPDXID: SPDXRef-Package-qtbase-qt-module-DeviceDiscoverySupportPrivate
PackageDownloadLocation: git://code.qt.io/qt/qtbase.git@ce5f43eaf95f13997977a321f14b4913a193cd5d
PackageVersion: 6.12.0
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
PackageLicenseDeclared: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=DeviceDiscoverySupportPrivate#src/platformsupport/devicediscovery
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-DeviceDiscoverySupportPrivate@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=DeviceDiscoverySupportPrivate#src/platformsupport/devicediscovery
PackageCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: DeviceDiscoverySupportPrivate
CMake exported target name: Qt6::DeviceDiscoverySupport
Contained in CMake package: Qt6DeviceDiscoverySupportPrivate
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qt:6.12.0:*:*:*:*:*:*:*
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qtbase:6.12.0:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-module-DeviceDiscoverySupportPrivate}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-DeviceDiscoverySupportPrivate}
Relationship: SPDXRef-Package-qtbase-qt-module-DeviceDiscoverySupportPrivate DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformModuleInternal
Relationship: SPDXRef-Package-qtbase-qt-module-DeviceDiscoverySupportPrivate DEPENDS_ON SPDXRef-Package-qtbase-qt-module-Core
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-module-DeviceDiscoverySupportPrivate
"
        )
