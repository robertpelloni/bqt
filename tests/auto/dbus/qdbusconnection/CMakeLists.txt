# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qdbusconnection Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qdbusconnection LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qdbusconnection
    SOURCES
        tst_qdbusconnection.cpp tst_qdbusconnection.h
    LIBRARIES
        BobUI::DBus
    TESTDATA
        tst_qdbusconnection.conf
)

bobui_internal_add_executable(qdbusdelayeddeliveryreenablehelper
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    SOURCES
        qdbusdelayeddeliveryreenablehelper.cpp
    LIBRARIES
        BobUI::DBus
)

add_dependencies(tst_qdbusconnection
    qdbusdelayeddeliveryreenablehelper
)
