# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bobuiextbrowser Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_bobuiextbrowser LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.html)
list(APPEND test_data ${test_data_glob})
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.md)
list(APPEND test_data ${test_data_glob})
list(APPEND test_data "markdown.really")
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    subdir/*)
list(APPEND test_data ${test_data_glob})

bobui_internal_add_test(tst_bobuiextbrowser
    SOURCES
        tst_bobuiextbrowser.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
        BobUI::WidgetsPrivate
    TESTDATA ${test_data}
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_bobuiextbrowser CONDITION builtin_testdata
    DEFINES
        BUILTIN_TESTDATA
)
