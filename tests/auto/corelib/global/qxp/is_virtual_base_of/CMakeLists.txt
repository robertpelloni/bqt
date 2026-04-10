# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qxp_is_virtual_base_of LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qxp_is_virtual_base_of
    EXCEPTIONS
    SOURCES
        tst_is_virtual_base_of.cpp
    LIBRARIES
        BobUI::Core
)

# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=90449
# GCCs < 10 have no way to suppress "inaccessible base" warnings, except by disabling all warnings:
bobui_internal_extend_target(tst_qxp_is_virtual_base_of
    CONDITION GCC AND (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "10")
    COMPILE_OPTIONS -w
)
