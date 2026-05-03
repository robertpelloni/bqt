# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qmake LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    testdata/*)
list(APPEND test_data ${test_data_glob})

bobui_internal_add_test(tst_qmake
    SOURCES
        testcompiler.cpp testcompiler.h
        tst_qmake.cpp
    TESTDATA ${test_data}
)

set(dependencies
    BobUI::moc
    BobUI::qmake
)
if(TARGET BobUI::Gui)
    list(APPEND dependencies BobUI::Gui)
endif()
if(TARGET BobUI::Widgets)
    list(APPEND dependencies BobUI::Widgets)
endif()
if(TARGET BobUI::rcc)
    list(APPEND dependencies BobUI::rcc)
endif()
if(TARGET BobUI::uic)
    list(APPEND dependencies BobUI::uic)
endif()
# Add dependencies that are implicitly used inside the test
add_dependencies(tst_qmake ${dependencies})
