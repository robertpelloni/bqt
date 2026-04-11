
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-module-OpenGLWidgets")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-OpenGLWidgets" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: OpenGLWidgets
SPDXID: SPDXRef-Package-qtbase-qt-module-OpenGLWidgets
PackageDownloadLocation: git://code.qt.io/qt/qtbase.git@ce5f43eaf95f13997977a321f14b4913a193cd5d
PackageVersion: 6.12.0
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
PackageLicenseDeclared: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=OpenGLWidgets#src/openglwidgets
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-OpenGLWidgets@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=OpenGLWidgets#src/openglwidgets
PackageCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: OpenGLWidgets
CMake exported target name: Qt6::OpenGLWidgets
Contained in CMake package: Qt6OpenGLWidgets
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qt:6.12.0:*:*:*:*:*:*:*
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qtbase:6.12.0:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-module-OpenGLWidgets}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-OpenGLWidgets}
Relationship: SPDXRef-Package-qtbase-qt-module-OpenGLWidgets DEPENDS_ON SPDXRef-Package-qtbase-qt-module-OpenGL
Relationship: SPDXRef-Package-qtbase-qt-module-OpenGLWidgets DEPENDS_ON SPDXRef-Package-qtbase-qt-module-Widgets
Relationship: SPDXRef-Package-qtbase-qt-module-OpenGLWidgets DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformModuleInternal
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-module-OpenGLWidgets
"
        )
