# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qfontdatabase Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qfontdatabase LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
list(APPEND test_data "LED_REAL.TTF")

bobui_internal_add_test(tst_qfontdatabase
    SOURCES
        tst_qfontdatabase.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
    TESTDATA ${test_data}
)

# Resources:
set_source_files_properties("../../../shared/resources/testfont.ttf"
    PROPERTIES BOBUI_RESOURCE_ALIAS "testfont.ttf"
)
set_source_files_properties("../../../shared/resources/testfont_condensed.ttf"
    PROPERTIES BOBUI_RESOURCE_ALIAS "testfont_condensed.ttf"
)
set_source_files_properties("../../../shared/resources/testfont_italic.ttf"
    PROPERTIES BOBUI_RESOURCE_ALIAS "testfont_italic.ttf"
)
set_source_files_properties("../../../shared/resources/testfont_open.otf"
    PROPERTIES BOBUI_RESOURCE_ALIAS "testfont_open.otf"
)
set_source_files_properties("../../../shared/resources/testfont_variable.ttf"
    PROPERTIES BOBUI_RESOURCE_ALIAS "testfont_variable.ttf"
)
set(testdata_resource_files
    "../../../shared/resources/testfont.ttf"
    "../../../shared/resources/testfont_condensed.ttf"
    "../../../shared/resources/testfont_italic.ttf"
    "../../../shared/resources/testfont_open.otf"
    "../../../shared/resources/testfont_variable.ttf"
    "LED_REAL.TTF"
    "BobUITestLimitedFont-Regular.ttf"
    "BobUITestFallbackFont-Regular.ttf"
    "BobUIEmojiTestFont-Regular.ttf"
)

bobui_internal_add_resource(tst_qfontdatabase "testdata"
    PREFIX
        "/"
    FILES
        ${testdata_resource_files}
)

