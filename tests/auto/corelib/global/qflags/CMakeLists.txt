# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qflags Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qflags LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qflags
    SOURCES
        tst_qflags.cpp
)

bobui_internal_add_test(tst_qflags64
    SOURCES
        tst_qflags.cpp
    DEFINES
        QFLAGS_TEST_64
)

bobui_internal_add_test(tst_qflags_non_typesafe
    SOURCES
        tst_qflags.cpp
    DEFINES
        QFLAGS_TEST_NO_TYPESAFE_FLAGS
)
