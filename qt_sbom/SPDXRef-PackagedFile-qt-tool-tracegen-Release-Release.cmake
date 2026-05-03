
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/tracegen.exe"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'bin/tracegen.exe' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/tracegen.exe' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/tracegen.exe" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-tool-tracegen" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-tool-tracegen")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-tool-tracegen"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./bin/tracegen.exe
SPDXID: SPDXRef-PackagedFile-qt-tool-tracegen-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-tool-tracegen CONTAINS SPDXRef-PackagedFile-qt-tool-tracegen-Release
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen//src_dir/qtbase/src/tools/tracegen/tracegen_resource-Release.rc
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/etw.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/etw.h
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/helpers.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/helpers.h
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/ctf.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/ctf.h
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/lttng.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/lttng.h
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/panic.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/panic.h
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/provider.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/provider.h
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/qtheaders.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/qtheaders.h
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/tracegen.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-tracegen-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/tracegen/tracegen_longpath.manifest
"
                )
        endif()
