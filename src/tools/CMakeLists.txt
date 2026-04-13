# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# The configure.cmake here does not get picked up automatically.
# Manually evaluate tool-related features.
include("${CMAKE_CURRENT_SOURCE_DIR}/configure.cmake")
bobui_feature_evaluate_features("${CMAKE_CURRENT_SOURCE_DIR}/configure.cmake")

add_subdirectory(uic)
if (BOBUI_FEATURE_dbus)
    add_subdirectory(qdbuscpp2xml)
    add_subdirectory(qdbusxml2cpp)
endif()
add_subdirectory(qlalr)
add_subdirectory(qvkgen)
if (BOBUI_FEATURE_commandlineparser)
    add_subdirectory(bobuipaths)
endif()

if(BOBUI_FEATURE_androiddeploybobui)
    add_subdirectory(androiddeploybobui)
    if(BOBUI_FEATURE_gui AND BOBUI_FEATURE_process AND BOBUI_FEATURE_systemsemaphore)
        add_subdirectory(androidtestrunner)
    endif()
endif()

if(BOBUI_FEATURE_macdeploybobui)
    add_subdirectory(macdeploybobui)
endif()

if(BOBUI_FEATURE_wasmdeploybobui)
    add_subdirectory(wasmdeploybobui)
endif()

if(BOBUI_FEATURE_windeploybobui)
    add_subdirectory(windeploybobui)
endif()

if(BOBUI_FEATURE_bobuiwaylandscanner)
    add_subdirectory(bobuiwaylandscanner)
endif()
