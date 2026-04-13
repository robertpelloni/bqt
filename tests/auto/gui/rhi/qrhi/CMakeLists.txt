# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qrhi Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qrhi LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Resources:
file(GLOB_RECURSE qrhi_resource_files
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    data/*
)

bobui_internal_add_test(tst_qrhi
    SOURCES
        tst_qrhi.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
    TESTDATA ${qrhi_resource_files}
    BUILTIN_TESTDATA
)
