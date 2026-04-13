# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(cube LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets)

bobui_standard_project_setup()

bobui_add_executable(cube
    geometryengine.cpp geometryengine.h
    main.cpp
    mainwidget.cpp mainwidget.h
)

set_target_properties(cube PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(cube PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::OpenGL
    BobUI6::OpenGLWidgets
    BobUI6::Widgets
)

# Resources:
set(shaders_resource_files
    "fshader.glsl"
    "vshader.glsl"
)

bobui6_add_resources(cube "shaders"
    PREFIX
        "/"
    FILES
        ${shaders_resource_files}
)

set(textures_resource_files
    "cube.png"
)

bobui6_add_resources(cube "textures"
    PREFIX
        "/"
    FILES
        ${textures_resource_files}
)

install(TARGETS cube
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET cube
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
