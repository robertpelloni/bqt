# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qlocalsocket LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

add_subdirectory(socketprocess)

bobui_internal_add_test(tst_qlocalsocket
    SOURCES
        tst_qlocalsocket.cpp
    DEFINES
        QLOCALSERVER_DEBUG
        QLOCALSOCKET_DEBUG
    LIBRARIES
        BobUI::Network
        BobUI::TestPrivate
)
add_dependencies(tst_qlocalsocket socketprocess)
