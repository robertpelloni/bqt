# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qhttpsocketengine LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if(NOT BOBUI_FEATURE_private_tests)
    return()
endif()

#####################################################################
## tst_qhttpsocketengine Test:
#####################################################################

bobui_internal_add_test(tst_qhttpsocketengine
    SOURCES
        tst_qhttpsocketengine.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::NetworkPrivate
    BOBUI_TEST_SERVER_LIST "squid" "danted" "cyrus" "apache2"
)

## Scopes:
#####################################################################

# BOBUI_TEST_SERVER_LIST = "squid" "danted" "cyrus" "apache2"

bobui_internal_extend_target(tst_qhttpsocketengine CONDITION WIN32
    LIBRARIES
        ws2_32
)
