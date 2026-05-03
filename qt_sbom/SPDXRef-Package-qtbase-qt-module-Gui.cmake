
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-module-Gui")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-Gui" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Gui
SPDXID: SPDXRef-Package-qtbase-qt-module-Gui
PackageDownloadLocation: git://code.qt.io/qt/qtbase.git@ce5f43eaf95f13997977a321f14b4913a193cd5d
PackageVersion: 6.12.0
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
PackageLicenseDeclared: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Gui#src/gui
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Gui@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Gui#src/gui
PackageCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Gui
CMake exported target name: Qt6::Gui
Contained in CMake package: Qt6Gui
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qt:6.12.0:*:*:*:*:*:*:*
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qtbase:6.12.0:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-module-Gui}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-Gui}
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-module-Core
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformModuleInternal
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledHarfbuzz
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledFreetype
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledZLIB
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledPcre2
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-rhi-miniengine-d3d12-mipmap
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-opengl-headers
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-opengl-es2-headers
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-grayraster
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-smooth-scaling-algorithm
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-xserverhelper
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-aglfn
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-vulkanmemoryallocator
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-emoji-segmenter
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-webgradients
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-icc-srgb-color-profile
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-d3d12memoryallocator
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-md4c
Relationship: SPDXRef-Package-qtbase-qt-module-Gui DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Gui-Attribution-zlib
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-module-Gui
"
        )
