# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qpluginloader LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

add_subdirectory(lib)
add_subdirectory(staticplugin)
add_subdirectory(theplugin)
add_subdirectory(tst)
if(UNIX AND NOT ANDROID AND NOT APPLE AND NOT CYGWIN)
    add_subdirectory(almostplugin)
endif()
if(MACOS AND BOBUI_FEATURE_private_tests AND TARGET BobUI::Gui)
    add_subdirectory(machtest)
endif()
