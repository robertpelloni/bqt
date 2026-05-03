
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/cmake_automoc_parser.exe"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'bin/cmake_automoc_parser.exe' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/cmake_automoc_parser.exe' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/cmake_automoc_parser.exe" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-tool-cmake-automoc-parser" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-tool-cmake-automoc-parser")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-tool-cmake-automoc-parser"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./bin/cmake_automoc_parser.exe
SPDXID: SPDXRef-PackagedFile-qt-tool-cmake-automoc-parser-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-tool-cmake-automoc-parser CONTAINS SPDXRef-PackagedFile-qt-tool-cmake-automoc-parser-Release
Relationship: SPDXRef-PackagedFile-qt-tool-cmake-automoc-parser-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/cmake_automoc_parser//src_dir/qtbase/src/tools/cmake_automoc_parser/cmake_automoc_parser_resource-Release.rc
Relationship: SPDXRef-PackagedFile-qt-tool-cmake-automoc-parser-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/cmake_automoc_parser/main.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-cmake-automoc-parser-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/cmake_automoc_parser/cmake_automoc_parser_longpath.manifest
"
                )
        endif()
