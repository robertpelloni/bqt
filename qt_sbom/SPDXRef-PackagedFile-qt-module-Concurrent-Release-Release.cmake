
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Concurrent.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'lib/Qt6Concurrent.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Concurrent.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/Qt6Concurrent.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-module-Concurrent" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-module-Concurrent")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-module-Concurrent"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./lib/Qt6Concurrent.lib
SPDXID: SPDXRef-PackagedFile-qt-module-Concurrent-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-module-Concurrent CONTAINS SPDXRef-PackagedFile-qt-module-Concurrent-Release
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/include/QtConcurrent/qtconcurrentexports.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtaskbuilder.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrent_global.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentcompilertest.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentfilter.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentfilter.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentfilterkernel.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentfunctionwrappers.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentiteratekernel.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentiteratekernel.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentmap.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentmap.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentmapkernel.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentmedian.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentreducekernel.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentrun.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentrun.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentrunbase.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentstoredfunctioncall.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrenttask.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentthreadengine.cpp
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/qtconcurrentthreadengine.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/ddf_Concurrent.h
Relationship: SPDXRef-PackagedFile-qt-module-Concurrent-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/concurrent/doc/qtconcurrent.qdocconf
"
                )
        endif()
