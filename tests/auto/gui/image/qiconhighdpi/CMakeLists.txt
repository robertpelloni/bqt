# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qiconhighdpi Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qiconhighdpi LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    icons/*)
list(APPEND test_data ${test_data_glob})

bobui_internal_add_test(tst_qiconhighdpi
    SOURCES
        tst_qiconhighdpi.cpp
    LIBRARIES
        BobUI::Gui
    TESTDATA ${test_data}
)

# Resources:
set(tst_qiconhighdpi_resource_files
    "icons/misc/button.9.png"
    "icons/misc/button@2x.9.png"
    "icons/testtheme/16x16/actions/appointment-new.png"
    "icons/testtheme/22x22/actions/appointment-new.png"
    "icons/testtheme/22x22@2/actions/appointment-new.png"
    "icons/testtheme/index.theme"
)

bobui_internal_add_resource(tst_qiconhighdpi "tst_qiconhighdpi"
    PREFIX
        "/"
    FILES
        ${tst_qiconhighdpi_resource_files}
)

