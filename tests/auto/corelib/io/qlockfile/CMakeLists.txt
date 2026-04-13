# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qlockfile Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qlockfile LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qlockfile
    SOURCES
        tst_qlockfile.cpp
    LIBRARIES
        BobUI::Concurrent
        BobUI::CorePrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qlockfile CONDITION WIN32
    LIBRARIES
        advapi32
)
add_subdirectory(qlockfiletesthelper)

if(BOBUI_FEATURE_process AND NOT ANDROID)
    add_dependencies(tst_qlockfile qlockfile_test_helper)
endif()
