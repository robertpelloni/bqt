# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(2dpainting LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets)

bobui_standard_project_setup()

bobui_add_executable(2dpainting
    glwidget.cpp glwidget.h
    helper.cpp helper.h
    main.cpp
    widget.cpp widget.h
    window.cpp window.h
)

set_target_properties(2dpainting PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(2dpainting PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::OpenGL
    BobUI6::OpenGLWidgets
    BobUI6::Widgets
)

install(TARGETS 2dpainting
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

bobui_generate_deploy_app_script(
    TARGET 2dpainting
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
