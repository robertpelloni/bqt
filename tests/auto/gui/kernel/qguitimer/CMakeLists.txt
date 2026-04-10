# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qguitimer Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qguitimer LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

function(addGuiTimerTest test)
    bobui_internal_add_test(${test}
        SOURCES
            ../../../corelib/kernel/bobuiimer/tst_bobuiimer.cpp
        LIBRARIES
            BobUI::CorePrivate
            BobUI::Gui
            BobUI::TestPrivate
    )
endfunction()

addGuiTimerTest(tst_qguitimer)
bobui_internal_extend_target(tst_qguitimer
    DEFINES
        tst_BOBUIimer=tst_QGuiTimer
)

if(BOBUI_FEATURE_glib AND UNIX)
    addGuiTimerTest(tst_qguitimer_no_glib)
    bobui_internal_extend_target(tst_qguitimer_no_glib
        DEFINES
            DISABLE_GLIB
            tst_BOBUIimer=tst_QGuiTimer_no_glib # Class name in the unittest
    )
endif()
