
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-PngPrivate")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-PngPrivate" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: PngPrivate
SPDXID: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-PngPrivate
PackageDownloadLocation: https://download.sourceforge.net/libpng/libpng-1.6.54.tar.xz
PackageVersion: 1.6.54
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: Libpng AND libpng-2.0
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/pnggroup/libpng@v1.6.54
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=PngPrivate#src/3rdparty/libpng
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-PngPrivate@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=PngPrivate#src/3rdparty/libpng
PackageCopyrightText: <text>Copyright (c) 1995-2026 The PNG Reference Library Authors
Copyright (c) 2000-2026 Cosmin Truta
Copyright (c) 1998-2018 Glenn Randers-Pehrson
Copyright (c) 1996-1997 Andreas Dilger
Copyright (c) 1995-1996 Guy Eric Schalnat, Group 42, Inc.
Copyright (c) 2000-2017 Simon-Pierre Cadieux
Copyright (c) 2000-2017 Eric S. Raymond
Copyright (c) 2000-2017 Mans Rullgard
Copyright (c) 2000-2017 Gilles Vollant
Copyright (c) 2000-2017 James Yu
Copyright (c) 2000-2017 Mandar Sahastrabuddhe
Copyright (c) 1998-2000 Tom Lane
Copyright (c) 1998-2000 Willem van Schaik
Copyright (c) 1996-1997 John Bowler
Copyright (c) 1996-1997 Kevin Bracey
Copyright (c) 1996-1997 Sam Bushell
Copyright (c) 1996-1997 Magnus Holmgren
Copyright (c) 1996-1997 Greg Roelofs
Copyright (c) 1996-1997 Tom Tanner
Copyright (c) 1995-1996 Dave Martindale
Copyright (c) 1995-1996 Paul Schmidt
Copyright (c) 1995-1996 Tim Wegner</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: PngPrivate
CMake exported target name: Qt6::Png
Contained in CMake package: Qt6PngPrivate
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/libpng/qt_attribution.json
    Entry index: 0
    Id: libpng
    Name: LibPNG
    Description: libpng is the official PNG reference library.
    Qt usage: Used in the qpng image plugin. Configure with -system-libpng or -no-libpng to avoid.
    License: libpng License and PNG Reference Library version 2
    License file: LICENSE
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:libpng:libpng:1.6.54:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-PngPrivate}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-PngPrivate}
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-PngPrivate
"
        )
