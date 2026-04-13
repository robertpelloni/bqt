# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if (NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
     cmake_minimum_required(VERSION 3.16)
     project(xmlstreamlint LANGUAGES CXX)
     find_package(BobUI6BuildInternals COMPONENTS STANDALONE_TEST)
 endif()

bobui_internal_add_manual_test(xmlstreamlint
    SOURCES
        main.cpp
)

set_target_properties(xmlstreamlint PROPERTIES
    WIN32_EXECUTABLE FALSE
    MACOSX_BUNDLE FALSE
)

target_link_libraries(xmlstreamlint PRIVATE
    BobUI::Core
    BobUI::Xml
)

