# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_macgui LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if(NOT APPLE)
    return()
endif()
if(NOT TARGET BobUI::Widgets)
    return()
endif()

#####################################################################
## tst_macgui Test:
#####################################################################

bobui_internal_add_test(tst_macgui
    SOURCES
        guitest.cpp guitest.h
        tst_macgui.cpp
    NO_PCH_SOURCES
        guitest.cpp # undef BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::CorePrivate
        BobUI::WidgetsPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_macgui CONDITION MACOS
    LIBRARIES
        ${FWApplicationServices}
)
