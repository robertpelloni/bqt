
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledFreetype")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledFreetype" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: BundledFreetype
SPDXID: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledFreetype
PackageDownloadLocation: https://download.savannah.gnu.org/releases/freetype/freetype-2.14.1.tar.gz
PackageVersion: 2.14.1
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: FTL OR GPL-2.0-only
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/freetype/freetype@VER-2-14-1
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=BundledFreetype#src/3rdparty/freetype
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-BundledFreetype@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=BundledFreetype#src/3rdparty/freetype
PackageCopyrightText: <text>Copyright (c) 2007-2014 Adobe Systems Incorporated
Copyright (c) 2004-2025 Albert Chin-A-Young
Copyright (c) 2018-2025 Armin Hasitzka, David Turner, Robert Wilhelm, and Werner Lemberg
Copyright (c) 2000 Computing Research Labs, New Mexico State University
Copyright (c) 1996-2025 David Turner, Robert Wilhelm, Dominik Röttsches, and Werner Lemberg
Copyright (c) 2004-2025 David Turner, Robert Wilhelm, Werner Lemberg and George Williams
Copyright (c) 2022-2025 David Turner, Robert Wilhelm, Werner Lemberg, and Moazin Khatti
Copyright (c) 2008-2025 David Turner, Robert Wilhelm, Werner Lemberg, and suzuki toshiya
Copyright (c) 2003-2025 David Turner, Robert Wilhelm, and Werner Lemberg
Copyright (c) 2005-2025 David Turner
Copyright (c) 2007-2025 Derek Clegg and Michael Toftdal
Copyright (c) 2007 Dmitry Timoshkov for Codeweavers
Copyright (c) 2001-2015 Francesco Zappa Nardelli
Copyright (c) 2005, 2007, 2008, 2013 George Williams
Copyright (c) 2013-2025 Google, Inc. Google Author(s) Behdad Esfahbod and Stuart Gill
Copyright (c) 2013-2022 Google, Inc.
Copyright (c) 2003 Huw D M Davies for Codeweavers
Copyright (c) 2010-2025 Joel Klinghed
Copyright (c) 1996-2025 Just van Rossum, David Turner, Robert Wilhelm, and Werner Lemberg
Copyright (c) 2003-2025 Masatake YAMATO and Redhat K.K.
Copyright (c) 2004-2025 Masatake YAMATO, Redhat K.K, David Turner, Robert Wilhelm, and Werner Lemberg
Copyright (c) 2019-2025 Nikhil Ramakrishnan, David Turner, Robert Wilhelm, and Werner Lemberg
Copyright (c) 2009-2025 Oran Agra and Mickey Gabel
Copyright (c) 2007-2025 Rahul Bhalerao <rahul.bhalerao@redhat.com>
Copyright (c) 2002-2025 Roberto Alameda
Copyright (c) 2015-2025 Werner Lemberg
Copyright (c) 2004-2025 suzuki toshiya, Masatake YAMATO, Red Hat K.K., David Turner, Robert Wilhelm, and Werner Lemberg
Copyright (c) 2019 nyorain
Copyright (c) 2022-2025 David Turner, Robert Wilhelm, Werner Lemberg, George Williams, and Dominik Röttsches
Copyright (C) 2009, 2023  Red Hat, Inc.</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: BundledFreetype
CMake exported target name: Qt6::BundledFreetype
Contained in CMake package: Qt6BundledFreetype
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/freetype/qt_attribution.json
    Entry index: 0
    Id: freetype
    Name: Freetype 2
    Description: FreeType is a freely available software library to render fonts.
    Qt usage: Optionally used in Qt GUI and platform plugins. Configure with -no-freetype, or -system-freetype to avoid.
    License: Freetype Project License or GNU General Public License v2.0 only
    License file: LICENSE.txt
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:freetype:freetype:2.14.1:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledFreetype}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledFreetype}
Relationship: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledFreetype DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformModuleInternal
Relationship: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledFreetype DEPENDS_ON SPDXRef-Package-qtbase-qt-module-Core
Relationship: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledFreetype DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-BundledFreetype-Attribution-freetype-zlib
Relationship: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledFreetype DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-BundledFreetype-Attribution-freetype-bdf
Relationship: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledFreetype DEPENDS_ON SPDXRef-Package-qtbase-qt-3rdparty-sources-BundledFreetype-Attribution-freetype-pcf
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledFreetype
"
        )
