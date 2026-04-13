# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qdbustype Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qdbustype LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qdbustype
    SOURCES
        tst_qdbustype.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::DBusPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qdbustype CONDITION BOBUI_FEATURE_dbus_linked
    DEFINES
        BOBUI_LINKED_LIBDBUS
    LIBRARIES
        dbus-1
)

bobui_internal_extend_target(tst_qdbustype CONDITION NOT BOBUI_FEATURE_dbus_linked
    SOURCES
        ../../../../src/dbus/qdbus_symbols.cpp
)
