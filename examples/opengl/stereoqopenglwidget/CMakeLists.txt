# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(stereoqopenglwidget LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets)

bobui_add_executable(stereoqopenglwidget
    glwidget.cpp glwidget.h
    main.cpp
    mainwindow.cpp mainwindow.h
)

set_target_properties(stereoqopenglwidget PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(stereoqopenglwidget PUBLIC
    BobUI6::Core
    BobUI6::Gui
    BobUI6::OpenGL
    BobUI6::OpenGLWidgets
    BobUI6::Widgets
)


install(TARGETS stereoqopenglwidget
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET stereoqopenglwidget
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
