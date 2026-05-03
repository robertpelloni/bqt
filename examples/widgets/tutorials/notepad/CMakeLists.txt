# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(notepad LANGUAGES CXX)

find_package(BobUI6
    REQUIRED COMPONENTS Core Gui Widgets
    OPTIONAL_COMPONENTS PrintSupport
)

bobui_standard_project_setup()

bobui_add_executable(notepad
    main.cpp
    notepad.cpp notepad.h notepad.ui
)

set_target_properties(notepad PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(notepad PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

if(TARGET BobUI6::PrintSupport)
    target_link_libraries(notepad PRIVATE BobUI6::PrintSupport)
endif()

# Resources:
set(notepad_resource_files
    "images/bold.png"
    "images/copy.png"
    "images/create.png"
    "images/cut.png"
    "images/edit_redo.png"
    "images/edit_undo.png"
    "images/exit.png"
    "images/font.png"
    "images/info.png"
    "images/italic.png"
    "images/new.png"
    "images/open.png"
    "images/paste.png"
    "images/pencil.png"
    "images/print.png"
    "images/save.png"
    "images/save_as.png"
    "images/underline.png"
)

bobui_add_resources(notepad "notepad"
    PREFIX
        "/"
    FILES
        ${notepad_resource_files}
)

install(TARGETS notepad
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET notepad
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
