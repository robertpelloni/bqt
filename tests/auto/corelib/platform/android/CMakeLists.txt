# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_android Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_android LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_android
    SOURCES
        tst_android.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
)

if(ANDROID)
    set_property(TARGET tst_android APPEND PROPERTY BOBUI_ANDROID_PACKAGE_SOURCE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/testdata
    )
endif()

bobui_internal_extend_target(tst_android CONDITION TARGET BobUI::Widgets
    LIBRARIES
        BobUI::Widgets
)
