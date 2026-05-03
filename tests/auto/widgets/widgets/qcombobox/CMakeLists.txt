# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qcombobox Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qcombobox LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
list(APPEND test_data "bobuilogo.png")
list(APPEND test_data "bobuilogoinverted.png")

bobui_internal_add_test(tst_qcombobox
    SOURCES
        tst_qcombobox.cpp
    DEFINES
        BOBUIEST_QPA_MOUSE_HANDLING
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::TestPrivate
        BobUI::Widgets
        BobUI::WidgetsPrivate
    TESTDATA ${test_data}
)
