
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledZLIB.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6BundledZLIB.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledZLIB.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6BundledZLIB.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledZLIB" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledZLIB")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledZLIB"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6BundledZLIB.lib
SPDXID: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: Zlib
FileCopyrightText: <text>(C) 1995-2024 Jean-loup Gailly and Mark Adler</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-bundled-3rdparty-module-BundledZLIB CONTAINS SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/adler32.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/compress.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/crc32.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/crc32.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/deflate.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/deflate.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/gzclose.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/gzguts.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/gzlib.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/gzread.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/gzwrite.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/infback.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/inffast.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/inffast.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/inffixed.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/inflate.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/inflate.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/inftrees.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/inftrees.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/trees.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/trees.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/uncompr.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/zconf.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/zlib.h
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/zutil.c
Relationship: SPDXRef-PackagedFile-qt-bundled-3rdparty-module-BundledZLIB-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/3rdparty/zlib/src/zutil.h
"
                )
        endif()
