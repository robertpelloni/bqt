# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(windowcontainer LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/widgets/windowcontainer")

find_package(BobUI6 REQUIRED COMPONENTS Core Gui OpenGL Widgets)

bobui_standard_project_setup()

bobui_add_executable(windowcontainer
    ../../opengl/openglwindow/openglwindow.cpp ../../opengl/openglwindow/openglwindow.h
    windowcontainer.cpp
)

set_target_properties(windowcontainer PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_include_directories(windowcontainer PRIVATE
    ../../opengl/openglwindow
)

target_link_libraries(windowcontainer PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::OpenGL
    BobUI6::Widgets
)

install(TARGETS windowcontainer
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
