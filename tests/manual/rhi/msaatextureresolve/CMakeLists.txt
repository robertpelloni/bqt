# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(msaatextureresolve LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_manual_test(msaatextureresolve
    GUI
    SOURCES
        msaatextureresolve.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

# Resources:
set_source_files_properties("../shared/color.frag.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "color.frag.qsb"
)
set_source_files_properties("../shared/color.vert.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "color.vert.qsb"
)
set_source_files_properties("../shared/texture.frag.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.frag.qsb"
)
set_source_files_properties("../shared/texture.vert.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.vert.qsb"
)

bobui_internal_add_resource(msaatextureresolve "msaatextureresolve"
    PREFIX
        "/"
    FILES
        "../shared/color.frag.qsb"
        "../shared/color.vert.qsb"
        "../shared/texture.frag.qsb"
        "../shared/texture.vert.qsb"
)
