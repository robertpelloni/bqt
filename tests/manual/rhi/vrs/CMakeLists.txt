# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(vrs LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_manual_test(vrs
    GUI
    SOURCES
        vrs.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)

set_source_files_properties("../shared/texture.vert.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.vert.qsb"
)
set_source_files_properties("../shared/texture.frag.qsb"
    PROPERTIES BOBUI_RESOURCE_ALIAS "texture.frag.qsb"
)
set_source_files_properties("../shared/bobui256.png"
    PROPERTIES BOBUI_RESOURCE_ALIAS "bobui256.png"
)
bobui_internal_add_resource(vrs "vrs"
    PREFIX
        "/"
    FILES
        "../shared/texture.vert.qsb"
        "../shared/texture.frag.qsb"
        "../shared/bobui256.png"
)

bobui_internal_extend_target(vrs CONDITION BOBUI_FEATURE_metal
    SOURCES
        vrs_metaltest.mm
)

set(imgui_base ../shared/imgui)
set(imgui_target vrs)
include(${imgui_base}/imgui.cmakeinc)
