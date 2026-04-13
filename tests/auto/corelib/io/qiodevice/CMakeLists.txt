# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qiodevice Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qiodevice LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
list(APPEND test_data "tst_qiodevice.cpp")

bobui_internal_add_test(tst_qiodevice
    SOURCES
        tst_qiodevice.cpp
    LIBRARIES
        BobUI::Network
    TESTDATA ${test_data}
    BOBUI_TEST_SERVER_LIST "apache2" "cyrus"
)
