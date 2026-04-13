# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qstylesheetstyle Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qstylesheetstyle LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Resources:
set(resources_resource_files
    "images/testimage.png"
    "images/testimage@2x.png"
)

bobui_internal_add_test(tst_qstylesheetstyle
    SOURCES
        tst_qstylesheetstyle.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::TestPrivate
        BobUI::Widgets
        BobUI::WidgetsPrivate
    TESTDATA ${resources_resource_files}
    BUILTIN_TESTDATA
)

# _REQUIREMENTS = "bobuiConfig(private_tests)"
