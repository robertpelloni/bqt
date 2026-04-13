# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qfilesystementry LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qioring
    SOURCES
        tst_qioring.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::TestPrivate
    DEFINES
        BOBUIEST_THROW_ON_FAIL
        BOBUIEST_THROW_ON_SKIP
)
