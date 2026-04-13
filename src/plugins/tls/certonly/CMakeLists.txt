# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_plugin(BOBUIlsBackendCertOnlyPlugin
    OUTPUT_NAME qcertonlybackend
    CLASS_NAME BOBUIlsBackendCertOnly
    PLUGIN_TYPE tls
    DEFAULT_IF NOT BOBUI_FEATURE_securetransport AND NOT (BOBUI_FEATURE_openssl OR BOBUI_FEATURE_openssl_linked) AND NOT BOBUI_FEATURE_schannel
    SOURCES
        ../shared/qx509_base_p.h
        ../shared/qx509_base.cpp
        ../shared/qx509_generic_p.h
        ../shared/qx509_generic.cpp
        ../shared/qasn1element_p.h
        ../shared/qasn1element.cpp
        bobuilsbackend_cert.cpp
        bobuilsbackend_cert_p.h
    LIBRARIES
        BobUI::NetworkPrivate
    DEFINES
        "BOBUI_ENABLE_STRICT_MODE_UP_TO=BOBUI_VERSION_CHECK(6,11,0)"
        BOBUI_NO_CAST_FROM_ASCII
)
