# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## screengadget Binary:
#####################################################################

if (NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(screengadget LANGUAGES C CXX)
    find_package(BobUI6BuildInternals COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_manual_test(screengadget
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
        BobUI::GuiPrivate
)
