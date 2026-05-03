# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_baseline_widgets LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_baseline_widgets
    SOURCES
        ../shared/baselineprotocol.cpp ../shared/baselineprotocol.h ../shared/lookup3.cpp
        ../shared/qbaselinetest.cpp ../shared/qbaselinetest.h
        ../shared/qwidgetbaselinetest.cpp ../shared/qwidgetbaselinetest.h
        tst_baseline_widgets.cpp
    INCLUDE_DIRECTORIES
        ../shared
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
        BobUI::WidgetsPrivate
        BobUI::Network
)
