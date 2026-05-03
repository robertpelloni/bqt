
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/imageformats/qico.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'plugins/imageformats/qico.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/imageformats/qico.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/imageformats/qico.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-plugin-QICOPlugin" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-plugin-QICOPlugin")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-plugin-QICOPlugin"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./plugins/imageformats/qico.lib
SPDXID: SPDXRef-PackagedFile-qt-plugin-QICOPlugin-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-plugin-QICOPlugin CONTAINS SPDXRef-PackagedFile-qt-plugin-QICOPlugin-Release
Relationship: SPDXRef-PackagedFile-qt-plugin-QICOPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/imageformats/ico/main.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QICOPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/imageformats/ico/qicohandler.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QICOPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/imageformats/ico/qicohandler.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QICOPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/imageformats/ico/ddf_QICOPlugin.h
"
                )
        endif()
