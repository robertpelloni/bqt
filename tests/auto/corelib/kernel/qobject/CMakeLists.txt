# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qobject Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qobject LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qobject
    SOURCES
        tst_qobject.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Network
        BobUI::TestPrivate
)

## Scopes:
#####################################################################
add_subdirectory(signalbug)
add_dependencies(tst_qobject signalbug_helper)
