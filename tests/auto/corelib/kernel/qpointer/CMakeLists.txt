# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qpointer Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qpointer LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if (NOT BOBUI_FEATURE_thread)
    return()
endif()

bobui_internal_add_test(tst_qpointer
    SOURCES
        tst_qpointer.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::TestPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qpointer CONDITION TARGET BobUI::Widgets
    LIBRARIES
        BobUI::Widgets
)
