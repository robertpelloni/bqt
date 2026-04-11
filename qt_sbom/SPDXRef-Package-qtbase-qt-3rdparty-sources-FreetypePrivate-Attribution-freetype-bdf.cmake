
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-bdf")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-bdf" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: FreetypePrivate_Attribution_freetype-bdf
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-bdf
PackageDownloadLocation: https://download.savannah.gnu.org/releases/freetype/freetype-2.14.1.tar.gz
PackageVersion: 2.14.1
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: MIT
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/freetype/freetype@VER-2-14-1
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=FreetypePrivate_Attribution_freetype-bdf#src/3rdparty/freetype
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-FreetypePrivate_Attribution_freetype-bdf@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=FreetypePrivate_Attribution_freetype-bdf#src/3rdparty/freetype
PackageCopyrightText: <text>Copyright (c) 2000 Computing Research Labs, New Mexico State University
Copyright (c) 2001-2014 Francesco Zappa Nardelli</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: FreetypePrivate_Attribution_freetype-bdf
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/freetype/qt_attribution.json
    Entry index: 2
    Id: freetype-bdf
    Name: Freetype 2 - Bitmap Distribution Format (BDF) support
    Description: FreeType is a freely available software library to render fonts.
    Qt usage: Optionally used in Qt GUI and platform plugins. Configure with -no-freetype, or -system-freetype to avoid.
    License: MIT License
    License file: BDF-LICENSE.txt
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:freetype:freetype:2.14.1:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-bdf}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-bdf}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-bdf
"
        )
