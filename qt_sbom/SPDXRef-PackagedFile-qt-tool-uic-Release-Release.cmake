
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/uic.exe"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'bin/uic.exe' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/uic.exe' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/uic.exe" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-tool-uic" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-tool-uic")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-tool-uic"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./bin/uic.exe
SPDXID: SPDXRef-PackagedFile-qt-tool-uic-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-tool-uic CONTAINS SPDXRef-PackagedFile-qt-tool-uic-Release
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic//src_dir/qtbase/src/tools/uic/uic_resource-Release.rc
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/cpp/cppwritedeclaration.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/cpp/cppwritedeclaration.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/cpp/cppwriteincludes.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/cpp/cppwriteincludes.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/cpp/cppwriteinitialization.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/cpp/cppwriteinitialization.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/customwidgetsinfo.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/customwidgetsinfo.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/databaseinfo.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/databaseinfo.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/driver.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/driver.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/main.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/option.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/python/pythonwritedeclaration.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/python/pythonwritedeclaration.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/python/pythonwriteimports.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/python/pythonwriteimports.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/shared/language.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/shared/language.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/shared/writeincludesbase.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/shared/writeincludesbase.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/treewalker.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/treewalker.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/ui4.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/ui4.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/uic.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/uic.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/utils.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/validator.cpp
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/validator.h
Relationship: SPDXRef-PackagedFile-qt-tool-uic-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/tools/uic/uic_longpath.manifest
"
                )
        endif()
