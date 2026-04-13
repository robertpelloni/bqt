# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qrhiwidget LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

file(GLOB_RECURSE qrhiwidget_resource_files
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    data/*
)

bobui_internal_add_test(tst_qrhiwidget
    SOURCES
        tst_qrhiwidget.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
    TESTDATA ${qrhiwidget_resource_files}
    BUILTIN_TESTDATA
)
