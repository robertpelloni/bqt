# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qloggingregistry Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qloggingregistry LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
list(APPEND test_data "bobuilogging.ini")

bobui_internal_add_test(tst_qloggingregistry
    SOURCES
        tst_qloggingregistry.cpp
    LIBRARIES
        BobUI::CorePrivate
    TESTDATA ${test_data}
)
