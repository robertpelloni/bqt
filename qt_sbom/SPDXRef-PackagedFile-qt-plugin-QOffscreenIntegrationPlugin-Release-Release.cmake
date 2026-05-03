
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/platforms/qoffscreen.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'plugins/platforms/qoffscreen.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/platforms/qoffscreen.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/platforms/qoffscreen.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-plugin-QOffscreenIntegrationPlugin" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-plugin-QOffscreenIntegrationPlugin")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-plugin-QOffscreenIntegrationPlugin"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./plugins/platforms/qoffscreen.lib
SPDXID: SPDXRef-PackagedFile-qt-plugin-QOffscreenIntegrationPlugin-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-plugin-QOffscreenIntegrationPlugin CONTAINS SPDXRef-PackagedFile-qt-plugin-QOffscreenIntegrationPlugin-Release
Relationship: SPDXRef-PackagedFile-qt-plugin-QOffscreenIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/offscreen/main.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QOffscreenIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/offscreen/qoffscreencommon.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QOffscreenIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/offscreen/qoffscreencommon.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QOffscreenIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/offscreen/qoffscreenintegration.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QOffscreenIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/offscreen/qoffscreenintegration.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QOffscreenIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/offscreen/qoffscreenwindow.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QOffscreenIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/offscreen/qoffscreenwindow.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QOffscreenIntegrationPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/platforms/offscreen/ddf_QOffscreenIntegrationPlugin.h
"
                )
        endif()
