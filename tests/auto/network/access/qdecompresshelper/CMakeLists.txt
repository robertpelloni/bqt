# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qdecompresshelper Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qdecompresshelper LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qdecompresshelper
    SOURCES
        gzip.rcc.cpp
        inflate.rcc.cpp
        tst_qdecompresshelper.cpp
        zstandard.rcc.cpp
    DEFINES
        SRC_DIR=${CMAKE_CURRENT_SOURCE_DIR}
    LIBRARIES
        BobUI::NetworkPrivate
)
