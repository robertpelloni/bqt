# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(puzzle LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/widgets/draganddrop_puzzle")

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(draganddrop_puzzle
    main.cpp
    mainwindow.cpp mainwindow.h
    pieceslist.cpp pieceslist.h
    puzzlewidget.cpp puzzlewidget.h
)

set_target_properties(draganddrop_puzzle PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(draganddrop_puzzle PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

# Resources:
set(puzzle_resource_files
    "example.jpg"
)

bobui_add_resources(draganddrop_puzzle "puzzle"
    PREFIX
        "/images"
    FILES
        ${puzzle_resource_files}
)

install(TARGETS draganddrop_puzzle
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
