
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledLibjpeg12bits.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6BundledLibjpeg12bits.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledLibjpeg12bits.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledLibjpeg12bits.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibjpeg12bits" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibjpeg12bits")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibjpeg12bits"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6BundledLibjpeg12bits.lib
SPDXID: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: IJG AND BSD-3-Clause
FileCopyrightText: <text>Copyright (C) 2009-2024 D. R. Commander
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
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibjpeg12bits CONTAINS SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcapistd_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jccolor_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcdiffct_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jclossls_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcmainct_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcprepct_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcsample_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdapistd_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdcolor_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jddiffct_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdlossls_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdmainct_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdpostct_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdsample_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jutils_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jccoefct_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcdctmgr_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdcoefct_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jddctmgr_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdmerge_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jfdctfst_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jfdctint_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jidctflt_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jidctfst_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jidctint_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jidctred_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jquant1_12.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg12bits-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jquant2_12.c
"
                )
        endif()
