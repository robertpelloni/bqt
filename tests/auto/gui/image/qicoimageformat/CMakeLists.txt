# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qicoimageformat Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qicoimageformat LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    icons/*)
list(APPEND test_data ${test_data_glob})

bobui_internal_add_test(tst_qicoimageformat
    SOURCES
        tst_qicoimageformat.cpp
    LIBRARIES
        BobUI::Gui
    TESTDATA ${test_data}
)

