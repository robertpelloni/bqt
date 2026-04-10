# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_plugin(QAppleNetworkInformationPlugin
    OUTPUT_NAME qapplenetworkinformation
    CLASS_NAME QAppleNetworkInformationBackendFactory
    PLUGIN_TYPE networkinformation
    DEFAULT_IF APPLE
    SOURCES
    qapplenetworkinformationbackend.mm
    LIBRARIES
        BobUI::NetworkPrivate
        ${FWNetwork}
    DEFINES
        "BOBUI_ENABLE_STRICT_MODE_UP_TO=BOBUI_VERSION_CHECK(6,11,0)"
        BOBUI_NO_CAST_FROM_ASCII
)
