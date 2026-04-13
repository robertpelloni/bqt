# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_plugin(BOBUIlsBackendOpenSSLPlugin
    OUTPUT_NAME qopensslbackend
    CLASS_NAME BOBUIlsBackendOpenSSL
    PLUGIN_TYPE tls
    SOURCES
        ../shared/qx509_base.cpp ../shared/qx509_base_p.h
        ../shared/bobuilskey_base.cpp ../shared/bobuilskey_base_p.h
        ../shared/qasn1element.cpp ../shared/qasn1element_p.h
        bobuilsbackend_openssl.cpp bobuilsbackend_openssl_p.h
        qx509_openssl.cpp qx509_openssl_p.h
        bobuilskey_openssl.cpp bobuilskey_openssl_p.h
        bobuils_openssl.cpp bobuils_openssl_p.h
        qssldiffiehellmanparameters_openssl.cpp
        qsslcontext_openssl.cpp qsslcontext_openssl_p.h
        qsslsocket_openssl_symbols.cpp qsslsocket_openssl_symbols_p.h
        qopenssl_p.h
    LIBRARIES
        BobUI::NetworkPrivate
        BobUI::CorePrivate
    DEFINES
        OPENSSL_API_COMPAT=0x10100000L
        "BOBUI_ENABLE_STRICT_MODE_UP_TO=BOBUI_VERSION_CHECK(6,11,0)"
        BOBUI_NO_CAST_FROM_ASCII
)

if (WIN32) # Windows header issues
    set_target_properties(BOBUIlsBackendOpenSSLPlugin PROPERTIES UNITY_BUILD OFF)
endif()

bobui_internal_extend_target(BOBUIlsBackendOpenSSLPlugin CONDITION BOBUI_FEATURE_dtls
    SOURCES
        qdtls_openssl.cpp qdtls_openssl_p.h
        ../shared/qdtls_base.cpp ../shared/qdtls_base_p.h
)

bobui_internal_extend_target(BOBUIlsBackendOpenSSLPlugin CONDITION APPLE
    SOURCES
        ../shared/qsslsocket_mac_shared.cpp
    LIBRARIES
        ${FWCoreFoundation}
        ${FWSecurity}
)

bobui_internal_extend_target(BOBUIlsBackendOpenSSLPlugin CONDITION ANDROID
    SOURCES
        qsslsocket_openssl_android.cpp
)

bobui_internal_extend_target(BOBUIlsBackendOpenSSLPlugin CONDITION WIN32
    SOURCES
        qwindowscarootfetcher.cpp qwindowscarootfetcher_p.h
        ../shared/qwincrypt_p.h
    LIBRARIES
        crypt32
)

if(BOBUI_FEATURE_openssl_linked)
    target_link_libraries(BOBUIlsBackendOpenSSLPlugin PRIVATE WrapOpenSSL::WrapOpenSSL)
else()
    bobui_internal_add_target_include_dirs(BOBUIlsBackendOpenSSLPlugin
                                        WrapOpenSSLHeaders::WrapOpenSSLHeaders)
endif()
