
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledHarfbuzz")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledHarfbuzz" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: BundledHarfbuzz
SPDXID: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledHarfbuzz
PackageDownloadLocation: https://github.com/harfbuzz/harfbuzz/releases/tag/12.3.0
PackageVersion: 12.3.0
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: MIT
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/harfbuzz/harfbuzz@12.3.0
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=BundledHarfbuzz#src/3rdparty/harfbuzz-ng
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-BundledHarfbuzz@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=BundledHarfbuzz#src/3rdparty/harfbuzz-ng
PackageCopyrightText: <text>Copyright © 2010-2022  Google, Inc.
Copyright © 2015-2020  Ebrahim Byagowi
Copyright © 2019,2020  Facebook, Inc.
Copyright © 2012,2015  Mozilla Foundation
Copyright © 2011  Codethink Limited
Copyright © 2008,2010  Nokia Corporation and/or its subsidiary(-ies)
Copyright © 2009  Keith Stribley
Copyright © 2011  Martin Hosken and SIL International
Copyright © 2007  Chris Wilson
Copyright © 2005,2006,2020,2021,2022,2023  Behdad Esfahbod
Copyright © 2004,2007,2008,2009,2010,2013,2021,2022,2023  Red Hat, Inc.
Copyright © 1998-2005  David Turner and Werner Lemberg
Copyright © 2016  Igalia S.L.
Copyright © 2022  Matthias Clasen
Copyright © 2018,2021  Khaled Hosny
Copyright © 2018,2019,2020  Adobe, Inc
Copyright © 2013-2015  Alexei Podtelezhnikov</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: BundledHarfbuzz
CMake exported target name: Qt6::BundledHarfbuzz
Contained in CMake package: Qt6BundledHarfbuzz
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/harfbuzz-ng/qt_attribution.json
    Entry index: 0
    Id: harfbuzz-ng
    Name: HarfBuzz-NG
    Description: HarfBuzz is an OpenType text shaping engine.
    Qt usage: Optionally used in Qt GUI. Configure with -system-harfbuzz to force the use of the system library, or -qt-harfbuzz to link statically to the library that is bundled with your Qt version.
    License: MIT License
    License file: COPYING
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:harfbuzz_project:harfbuzz:12.3.0:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledHarfbuzz}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledHarfbuzz}
Relationship: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledHarfbuzz DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformModuleInternal
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledHarfbuzz
"
        )
