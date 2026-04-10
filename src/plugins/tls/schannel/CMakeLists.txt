# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_plugin(QSchannelBackendPlugin
    OUTPUT_NAME qschannelbackend
    CLASS_NAME QSchannelBackend
    PLUGIN_TYPE tls
    DEFAULT_IF WIN32
    SOURCES
        ../shared/bobuilskey_base_p.h
        ../shared/bobuilskey_base.cpp
        ../shared/bobuilskey_generic_p.h
        ../shared/bobuilskey_generic.cpp
        ../shared/qx509_base_p.h
        ../shared/qx509_base.cpp
        ../shared/qx509_generic_p.h
        ../shared/qx509_generic.cpp
        ../shared/qsslsocket_bobui.cpp
        ../shared/qwincrypt_p.h
        ../shared/qasn1element_p.h
        ../shared/qasn1element.cpp
        bobuils_schannel.cpp bobuils_schannel_p.h
        bobuilsbackend_schannel_p.h
        bobuilskey_schannel.cpp bobuilskey_schannel_p.h
        qx509_schannel.cpp qx509_schannel_p.h
    LIBRARIES
        BobUI::NetworkPrivate
        crypt32
        secur32
        bcrypt
        ncrypt
    DEFINES
        "BOBUI_ENABLE_STRICT_MODE_UP_TO=BOBUI_VERSION_CHECK(6,11,0)"
        BOBUI_NO_CAST_FROM_ASCII
)
