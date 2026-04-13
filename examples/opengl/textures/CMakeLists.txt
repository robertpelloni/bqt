# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(textures LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets)

bobui_standard_project_setup()

bobui_add_executable(textures
    glwidget.cpp glwidget.h
    main.cpp
    window.cpp window.h
)

set_target_properties(textures PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(textures PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::OpenGL
    BobUI6::OpenGLWidgets
    BobUI6::Widgets
)

# Resources:
set(textures_resource_files
    "images/side1.png"
    "images/side2.png"
    "images/side3.png"
    "images/side4.png"
    "images/side5.png"
    "images/side6.png"
)

bobui6_add_resources(textures "textures"
    PREFIX
        "/"
    FILES
        ${textures_resource_files}
)

install(TARGETS textures
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET textures
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
