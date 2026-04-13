# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qcommandlineparser Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qcommandlineparser LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qcommandlineparser
    SOURCES
        tst_qcommandlineparser.cpp
)
add_subdirectory(testhelper)
if(BOBUI_FEATURE_process AND NOT ANDROID)
    add_dependencies(tst_qcommandlineparser qcommandlineparser_test_helper)
endif()
