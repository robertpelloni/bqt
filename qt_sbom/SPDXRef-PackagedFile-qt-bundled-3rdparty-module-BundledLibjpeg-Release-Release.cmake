
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledLibjpeg.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6BundledLibjpeg.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledLibjpeg.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledLibjpeg.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibjpeg" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibjpeg")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibjpeg"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6BundledLibjpeg.lib
SPDXID: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release
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
Relationship: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledLibjpeg CONTAINS SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcapistd.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jccolor.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcdiffct.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jclossls.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcmainct.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcprepct.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcsample.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdapistd.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdcolor.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jddiffct.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdlossls.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdmainct.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdpostct.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdsample.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jutils.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jccoefct.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcdctmgr.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdcoefct.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jddctmgr.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdmerge.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jfdctfst.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jfdctint.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jidctflt.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jidctfst.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jidctint.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jidctred.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jquant1.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jquant2.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jaricom.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcapimin.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcarith.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jchuff.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcicc.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcinit.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jclhuff.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcmarker.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcmaster.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcomapi.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcparam.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jcphuff.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jctrans.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdapimin.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdarith.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdatadst.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdatasrc.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdhuff.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdicc.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdinput.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdlhuff.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdmarker.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdmaster.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdphuff.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jdtrans.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jerror.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jfdctflt.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jmemmgr.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jmemnobs.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledLibjpeg-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/libjpeg/jpeg_nbits.c
"
                )
        endif()
