# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bobuiextdocumentlayout Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_bobuiextdocumentlayout LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_bobuiextdocumentlayout
    SOURCES
        tst_bobuiextdocumentlayout.cpp
    LIBRARIES
        BobUI::Gui
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_bobuiextdocumentlayout CONDITION TARGET BobUI::Widgets
    LIBRARIES
        BobUI::Widgets
)
