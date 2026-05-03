
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/windeployqt.exe"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'bin/windeployqt.exe' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/windeployqt.exe' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/windeployqt.exe" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-tool-windeployqt" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-tool-windeployqt")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-tool-windeployqt"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./bin/windeployqt.exe
SPDXID: SPDXRef-PackagedFile-qt-tool-windeployqt-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-tool-windeployqt CONTAINS SPDXRef-PackagedFile-qt-tool-windeployqt-Release
Relationship: SPDXRef-PackagedFile-qt-tool-windeployqt-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/windeployqt//src_dir/qtbase/src/tools/windeployqt/windeployqt_resource-Release.rc
Relationship: SPDXRef-PackagedFile-qt-tool-windeployqt-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/windeployqt/qmlutils.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-windeployqt-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/windeployqt/qmlutils.h
Relationship: SPDXRef-PackagedFile-qt-tool-windeployqt-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/windeployqt/qtmoduleinfo.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-windeployqt-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/windeployqt/qtmoduleinfo.h
Relationship: SPDXRef-PackagedFile-qt-tool-windeployqt-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/windeployqt/qtplugininfo.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-windeployqt-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/windeployqt/qtplugininfo.h
Relationship: SPDXRef-PackagedFile-qt-tool-windeployqt-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/windeployqt/utils.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-windeployqt-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/windeployqt/utils.h
Relationship: SPDXRef-PackagedFile-qt-tool-windeployqt-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/windeployqt/peheaderinfo.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-windeployqt-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/windeployqt/peheaderinfo.h
Relationship: SPDXRef-PackagedFile-qt-tool-windeployqt-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/windeployqt/main.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-windeployqt-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/windeployqt/windeployqt_longpath.manifest
"
                )
        endif()
