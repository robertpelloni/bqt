# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(computegles31 LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/opengl/computegles31")

find_package(BobUI6 REQUIRED COMPONENTS Core Gui OpenGL)

bobui_standard_project_setup()

bobui_add_executable(computegles31
    glwindow.cpp glwindow.h
    main.cpp
)

set_target_properties(computegles31 PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(computegles31 PRIVATE
    BobUI6::Core
    BobUI6::Gui
    BobUI6::OpenGL
)

# Resources:
set(computegles31_resource_files
    "BobUI-logo-medium.png"
)

bobui_add_resources(computegles31 "computegles31"
    PREFIX
        "/"
    FILES
        ${computegles31_resource_files}
)

install(TARGETS computegles31
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
