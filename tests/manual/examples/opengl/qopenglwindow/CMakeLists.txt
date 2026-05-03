# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(qopenglwindow LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/opengl/qopenglwindow")

find_package(BobUI6 REQUIRED COMPONENTS Core Gui OpenGL)

bobui_standard_project_setup()

bobui_add_executable(qopenglwindow
    background_renderer.cpp background_renderer.h
    main.cpp
)

set_target_properties(qopenglwindow PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(qopenglwindow PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::OpenGL
)

# Resources:
set(shaders_resource_files
    "background.frag"
)

bobui_add_resources(qopenglwindow "shaders"
    PREFIX
        "/"
    FILES
        ${shaders_resource_files}
)

install(TARGETS qopenglwindow
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
