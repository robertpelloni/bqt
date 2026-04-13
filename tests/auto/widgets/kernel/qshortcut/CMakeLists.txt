# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qshortcut Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qshortcut LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qshortcut
    SOURCES
        tst_qshortcut.cpp
    INCLUDE_DIRECTORIES
        ..
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
        BobUI::WidgetsPrivate
)

bobui_internal_add_test(tst_qguishortcut_with_qapplication
    SOURCES
        ../../../gui/kernel/qshortcut/tst_qshortcut.cpp
    DEFINES
        tst_QShortcut=tst_QGuiShortcutWithQApplication
    INCLUDE_DIRECTORIES
        ..
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
        BobUI::WidgetsPrivate
)
