# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qcolumnview Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qcolumnview LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qcolumnview
    SOURCES
        ../../../../shared/fakedirmodel.h
        tst_qcolumnview.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::TestPrivate
        BobUI::Widgets
        BobUI::WidgetsPrivate
)
