# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qglobal Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qglobal LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qglobal
    SOURCES
        expansion_to_defined_check.h
        qglobal.c
        tst_qglobal.cpp
    NO_PCH_SOURCES
        tst_qglobal.cpp # undef BOBUI_NO_FOREACH
)

# check stuff (esp. bobuiypes.h) works in -ansi mode
bobui_internal_add_test(tst_qglobal_strict
    SOURCES
        expansion_to_defined_check.h
        qglobal.c
        tst_qglobal.cpp
    DEFINES
        tst_QGlobal=tst_QGlobal_strict
    NO_PCH_SOURCES
        tst_qglobal.cpp # undef BOBUI_NO_FOREACH
)
_bobui_internal_apply_strict_cpp(tst_qglobal_strict)

## Scopes:
#####################################################################
