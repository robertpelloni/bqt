# Copyright (C) 2025 Intel Corporatin
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qatomicwait LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qatomicwait
    SOURCES
        tst_qatomicwait.cpp
        ../../../../../src/corelib/thread/qatomicwait.cpp
    DEFINES
        QATOMICWAIT_USE_FALLBACK
    LIBRARIES
        BobUI::CorePrivate
)
