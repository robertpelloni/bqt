# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qwizard Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qwizard LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Resources:
set(qwizard_resource_files
    "images/background.png"
    "images/banner.png"
    "images/logo.png"
    "images/watermark.png"
)

bobui_internal_add_test(tst_qwizard
    SOURCES
        tst_qwizard.cpp
        tst_qwizard_2.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
        BobUI::WidgetsPrivate
    TESTDATA ${qwizard_resource_files}
    BUILTIN_TESTDATA
)

