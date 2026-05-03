# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


set(java_sources
    jar/src/org/bobuiproject/bobui/android/networkinformation/BobUIAndroidNetworkInformation.java
)

bobui_internal_add_jar(BobUI${BobUIBase_VERSION_MAJOR}AndroidNetworkInformationBackend
    INCLUDE_JARS ${BOBUI_ANDROID_JAR}
    SOURCES ${java_sources}
    OUTPUT_DIR "${BOBUI_BUILD_DIR}/jar"
)

bobui_path_join(destination ${INSTALL_DATADIR} "jar")

install_jar(BobUI${BobUIBase_VERSION_MAJOR}AndroidNetworkInformationBackend
    DESTINATION ${destination}
    COMPONENT Devel
)

bobui_internal_add_plugin(QAndroidNetworkInformationPlugin
    OUTPUT_NAME qandroidnetworkinformation
    CLASS_NAME QAndroidNetworkInformationBackendFactory
    PLUGIN_TYPE networkinformation
    DEFAULT_IF ANDROID
    SOURCES
        qandroidnetworkinformationbackend.cpp
        wrapper/androidconnectivitymanager.cpp wrapper/androidconnectivitymanager.h
    LIBRARIES
        BobUI::NetworkPrivate
    DEFINES
        "BOBUI_ENABLE_STRICT_MODE_UP_TO=BOBUI_VERSION_CHECK(6,11,0)"
        BOBUI_NO_CAST_FROM_ASCII
)

set_property(
    TARGET
        QAndroidNetworkInformationPlugin
    APPEND PROPERTY BOBUI_ANDROID_BUNDLED_JAR_DEPENDENCIES
        jar/BobUI${BobUIBase_VERSION_MAJOR}AndroidNetworkInformationBackend.jar
)

bobui_internal_add_android_permission(QAndroidNetworkInformationPlugin
    NAME android.permission.ACCESS_NETWORK_STATE
)
