# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qmenubar Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qmenubar LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qmenubar
    SOURCES
        tst_qmenubar.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::TestPrivate
        BobUI::Widgets
        BobUI::WidgetsPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qmenubar CONDITION MACOS
    SOURCES
        tst_qmenubar_mac.mm
    LIBRARIES
        ${FWAppKit}
)
