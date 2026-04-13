# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qdatetime Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qdatetime LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qdatetime
    SOURCES
        tst_qdatetime.cpp
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_TO_ASCII
        BOBUI_NO_FOREACH
        BOBUI_NO_KEYWORDS
    LIBRARIES
        BobUI::CorePrivate
        BobUI::TestPrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qdatetime CONDITION APPLE
    SOURCES
        tst_qdatetime_mac.mm
    LIBRARIES
        ${FWFoundation}
)
