# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qfont Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qfont LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Resources:
set_source_files_properties("../../../shared/resources/testfont_variable.ttf"
    PROPERTIES BOBUI_RESOURCE_ALIAS "testfont_variable.ttf"
)

set(testfont_resource_files
    "datastream.515"
    "weirdfont.otf"
    "../../../shared/resources/testfont_variable.ttf"
)

bobui_internal_add_test(tst_qfont
    SOURCES
        tst_qfont.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::TestPrivate
    TESTDATA ${testfont_resource_files}
    BUILTIN_TESTDATA
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qfont CONDITION TARGET BobUI::Widgets
    LIBRARIES
        BobUI::Widgets
)
