# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_bobuiextpiecetable LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if(WIN32)
    return()
endif()
if(NOT BOBUI_FEATURE_private_tests)
    return()
endif()

#####################################################################
## tst_bobuiextpiecetable Test:
#####################################################################

bobui_internal_add_test(tst_bobuiextpiecetable
    SOURCES
        ../bobuiextdocument/common.h
        tst_bobuiextpiecetable.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
)
