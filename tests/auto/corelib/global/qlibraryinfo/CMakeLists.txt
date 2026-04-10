# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qlibraryinfo LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()


bobui_internal_add_test(tst_qlibraryinfo SOURCES tst_qlibraryinfo.cpp
    LIBRARIES
        BobUI::CorePrivate

)

bobui_add_resources(tst_qlibraryinfo "bobuiconffiles"
    PREFIX "/"
    FILES
        empty.bobui.conf
        partial.bobui.conf
        list.bobui.conf
        merge.bobui.conf
)
