# Copyright (C) 2022 Intel Corporation.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qnativeipckey LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qnativeipckey
    SOURCES
        tst_qnativeipckey.cpp
    LIBRARIES
        BobUI::TestPrivate
)
