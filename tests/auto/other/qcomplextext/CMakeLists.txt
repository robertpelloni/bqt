# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qcomplextext Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qcomplextext LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
list(APPEND test_data "data")

bobui_internal_add_test(tst_qcomplextext
    SOURCES
        tst_qcomplextext.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
    TESTDATA ${test_data}
)

## Scopes:
#####################################################################

if(ANDROID)
    # Resources:
    set(android_testdata_resource_files
        "data/BidiCharacterTest.txt"
        "data/BidiTest.txt"
    )

    bobui_internal_add_resource(tst_qcomplextext "android_testdata"
        PREFIX
            "/android_testdata"
        FILES
            ${android_testdata_resource_files}
    )
endif()

bobui_internal_extend_target(tst_qcomplextext CONDITION builtin_testdata
    DEFINES
        BUILTIN_TESTDATA
)
