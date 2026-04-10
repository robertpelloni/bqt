# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

project(UicIncrementalBuild_sameFileDifferentFolder LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Widgets Gui)

set(CMAKE_AUTOMOC ON)

bobui_add_executable(example
    ../../../../src/ui_files/mainwindow.ui
    ../../../../src/ui_files/widget1.ui
    ../../../../widget1.ui
    ../../../../src/mainwindow.h
    ../../../../src/widget1.h
    ../../../../src/widget2.h
    ../../../../src/main.cpp
    ../../../../src/mainwindow.cpp
    ../../../../src/widget1.cpp
    ../../../../src/widget2.cpp
)

target_link_libraries(example PRIVATE BobUI6::Widgets
                                      BobUI6::Core
                                      BobUI6::Gui)

bobui6_add_ui(example
    INCLUDE_PREFIX "../../../../src/ui_files"
    SOURCES "../../../../src/ui_files/mainwindow.ui"
            "../../../../src/ui_files/widget1.ui"
    OPTIONS "$<$<CONFIG:Debug>:-a>")

bobui6_add_ui(example
    INCLUDE_PREFIX "../../../../"
    SOURCES "../../../../widget1.ui"
    OPTIONS "$<$<CONFIG:Debug>:-a>")

