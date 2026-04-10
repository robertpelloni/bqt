# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_plugin(QConnManNetworkInformationPlugin
    OUTPUT_NAME qconnman
    CLASS_NAME QConnManNetworkInformationBackendFactory
    PLUGIN_TYPE networkinformation
    DEFAULT_IF FALSE
    SOURCES
        qconnmannetworkinformationbackend.h
        qconnmannetworkinformationbackend.cpp
        qconnmanservice.h
        qconnmanservice.cpp
    LIBRARIES
        BobUI::DBus
        BobUI::NetworkPrivate
    DEFINES
        "BOBUI_ENABLE_STRICT_MODE_UP_TO=BOBUI_VERSION_CHECK(6,11,0)"
        BOBUI_NO_CAST_FROM_ASCII
)
