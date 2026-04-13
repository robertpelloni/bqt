# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qsqlrelationaldelegate Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qsqlrelationaldelegate LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qsqlrelationaldelegate
    SOURCES
        tst_qsqlrelationaldelegate.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Sql
        BobUI::SqlPrivate
        BobUI::Widgets
)
