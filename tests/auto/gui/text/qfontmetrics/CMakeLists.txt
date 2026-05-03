# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qfontmetrics Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qfontmetrics LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qfontmetrics
    SOURCES
        tst_qfontmetrics.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
)

# Resources:
set_source_files_properties("../../../shared/resources/testfont.ttf"
    PROPERTIES BOBUI_RESOURCE_ALIAS "testfont.ttf"
)
set_source_files_properties("../../../shared/resources/testfont_linemetrics.otf"
    PROPERTIES BOBUI_RESOURCE_ALIAS "testfont_linemetrics.otf"
)
set_source_files_properties("../../../shared/resources/testfont_bidimarks.ttf"
    PROPERTIES BOBUI_RESOURCE_ALIAS "testfont_bidimarks.ttf"
)
set(testfont_resource_files
    "../../../shared/resources/testfont.ttf"
    "../../../shared/resources/testfont_linemetrics.otf"
    "../../../shared/resources/testfont_bidimarks.ttf"
    "ucs4font.ttf"
)

bobui_internal_add_resource(tst_qfontmetrics "testfont"
    PREFIX
        "/fonts"
    FILES
        ${testfont_resource_files}
)

