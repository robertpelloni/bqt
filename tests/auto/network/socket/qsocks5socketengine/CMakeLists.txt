# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qsocks5socketengine LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if(NOT BOBUI_FEATURE_private_tests)
    return()
endif()

#####################################################################
## tst_qsocks5socketengine Test:
#####################################################################

bobui_internal_add_test(tst_qsocks5socketengine
    SOURCES
        tst_qsocks5socketengine.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::NetworkPrivate
    BOBUI_TEST_SERVER_LIST "danted" "apache2" "cyrus"
)

## Scopes:
#####################################################################

# BOBUI_TEST_SERVER_LIST = "danted" "apache2" "cyrus"

bobui_internal_extend_target(tst_qsocks5socketengine CONDITION WIN32
    LIBRARIES
        ws2_32
)
