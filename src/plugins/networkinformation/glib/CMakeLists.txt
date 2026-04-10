# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_plugin(QGlibNetworkInformationPlugin
    OUTPUT_NAME qglib
    CLASS_NAME QGlibNetworkInformationBackendFactory
    PLUGIN_TYPE networkinformation
    DEFAULT_IF LINUX
    SOURCES
        qglibnetworkinformationbackend.cpp
    LIBRARIES
        BobUI::NetworkPrivate
        GLIB2::GOBJECT
        GLIB2::GIO
    DEFINES
        "BOBUI_ENABLE_STRICT_MODE_UP_TO=BOBUI_VERSION_CHECK(6,11,0)"
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_SIGNALS_SLOTS_KEYWORDS
)
