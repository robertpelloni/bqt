# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(contextinfo LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/opengl/contextinfo")

find_package(BobUI6 REQUIRED COMPONENTS Core Gui OpenGL Widgets)

bobui_standard_project_setup()

bobui_add_executable(contextinfo
    main.cpp
    renderwindow.cpp renderwindow.h
    widget.cpp widget.h
)

set_target_properties(contextinfo PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(contextinfo PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::OpenGL
    BobUI6::Widgets
)

install(TARGETS contextinfo
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
