# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qsharedmemory LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qsharedmemory
    SOURCES
        tst_qsharedmemory.cpp
    LIBRARIES
        BobUI::CorePrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qsharedmemory CONDITION LINUX
    LIBRARIES
        rt
)
add_subdirectory(producerconsumer)
if(BOBUI_FEATURE_process)
    add_dependencies(tst_qsharedmemory producerconsumer_helper)
endif()
