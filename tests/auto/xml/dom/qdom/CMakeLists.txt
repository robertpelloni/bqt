# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qdom Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qdom LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    testdata/*)
list(APPEND test_data ${test_data_glob})
list(APPEND test_data "doubleNamespaces.xml")
list(APPEND test_data "umlaut.xml")

bobui_internal_add_test(tst_qdom
    SOURCES
        tst_qdom.cpp
    LIBRARIES
        BobUI::Xml
        BobUI::XmlPrivate
        BobUI::TestPrivate
    TESTDATA ${test_data}
)
