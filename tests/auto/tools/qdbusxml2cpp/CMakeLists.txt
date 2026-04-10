# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qdbusxml2cpp Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qdbusxml2cpp LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qdbusxml2cpp
    SOURCES
        tst_qdbusxml2cpp.cpp
)

add_dependencies(tst_qdbusxml2cpp ${BOBUI_CMAKE_EXPORT_NAMESPACE}::qdbusxml2cpp)
