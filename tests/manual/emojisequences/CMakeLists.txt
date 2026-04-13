# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(emojisequences LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(BobUI6BuildInternals COMPONENTS STANDALONE_TEST)

bobui_standard_project_setup()

bobui_internal_add_manual_test(emojisequences
    GUI
    SOURCES
        main.cpp
        mainwindow.h mainwindow.cpp
        mainwindow.ui
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
    ENABLE_AUTOGEN_TOOLS
        uic
)

set(emojisequences_resource_files
    "emoji-test.txt"
)

bobui_internal_add_resource(emojisequences "emojisequences"
    PREFIX
        "/"
    FILES
        ${emojisequences_resource_files}
)
