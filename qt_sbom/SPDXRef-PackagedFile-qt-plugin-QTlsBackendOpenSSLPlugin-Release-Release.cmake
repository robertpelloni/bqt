
        if(NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/tls/qopensslbackend.lib"
                AND NOT QT_SBOM_BUILD_TIME AND NOT QT_SBOM_FAKE_CHECKSUM)
            if(NOT TRUE)
                message(FATAL_ERROR "Cannot find 'plugins/tls/qopensslbackend.lib' to compute its checksum. "
                    "Expected to find it at '$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/tls/qopensslbackend.lib' ")
            endif()
        else()
            if(NOT QT_SBOM_BUILD_TIME)
                if(QT_SBOM_FAKE_CHECKSUM)
                    set(sha1 "158942a783ee1095eafacaffd93de73edeadbeef")
                else()
                    file(SHA1 "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/tls/qopensslbackend.lib" sha1)
                endif()

                set("QT_SBOM_PACKAGE_HAS_FILES_SPDXRef-Package-qtbase-qt-plugin-QTlsBackendOpenSSLPlugin" true)

                list(APPEND QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES
                    "SPDXRef-Package-qtbase-qt-plugin-QTlsBackendOpenSSLPlugin")
                list(APPEND
                    "QT_SBOM_PACKAGES_WITH_VERIFICATION_CODES_SPDXRef-Package-qtbase-qt-plugin-QTlsBackendOpenSSLPlugin"
                    "${sha1}")
            endif()
            file(APPEND "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in"
"
FileName: ./plugins/tls/qopensslbackend.lib
SPDXID: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release
FileType: BINARY
FileChecksum: SHA1: ${sha1}
LicenseConcluded: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
FileCopyrightText: <text>Copyright (C) The Qt Company Ltd. and other contributors.</text>
LicenseInfoInFile: NOASSERTION
Relationship: SPDXRef-Package-qtbase-qt-plugin-QTlsBackendOpenSSLPlugin CONTAINS SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qx509_base.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qx509_base_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qtlskey_base.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qtlskey_base_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qasn1element.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qasn1element_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qtlsbackend_openssl.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qtlsbackend_openssl_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qx509_openssl.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qx509_openssl_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qtlskey_openssl.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qtlskey_openssl_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qtls_openssl.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qtls_openssl_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qssldiffiehellmanparameters_openssl.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qsslcontext_openssl.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qsslcontext_openssl_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qsslsocket_openssl_symbols.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qsslsocket_openssl_symbols_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qopenssl_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qdtls_openssl.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qdtls_openssl_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qdtls_base.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qdtls_base_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qwindowscarootfetcher.cpp
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/qwindowscarootfetcher_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/shared/qwincrypt_p.h
Relationship: SPDXRef-PackagedFile-qt-plugin-QTlsBackendOpenSSLPlugin-Release GENERATED_FROM NOASSERTION
RelationshipComment: /src_dir/qtbase/src/plugins/tls/openssl/ddf_QTlsBackendOpenSSLPlugin.h
"
                )
        endif()
