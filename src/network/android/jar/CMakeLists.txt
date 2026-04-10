# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

set(java_sources
    src/org/bobuiproject/bobui/android/network/BobUINetwork.java
)

bobui_internal_add_jar(BobUI${BobUIBase_VERSION_MAJOR}AndroidNetwork
    INCLUDE_JARS ${BOBUI_ANDROID_JAR}
    SOURCES ${java_sources}
    OUTPUT_DIR "${BOBUI_BUILD_DIR}/jar"
)

bobui_path_join(destination ${INSTALL_DATADIR} "jar")

install_jar(BobUI${BobUIBase_VERSION_MAJOR}AndroidNetwork
    DESTINATION ${destination}
    COMPONENT Devel
)
