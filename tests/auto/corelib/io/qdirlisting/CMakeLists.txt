# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qdirlisting Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qdirlisting LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
list(APPEND test_data "entrylist")

bobui_internal_add_test(tst_qdirlisting
    SOURCES
        tst_qdirlisting.cpp
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
    TESTDATA ${test_data}
)

# Resources:
set(qdirlisting_resource_files
    "entrylist/directory/dummy"
    "entrylist/file"
)

bobui_internal_add_resource(tst_qdirlisting "qdirlisting"
    PREFIX
        "/testdata/"
    FILES
        ${qdirlisting_resource_files}
)


## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qdirlisting CONDITION CONFIG___contains___builtin_testdata
    DEFINES
        BUILTIN_TESTDATA
)
