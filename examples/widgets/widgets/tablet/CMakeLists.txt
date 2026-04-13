# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(bobuitablet LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui Widgets)

bobui_standard_project_setup()

bobui_add_executable(bobuitablet
    main.cpp
    mainwindow.cpp mainwindow.h
    tabletapplication.cpp tabletapplication.h
    tabletcanvas.cpp tabletcanvas.h
)

set_target_properties(bobuitablet PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(bobuitablet PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::Widgets
)

# Resources:
set(images_resource_files
    "images/cursor-airbrush.png"
    "images/cursor-eraser.png"
    "images/cursor-felt-marker.png"
    "images/cursor-pencil.png"
)

bobui_add_resources(bobuitablet "images"
    PREFIX
        "/"
    FILES
        ${images_resource_files}
)

install(TARGETS bobuitablet
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET bobuitablet
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
