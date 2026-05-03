# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if (NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(mouseevents LANGUAGES C CXX)
    find_package(BobUI6BuildInternals COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_manual_test(mouseevents
    SOURCES
        mouseevents.cpp
    DEFINES
        "BOBUI_ENABLE_STRICT_MODE_UP_TO=BOBUI_VERSION_CHECK(6,11,0)"
        BOBUI_NO_CAST_FROM_ASCII
    LIBRARIES
        BobUI::Widgets
)
