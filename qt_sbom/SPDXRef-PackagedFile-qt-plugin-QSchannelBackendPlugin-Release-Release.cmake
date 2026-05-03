
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/tls/qschannelbackend.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'plugins/tls/qschannelbackend.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/tls/qschannelbackend.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/tls/qschannelbackend.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-plugin-QSchannelBackendPlugin" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-plugin-QSchannelBackendPlugin")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-plugin-QSchannelBackendPlugin"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./plugins/tls/qschannelbackend.lib
SPDXID: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-plugin-QSchannelBackendPlugin CONTAINS SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qtlskey_base_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qtlskey_base.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qtlskey_generic_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qtlskey_generic.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qx509_base_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qx509_base.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qx509_generic_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qx509_generic.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qsslsocket_qt.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qwincrypt_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qasn1element_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qasn1element.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/schannel/qtls_schannel.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/schannel/qtls_schannel_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/schannel/qtlsbackend_schannel_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/schannel/qtlskey_schannel.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/schannel/qtlskey_schannel_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/schannel/qx509_schannel.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/schannel/qx509_schannel_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QSchannelBackendPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/schannel/ddf_QSchannelBackendPlugin.h
"
                )
        endif()
