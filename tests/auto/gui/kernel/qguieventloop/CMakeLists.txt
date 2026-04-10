# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qguieventloop Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qguieventloop LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qguieventloop
    SOURCES
        ../../../corelib/kernel/qeventloop/tst_qeventloop.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::Network
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_qguieventloop CONDITION WIN32
    LIBRARIES
        user32
)

bobui_internal_extend_target(tst_qguieventloop CONDITION BOBUI_FEATURE_glib
    DEFINES
        HAVE_GLIB
)
