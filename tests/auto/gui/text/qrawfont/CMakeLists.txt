# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qrawfont Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qrawfont LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Resources:
set_source_files_properties("../../../shared/resources/testfont.ttf"
    PROPERTIES BOBUI_RESOURCE_ALIAS "testfont.ttf"
)
set(testdata_resource_files
    "../../../shared/resources/testfont.ttf"
    "testfont_zeroem.ttf"
    "testfont_bold_italic.ttf"
    "testfont_os2_v1.ttf"
)

bobui_internal_add_test(tst_qrawfont
    SOURCES
        tst_qrawfont.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
    TESTDATA ${testdata_resource_files}
    BUILTIN_TESTDATA
)

