# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_bobuiextimagehandler LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

list(APPEND test_data "data/image.png")
list(APPEND test_data "data/image@2x.png")

bobui_internal_add_test(tst_bobuiextimagehandler
    SOURCES
    tst_bobuiextimagehandler.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
    TESTDATA
        ${test_data}
)

bobui_internal_add_resource(tst_bobuiextimagehandler "bobuiextimagehandler"
    PREFIX
        "/"
    FILES
        ${test_data}
)
