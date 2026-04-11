
        list(APPEND QT_SBOM_PACKAGES "SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibjpeg12bits")
        set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibjpeg12bits" false)

        file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
PackageName: BundledLibjpeg12bits
SPDXID: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibjpeg12bits
PackageDownloadLocation: https://github.com/libjpeg-turbo/libjpeg-turbo/releases/download/3.1.3/libjpeg-turbo-3.1.3.tar.gz
PackageVersion: 3.1.3
PackageSupplier: Organization: TheQtCompany
PackageLicenseConcluded: IJG AND BSD-3-Clause
PackageLicenseDeclared: NOASSERTION
ExternalRef: PACKAGE-MANAGER purl pkg:github/libjpeg-turbo/libjpeg-turbo@3.1.3
ExternalRef: PACKAGE-MANAGER purl pkg:github/qt/qtbase@ce5f43eaf9?library_name=BundledLibjpeg12bits#src/3rdparty/libjpeg
ExternalRef: PACKAGE-MANAGER purl pkg:generic/TheQtCompany/qtbase-BundledLibjpeg12bits@ce5f43eaf9?vcs_url=https://code.qt.io/qt/qtbase.git@ce5f43eaf9&library_name=BundledLibjpeg12bits#src/3rdparty/libjpeg
PackageCopyrightText: <text>Copyright (C) 2009-2024 D. R. Commander
Copyright (C) 2015, 2020 Google, Inc.
Copyright (C) 2019-2020 Arm Limited
Copyright (C) 2015-2016, 2018 Matthieu Darbois
Copyright (C) 2011-2016 Siarhei Siamashka
Copyright (C) 2015 Intel Corporation
Copyright (C) 2013-2014 Linaro Limited
Copyright (C) 2013-2014 MIPS Technologies, Inc.
Copyright (C) 2009, 2012 Pierre Ossman for Cendio AB
Copyright (C) 2009-2011 Nokia Corporation and/or its subsidiary(-ies)
Copyright (C) 1999-2006 MIYASAKA Masaru
Copyright (C) 1999 Ken Murchison
Copyright (C) 1991-2020 Thomas G. Lane, Guido Vollbeding
</text>
PrimaryPackagePurpose: LIBRARY
PackageComment: <text>
CMake target name: BundledLibjpeg12bits
CMake exported target name: Qt6::BundledLibjpeg12bits
Contained in CMake package: Qt6BundledLibjpeg12bits
    Information extracted from:
     /src_dir/qtbase/src/3rdparty/libjpeg/qt_attribution.json
    Entry index: 0
    Id: libjpeg
    Name: LibJPEG-turbo
    Description: The Independent JPEG Group's JPEG software
    Qt usage: Used in the qjpeg image plugin. Configure with -system-libjpeg or -no-libjpeg to avoid.
    License: Independent JPEG Group License and BSD 3-Clause \"New\" or \"Revised\" License
</text>
ExternalRef: SECURITY cpe23Type cpe:2.3:a:libjpeg-turbo:libjpeg-turbo:3.1.3:*:*:*:*:*:*:*\${QT_SBOM_VERIFICATION_CODE_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibjpeg12bits}
FilesAnalyzed: \${QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibjpeg12bits}
Relationship: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibjpeg12bits DEPENDS_ON SPDXRef-Package-qtbase-qt-module-PlatformModuleInternal
Relationship: SPDXRef-Package-qtbase CONTAINS SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibjpeg12bits
"
        )
