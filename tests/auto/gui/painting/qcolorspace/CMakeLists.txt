# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qcolorspace Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qcolorspace LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
file(GLOB_RECURSE test_data
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    resources/*
)

foreach(file IN LISTS test_data)
    set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/${file}" PROPERTIES BOBUI_RESOURCE_ALIAS "${file}")
endforeach()

bobui_internal_add_test(tst_qcolorspace
    SOURCES
        tst_qcolorspace.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
    TESTDATA ${test_data}
    BUILTIN_TESTDATA
)
