# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qnetworkcookiejar Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qnetworkcookiejar LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
list(APPEND test_data "parser.json" "testdata/publicsuffix/public_suffix_list.dafsa")

bobui_internal_add_test(tst_qnetworkcookiejar
    SOURCES
        tst_qnetworkcookiejar.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Network
        BobUI::NetworkPrivate
    TESTDATA ${test_data}
)
