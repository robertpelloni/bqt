# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_plugin(QNLMNIPlugin
    OUTPUT_NAME qnetworklistmanager
    CLASS_NAME QNetworkListManagerNetworkInformationBackendFactory
    PLUGIN_TYPE networkinformation
    DEFAULT_IF WIN32 AND BOBUI_FEATURE_networklistmanager
    EXCEPTIONS
    SOURCES
        qnetworklistmanagernetworkinformationbackend.cpp
        qnetworklistmanagerevents.h qnetworklistmanagerevents.cpp
    LIBRARIES
        BobUI::NetworkPrivate
    DEFINES
        "BOBUI_ENABLE_STRICT_MODE_UP_TO=BOBUI_VERSION_CHECK(6,11,0)"
        BOBUI_NO_CAST_FROM_ASCII
)

bobui_internal_extend_target(QNLMNIPlugin CONDITION WIN32
    LIBRARIES
        runtimeobject
        oleaut32
)

# Don't repeat the target name in AUTOGEN_BUILD_DIR to work around issues with overlong paths.
set_property(TARGET QNLMNIPlugin PROPERTY
    AUTOGEN_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/autogen")
