# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qsplitter Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qsplitter LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
list(APPEND test_data "extradata.txt")
list(APPEND test_data "setSizes3.dat")

bobui_internal_add_test(tst_qsplitter
    SOURCES
        tst_qsplitter.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
    TESTDATA ${test_data}
)
