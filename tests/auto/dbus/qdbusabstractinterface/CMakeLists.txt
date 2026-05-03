# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qdbusabstractinterface LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qdbusabstractinterface
    SOURCES
        interface.cpp
        tst_qdbusabstractinterface.cpp
    LIBRARIES
        BobUI::Core
        BobUI::DBus
)

bobui_internal_extend_target(tst_qdbusabstractinterface
    DBUS_INTERFACE_SOURCES
        org.bobuiproject.BobUIDBus.Pinger.xml
    DBUS_INTERFACE_BASENAME
        pinger_interface
    DBUS_INTERFACE_FLAGS
        -i interface.h
)

add_subdirectory(qpinger)
add_dependencies(tst_qdbusabstractinterface qpinger)
