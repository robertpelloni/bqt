# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(spreadsheet LANGUAGES CXX)

find_package(BobUI6
    REQUIRED COMPONENTS Core Gui Widgets
    OPTIONAL_COMPONENTS PrintSupport
)

bobui_standard_project_setup()

bobui_add_executable(spreadsheet
    main.cpp
    printview.cpp printview.h
    spreadsheet.cpp spreadsheet.h
    spreadsheetdelegate.cpp spreadsheetdelegate.h
    spreadsheetitem.cpp spreadsheetitem.h
)

set_target_properties(spreadsheet PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(spreadsheet PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

if (TARGET BobUI6::PrintSupport)
    target_link_libraries(spreadsheet PRIVATE BobUI6::PrintSupport)
endif()

# Resources:
set(spreadsheet_resource_files
    "images/interview.png"
)

bobui_add_resources(spreadsheet "spreadsheet"
    PREFIX
        "/"
    FILES
        ${spreadsheet_resource_files}
)

install(TARGETS spreadsheet
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET spreadsheet
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
