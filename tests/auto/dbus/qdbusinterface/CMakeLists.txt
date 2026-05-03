# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qdbusinterface LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

add_subdirectory(qmyserver)

#####################################################################
## ../tst_qdbusinterface Test:
#####################################################################

bobui_internal_add_test(tst_qdbusinterface
    SOURCES
        myobject.h
        tst_qdbusinterface.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::DBusPrivate
    LIBRARIES
        BobUI::Core
        BobUI::DBus
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qdbusinterface CONDITION BOBUI_FEATURE_dbus_linked
    DEFINES
        BOBUI_LINKED_LIBDBUS
    LIBRARIES
        dbus-1
)

bobui_internal_extend_target(tst_qdbusinterface CONDITION NOT BOBUI_FEATURE_dbus_linked
    SOURCES
        ../../../../src/dbus/qdbus_symbols.cpp
)

add_dependencies(tst_qdbusinterface qmyserver_qdbusinterface)

