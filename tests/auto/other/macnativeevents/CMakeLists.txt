# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_macnativeevents LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if(NOT APPLE)
    return()
endif()

#####################################################################
## tst_macnativeevents Test:
#####################################################################

bobui_internal_add_test(tst_macnativeevents
    SOURCES
        expectedeventlist.cpp expectedeventlist.h
        nativeeventlist.cpp nativeeventlist.h
        qnativeevents.cpp qnativeevents.h
        qnativeevents_mac.cpp
        tst_macnativeevents.cpp
    LIBRARIES
        ${FWAppKit}
        BobUI::Gui
        BobUI::Widgets
)
