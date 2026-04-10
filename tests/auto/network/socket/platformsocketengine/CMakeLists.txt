# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_platformsocketengine LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if(NOT BOBUI_FEATURE_private_tests)
    return()
endif()

#####################################################################
## tst_platformsocketengine Test:
#####################################################################

bobui_internal_add_test(tst_platformsocketengine
    SOURCES
        tst_platformsocketengine.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::NetworkPrivate
    BOBUI_TEST_SERVER_LIST "cyrus"
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_platformsocketengine CONDITION WIN32
    LIBRARIES
        ws2_32
)
