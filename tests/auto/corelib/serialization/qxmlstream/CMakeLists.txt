# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qxmlstream Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qxmlstream LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
file(GLOB_RECURSE test_data
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        data/* XML-Test-Suite/*)

file(GLOB_RECURSE tokenError
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        tokenError/*)

bobui_internal_add_test(tst_qxmlstream
    SOURCES
        tst_qxmlstream.cpp
    LIBRARIES
        BobUI::Network
        BobUI::CorePrivate
        BobUI::TestPrivate
    TESTDATA
        ${test_data}
        ${tokenError}
)
