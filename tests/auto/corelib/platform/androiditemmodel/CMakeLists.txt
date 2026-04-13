# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_androiditemmodel Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_androiditemmodel LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_androiditemmodel
    SOURCES
        tst_androiditemmodel.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::CorePrivate
)

set_property(TARGET tst_androiditemmodel APPEND PROPERTY BOBUI_ANDROID_PACKAGE_SOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/testdata
)
