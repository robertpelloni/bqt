# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qstring_no_cast_from_bytearray Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qstring_no_cast_from_bytearray LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qstring_no_cast_from_bytearray
    SOURCES
        tst_qstring_no_cast_from_bytearray.cpp
    DEFINES
        BOBUI_NO_CAST_FROM_BYTEARRAY
)
