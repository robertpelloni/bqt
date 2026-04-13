# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qdbusconnection_no_app Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qdbusconnection_no_app LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qdbusconnection_no_app
    SOURCES
        ../qdbusconnection/tst_qdbusconnection.h
        tst_qdbusconnection_no_app.cpp
    DEFINES
        tst_QDBusConnection=tst_QDBusConnection_NoApplication
    LIBRARIES
        BobUI::DBus
)
