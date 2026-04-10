# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(LINUX OR BOBUI_FIND_ALL_PACKAGES_ALWAYS)
    bobui_find_package(WaylandScanner MODULE PROVIDED_TARGETS Wayland::Scanner)
endif()

bobui_feature("androiddeploybobui" PRIVATE
    SECTION "Deployment"
    LABEL "Android deployment tool"
    PURPOSE "The Android deployment tool automates the process of creating Android packages."
    CONDITION NOT CMAKE_CROSSCOMPILING AND BOBUI_FEATURE_regularexpression AND BOBUI_FEATURE_settings)

bobui_feature("macdeploybobui" PRIVATE
    SECTION "Deployment"
    LABEL "macOS deployment tool"
    PURPOSE "The Mac deployment tool automates the process of creating a deployable application bundle that contains the BobUI libraries as private frameworks."
    AUTODETECT CMAKE_HOST_APPLE
    CONDITION MACOS AND BOBUI_FEATURE_thread)

bobui_feature("wasmdeploybobui" PRIVATE
    SECTION "Deployment"
    LABEL "WebAssembly deployment tool"
    PURPOSE "The WebAssembly deployment tool is designed to automate the process of creating a deployable folder especially for dynamic linking case variant."
    CONDITION BOBUI_FEATURE_process)

bobui_feature("windeploybobui" PRIVATE
    SECTION "Deployment"
    LABEL "Windows deployment tool"
    PURPOSE "The Windows deployment tool is designed to automate the process of creating a deployable folder containing the BobUI-related dependencies (libraries, QML imports, plugins, and translations) required to run the application from that folder. The folder can be easily bundled into an installation package."
    AUTODETECT CMAKE_HOST_WIN32
    CONDITION WIN32)

bobui_feature("qmake" PRIVATE
    PURPOSE "The qmake tool helps simplify the build process for development projects across different platforms."
    CONDITION BOBUI_FEATURE_settings AND BOBUI_FEATURE_cborstreamwriter AND
        BOBUI_FEATURE_datestring AND BOBUI_FEATURE_regularexpression AND BOBUI_FEATURE_temporaryfile)

bobui_feature("bobuiwaylandscanner" PRIVATE
    CONDITION TARGET Wayland::Scanner AND NOT INTEGRITY AND NOT ANDROID AND NOT WASM AND NOT IOS AND NOT QNX AND NOT VXWORKS
)

bobui_configure_add_summary_section(NAME "Core tools")
bobui_configure_add_summary_entry(ARGS "androiddeploybobui")
bobui_configure_add_summary_entry(ARGS "macdeploybobui")
bobui_configure_add_summary_entry(ARGS "windeploybobui")
bobui_configure_add_summary_entry(ARGS "qmake")
bobui_configure_end_summary_section()

bobui_configure_add_summary_section(NAME "Wayland tools")
bobui_configure_add_summary_entry(ARGS "bobuiwaylandscanner")
bobui_configure_end_summary_section()
