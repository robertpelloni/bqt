# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qpathclipper LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if(NOT BOBUI_FEATURE_private_tests)
    return()
endif()

#####################################################################
## tst_qpathclipper Test:
#####################################################################

bobui_internal_add_test(tst_qpathclipper
    SOURCES
        paths.cpp paths.h
        tst_qpathclipper.cpp
    INCLUDE_DIRECTORIES
        .
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qpathclipper CONDITION UNIX AND NOT APPLE AND NOT HAIKU AND NOT INTEGRITY
    LIBRARIES
        m
)
