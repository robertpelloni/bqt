# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    qss/*)
list(APPEND test_data ${test_data_glob})

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_baseline_stylesheet LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_baseline_stylesheet
    SOURCES
        ../shared/baselineprotocol.cpp ../shared/baselineprotocol.h ../shared/lookup3.cpp
        ../shared/qbaselinetest.cpp ../shared/qbaselinetest.h
        ../shared/qwidgetbaselinetest.cpp ../shared/qwidgetbaselinetest.h
        tst_baseline_stylesheet.cpp
    INCLUDE_DIRECTORIES
        ../shared
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
        BobUI::WidgetsPrivate
        BobUI::Network
    TESTDATA ${test_data}
)

bobui6_add_resources(tst_baseline_stylesheet "tst_baseline_stylesheet"
    PREFIX
        "/"
    FILES
        "icons/align-center.png"
        "icons/align-left.png"
        "icons/align-right.png"
        "icons/arrow-up.png"
)
