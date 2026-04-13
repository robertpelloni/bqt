# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_nobobuieventloop Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_nobobuieventloop LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_nobobuieventloop
    SOURCES
        tst_nobobuieventloop.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Network
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_nobobuieventloop CONDITION BOBUI_FEATURE_dynamicgl AND WIN32
    LIBRARIES
        user32
)
