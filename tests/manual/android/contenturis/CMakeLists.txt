# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
if (NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(contenturis LANGUAGES CXX)
    find_package(BobUI6BuildInternals COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(contenturis
    SOURCES
        contenturis.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Widgets
)
