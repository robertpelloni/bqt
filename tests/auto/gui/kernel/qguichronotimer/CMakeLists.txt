# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qguichronotimer Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qguichronotimer LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

function(addGuiChronoTimerTest test)
    bobui_internal_add_test(${test}
        SOURCES
            ../../../corelib/kernel/qchronotimer/tst_qchronotimer.cpp
        LIBRARIES
            BobUI::CorePrivate
            BobUI::Gui
            BobUI::TestPrivate
    )
endfunction()

addGuiChronoTimerTest(tst_qguichronotimer)
bobui_internal_extend_target(tst_qguichronotimer
    DEFINES
        tst_QChronoTimer=tst_QGuiChronoTimer
)

if(BOBUI_FEATURE_glib AND UNIX)
    addGuiChronoTimerTest(tst_qguichronotimer_no_glib)
    bobui_internal_extend_target(tst_qguichronotimer_no_glib
        DEFINES
            DISABLE_GLIB
            tst_QChronoTimer=tst_QGuiChronoTimer_no_glib # Class name in the unittest
    )
endif()
