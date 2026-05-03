# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qbytearrayview Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qbytearrayview LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qbytearrayview
    SOURCES
        tst_qbytearrayview.cpp
        ../qstringview/arrays_of_unknown_bounds.cpp
        ../qstringview/arrays_of_unknown_bounds.h
)

if(BOBUI_FEATURE_sanitize_undefined)
    bobui_internal_extend_target(tst_qbytearrayview
        DEFINES
            BOBUI_SANITIZE_UNDEFINED # GCC (in)famously doesn't provide a predefined macro for this
    )
endif()
