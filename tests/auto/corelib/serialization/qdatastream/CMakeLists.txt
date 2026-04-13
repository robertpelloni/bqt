# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qdatastream Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qdatastream LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
list(APPEND test_data "datastream.q42")
list(APPEND test_data "typedef.q5")

bobui_internal_add_test(tst_qdatastream
    SOURCES
        tst_qdatastream.cpp
    LIBRARIES
        BobUI::Gui
    TESTDATA ${test_data}
)
