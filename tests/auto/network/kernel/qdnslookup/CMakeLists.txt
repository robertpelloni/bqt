# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qdnslookup Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qdnslookup LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qdnslookup
    SOURCES
        tst_qdnslookup.cpp
    LIBRARIES
        BobUI::Network
        BobUI::TestPrivate
)

bobui_internal_extend_target(tst_qdnslookup CONDITION WIN32
    LIBRARIES
        iphlpapi
)
