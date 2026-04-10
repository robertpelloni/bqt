# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qmovie Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qmovie LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    animations/* multiframe/*)
list(APPEND test_data ${test_data_glob})

bobui_internal_add_test(tst_qmovie
    SOURCES
        tst_qmovie.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::TestPrivate
    TESTDATA ${test_data}
)

# Resources:
set(resources_resource_files
    "animations/comicsecard.gif"
    "animations/corrupt.gif"
    "animations/trolltech.gif"
    "multiframe/Obj_N2_Internal_Mem.ico"
)

bobui_internal_add_resource(tst_qmovie "resources"
    PREFIX
        "/"
    FILES
        ${resources_resource_files}
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qmovie CONDITION TARGET BobUI::Widgets
    LIBRARIES
        BobUI::Widgets
)

bobui_internal_extend_target(tst_qmovie CONDITION BOBUI_FEATURE_gif
    DEFINES
        BOBUIEST_HAVE_GIF
)

bobui_internal_extend_target(tst_qmovie CONDITION BOBUI_FEATURE_jpeg
    DEFINES
        BOBUIEST_HAVE_JPEG
)
