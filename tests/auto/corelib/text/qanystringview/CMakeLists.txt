# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qstringview Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qanystringview LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qanystringview
    SOURCES
        tst_qanystringview.cpp
        ../qstringview/arrays_of_unknown_bounds.cpp
        ../qstringview/arrays_of_unknown_bounds.h
    NO_BATCH # BOBUIBUG-121815
    DEFINES
        BOBUIEST_THROW_ON_FAIL
        BOBUIEST_THROW_ON_SKIP
    LIBRARIES
        BobUI::CorePrivate
        BobUI::TestPrivate
)

## Scopes:
#####################################################################
