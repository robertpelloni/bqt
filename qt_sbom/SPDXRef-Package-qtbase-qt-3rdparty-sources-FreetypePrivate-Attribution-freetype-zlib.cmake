
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-zlib")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-zlib" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: FreetypePrivate_Attribution_freetype-zlib
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-zlib
PackageDownloadLocation: https://download.savannah.gnu.org/releases/freetype/freetype-2.14.1.tar.gz
PackageVersion: 2.14.1
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: Zlib
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/freetype/freetype@VER-2-14-1
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=FreetypePrivate_Attribution_freetype-zlib#src/3rdparty/freetype
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-FreetypePrivate_Attribution_freetype-zlib@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=FreetypePrivate_Attribution_freetype-zlib#src/3rdparty/freetype
PackageCopyrightText: <text>Copyright (C) 1995-2024 Jean-loup Gailly and Mark Adler</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: FreetypePrivate_Attribution_freetype-zlib
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/freetype/qt_attribution.json
    Entry index: 1
    Id: freetype-zlib
    Name: Freetype 2 - zlib
    Description: FreeType is a freely available software library to render fonts.
    Qt usage: Optionally used in Qt GUI and platform plugins. Configure with -no-freetype, or -system-freetype to avoid.
    License: zlib License
    License file: ZLIB-LICENSE.txt
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:freetype:freetype:2.14.1:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-zlib}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-zlib}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-zlib
"
        )
