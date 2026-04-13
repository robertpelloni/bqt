# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qabstractnetworkcache Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qabstractnetworkcache LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    tests/*)
list(APPEND test_data ${test_data_glob})

bobui_internal_add_test(tst_qabstractnetworkcache
    SOURCES
        tst_qabstractnetworkcache.cpp
    LIBRARIES
        BobUI::Network
    TESTDATA ${test_data}
    BOBUI_TEST_SERVER_LIST "apache2"
)

# BOBUI_TEST_SERVER_LIST = "apache2"
