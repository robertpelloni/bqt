
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/qtpaths.exe"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'bin/qtpaths.exe' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/qtpaths.exe' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/qtpaths.exe" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-tool-qtpaths" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-tool-qtpaths")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-tool-qtpaths"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./bin/qtpaths.exe
SPDXID: SPDXRef-PackagedFile-qt-tool-qtpaths-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-tool-qtpaths CONTAINS SPDXRef-PackagedFile-qt-tool-qtpaths-Release
Relationship: SPDXRef-PackagedFile-qt-tool-qtpaths-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/qtpaths//src_dir/qtbase/src/tools/qtpaths/qtpaths_resource-Release.rc
Relationship: SPDXRef-PackagedFile-qt-tool-qtpaths-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/qtpaths/qtpaths.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-qtpaths-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/qtpaths/qtpaths_longpath.manifest
"
                )
        endif()
