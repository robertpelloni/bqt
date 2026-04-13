# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_json Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_json LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

set(json_resource_files
    "bom.json"
    "test.json"
    "test2.json"
    "test3.json"
    "simple.duplicates.json"
    "test.duplicates.json"
    "test3.duplicates.json"
)

bobui_internal_add_test(tst_json
    SOURCES
        tst_bobuijson.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::TestPrivate
    TESTDATA ${json_resource_files}
)

bobui_internal_extend_target(tst_json CONDITION NOT BOBUI_FEATURE_doubleconversion AND NOT BOBUI_FEATURE_system_doubleconversion
    DEFINES
        BOBUI_NO_DOUBLECONVERSION
)
