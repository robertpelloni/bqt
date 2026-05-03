# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_localfileapi Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_localfileapi LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_localfileapi
    SOURCES
        tst_localfileapi.cpp
    DEFINES
        BOBUI_NO_FOREACH
    LIBRARIES
        BobUI::GuiPrivate
        BobUI::Core
        BobUI::Gui
)
