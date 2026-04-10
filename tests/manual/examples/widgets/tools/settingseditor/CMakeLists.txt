# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(settingseditor LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)
bobui_internal_add_manual_test(settingseditor
    SOURCES
        locationdialog.cpp locationdialog.h
        main.cpp
        mainwindow.cpp mainwindow.h
        settingstree.cpp settingstree.h
        variantdelegate.cpp variantdelegate.h
    LIBRARIES
        BobUI::Widgets
)
