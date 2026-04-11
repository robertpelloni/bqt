
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-module-Core")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-Core" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: Core
SPDXID: SPDXRef-Package-qtbase-qt-module-Core
PackageDownloadLocation: git://code.qt.io/qt/qtbase.git@ce5f43eaf95f13997977a321f14b4913a193cd5d
PackageVersion: 6.12.0
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
PackageLicenseDeclared: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=Core#src/corelib
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-Core@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=Core#src/corelib
PackageCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: Core
CMake exported target name: Qt6::Core
Contained in CMake package: Qt6Core
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qt:6.12.0:*:*:*:*:*:*:*
ExternalRef: SECURITY cpe23Type cpe:2.3:a:qt:qtbase:6.12.0:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-module-Core}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-Core}
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-module-GlobalConfigPrivate
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledZLIB
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformModuleInternal
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-ZlibPrivate
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledPcre2
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-module-Platform
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-system-3rdparty-WrapAtomic
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-unicode-character-database
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-unicode-cldr
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-tlexpected
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-siphash
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-blake2
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-md4
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-md5
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha1
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha3-endian
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-sha3-keccak
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-rfc6234
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-tinycbor
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-doubleconversion
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-easing
Relationship: SPDXRef-Package-qtbase-qt-module-Core DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-Core-Attribution-tika-mimetypes
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-module-Core
"
        )
