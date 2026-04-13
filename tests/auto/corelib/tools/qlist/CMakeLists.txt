# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qlist Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qlist LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qlist
    SOURCES
        tst_qlist.cpp
    DEFINES
        BOBUI_NO_STRICT_QLIST_ITERATORS # doesn't exist, except for this test
    LIBRARIES
        BobUI::CorePrivate
        BobUI::TestPrivate
)

bobui_internal_add_test(tst_qlist_strict_iterators
    SOURCES
        tst_qlist.cpp
    DEFINES
        BOBUI_STRICT_QLIST_ITERATORS
        tst_QList=tst_QList_strict_iterators
    LIBRARIES
        BobUI::CorePrivate
        BobUI::TestPrivate
)

## Scopes:
#####################################################################
