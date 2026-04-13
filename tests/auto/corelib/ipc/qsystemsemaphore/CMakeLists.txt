# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qsystemsemaphore Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qsystemsemaphore LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qsystemsemaphore
    SOURCES
        tst_qsystemsemaphore.cpp
)

add_subdirectory(acquirerelease)
if(BOBUI_FEATURE_process)
    add_dependencies(tst_qsystemsemaphore acquirerelease_helper)
endif()
