# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_plugin(QSecureTransportBackendPlugin
    OUTPUT_NAME qsecuretransportbackend
    CLASS_NAME QSecureTransportBackend
    PLUGIN_TYPE tls
    DEFAULT_IF APPLE
    SOURCES
        ../shared/qsslsocket_mac_shared.cpp
        ../shared/bobuilskey_generic_p.h
        ../shared/bobuilskey_generic.cpp
        ../shared/qx509_base_p.h
        ../shared/qx509_base.cpp
        ../shared/qx509_generic_p.h
        ../shared/qx509_generic.cpp
        ../shared/bobuilskey_base_p.h
        ../shared/bobuilskey_base.cpp
        ../shared/qsslsocket_bobui.cpp
        ../shared/qasn1element_p.h
        ../shared/qasn1element.cpp
        bobuilsbackend_st.cpp
        bobuilsbackend_st_p.h
        qx509_st.cpp
        bobuilskey_st.cpp
        bobuilskey_st_p.h
        qx509_st_p.h
        bobuils_st.cpp
        bobuils_st_p.h
    LIBRARIES
        BobUI::NetworkPrivate
        BobUI::CorePrivate
        ${FWCoreFoundation}
        ${FWSecurity}
    DEFINES
        "BOBUI_ENABLE_STRICT_MODE_UP_TO=BOBUI_VERSION_CHECK(6,11,0)"
        BOBUI_NO_CAST_FROM_ASCII
)
