# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(dockwidgets LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/widgets/mainwindows/dockwidgets")

find_package(BobUI6
    REQUIRED COMPONENTS Core Gui Widgets
    OPTIONAL_COMPONENTS PrintSupport
)

bobui_standard_project_setup()

bobui_add_executable(dockwidgets
    main.cpp
    mainwindow.cpp mainwindow.h
)

set_target_properties(dockwidgets PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(dockwidgets PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

if (TARGET BobUI6::PrintSupport)
    target_link_libraries(dockwidgets PRIVATE BobUI6::PrintSupport)
endif()

# Resources:
set(dockwidgets_resource_files
    "images/new.png"
    "images/print.png"
    "images/save.png"
    "images/undo.png"
)

bobui_add_resources(dockwidgets "dockwidgets"
    PREFIX
        "/"
    FILES
        ${dockwidgets_resource_files}
)

install(TARGETS dockwidgets
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
