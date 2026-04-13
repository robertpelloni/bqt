# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(paintedwindow LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/opengl/paintedwindow")

find_package(BobUI6 REQUIRED COMPONENTS Core Gui OpenGL)

bobui_standard_project_setup()

bobui_add_executable(paintedwindow
    main.cpp
    paintedwindow.cpp paintedwindow.h
)

set_target_properties(paintedwindow PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(paintedwindow PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::OpenGL
)

install(TARGETS paintedwindow
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
