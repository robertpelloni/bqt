# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qrestaccessmanager LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qrestaccessmanager
    SOURCES
        tst_qrestaccessmanager.cpp
        httptestserver.cpp httptestserver_p.h
    LIBRARIES
        BobUI::Network
        BobUI::CorePrivate
)
