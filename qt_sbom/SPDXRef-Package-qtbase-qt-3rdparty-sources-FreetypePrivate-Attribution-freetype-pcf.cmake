
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-pcf")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-pcf" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: FreetypePrivate_Attribution_freetype-pcf
SPDXID: SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-pcf
PackageDownloadLocation: https://download.savannah.gnu.org/releases/freetype/freetype-2.14.1.tar.gz
PackageVersion: 2.14.1
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: MIT AND MIT-open-group
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/freetype/freetype@VER-2-14-1
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=FreetypePrivate_Attribution_freetype-pcf#src/3rdparty/freetype
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-FreetypePrivate_Attribution_freetype-pcf@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=FreetypePrivate_Attribution_freetype-pcf#src/3rdparty/freetype
PackageCopyrightText: <text>Copyright (c) 2001, 2012 David Turner, Robert Wilhelm, and Werner Lemberg
Copyright (c) 2000-2014 Francesco Zappa Nardelli
Copyright (c) 1990, 1994, 1998 The Open Group</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: FreetypePrivate_Attribution_freetype-pcf
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/freetype/qt_attribution.json
    Entry index: 3
    Id: freetype-pcf
    Name: Freetype 2 - Portable Compiled Format (PCF) support
    Description: FreeType is a freely available software library to render fonts.
    Qt usage: Optionally used in Qt GUI and platform plugins. Configure with -no-freetype, or -system-freetype to avoid.
    License: MIT License and MIT Open Group variant
    License file: PCF-LICENSE.txt
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:freetype:freetype:2.14.1:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-pcf}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-pcf}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-3rdparty-sources-FreetypePrivate-Attribution-freetype-pcf
"
        )
