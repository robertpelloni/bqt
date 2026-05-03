# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bobuiexturefilereader Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_bobuiexturefilereader LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Resources:
set(bobuiexturefilereader_resource_files
    "texturefiles/car.ktx"
    "texturefiles/car_mips.ktx"
    "texturefiles/cubemap_float32_rgba.ktx"
    "texturefiles/cubemap_metadata.ktx"
    "texturefiles/invalid.ktx"
    "texturefiles/newlogo.astc"
    "texturefiles/newlogo_srgb.astc"
    "texturefiles/pattern.pkm"
)

bobui_internal_add_test(tst_bobuiexturefilereader
    SOURCES
        tst_bobuiexturefilereader.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
    TESTDATA ${bobuiexturefilereader_resource_files}
    BUILTIN_TESTDATA
)

