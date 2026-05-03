# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qabstractfileengine Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qabstractfileengine LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qabstractfileengine
    SOURCES
        tst_qabstractfileengine.cpp
    LIBRARIES
        BobUI::CorePrivate
)

# Resources:
set(qabstractfileengine_resource_files
    "resources/"
)

bobui_internal_add_resource(tst_qabstractfileengine "qabstractfileengine"
    PREFIX
        "/tst_qabstractfileengine/"
    FILES
        ${qabstractfileengine_resource_files}
)

