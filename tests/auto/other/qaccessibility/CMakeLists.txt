# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qaccessibility LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if(NOT BOBUI_FEATURE_accessibility)
    return()
endif()

#####################################################################
## tst_qaccessibility Test:
#####################################################################

bobui_internal_add_test(tst_qaccessibility
    SOURCES
        accessiblewidgets.h
        tst_qaccessibility.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::TestPrivate
        BobUI::WidgetsPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qaccessibility CONDITION UNIX AND NOT APPLE AND NOT HAIKU AND NOT INTEGRITY
    LIBRARIES
        m
)

bobui_internal_extend_target(tst_qaccessibility CONDITION WIN32
    LIBRARIES
        ole32
        oleacc
        oleaut32
        uuid
)
