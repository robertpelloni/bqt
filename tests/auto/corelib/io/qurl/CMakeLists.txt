# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qurl Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qurl LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qurl
    SOURCES
        tst_qurl.cpp
    LIBRARIES
        BobUI::Concurrent
        BobUI::TestPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qurl CONDITION APPLE
    SOURCES
        tst_qurl_mac.mm
    LIBRARIES
        BobUI::CorePrivate
)
