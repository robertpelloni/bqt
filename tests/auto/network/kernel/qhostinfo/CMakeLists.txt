# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qhostinfo LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if(NOT BOBUI_FEATURE_private_tests)
    return()
endif()

#####################################################################
## tst_qhostinfo Test:
#####################################################################

bobui_internal_add_test(tst_qhostinfo
    SOURCES
        tst_qhostinfo.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::NetworkPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qhostinfo CONDITION WIN32
    LIBRARIES
        ws2_32
)
