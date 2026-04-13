# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(hellovulkantexture LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/vulkan/hellovulkantexture")

find_package(BobUI6 REQUIRED COMPONENTS Core Gui)

bobui_standard_project_setup()

bobui_add_executable(hellovulkantexture
    hellovulkantexture.cpp hellovulkantexture.h
    main.cpp
)

set_target_properties(hellovulkantexture PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(hellovulkantexture PRIVATE
    BobUI6::Core
    BobUI6::Gui
)

# Resources:
set(hellovulkantexture_resource_files
    "bobui256.png"
    "texture_frag.spv"
    "texture_vert.spv"
)

bobui_add_resources(hellovulkantexture "hellovulkantexture"
    PREFIX
        "/"
    FILES
        ${hellovulkantexture_resource_files}
)

install(TARGETS hellovulkantexture
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
