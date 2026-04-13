# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_bobuiimer LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if (NOT BOBUI_FEATURE_thread)
    return()
endif()

function(addTimerTest test)
    bobui_internal_add_test(${test}
        SOURCES
            tst_bobuiimer.cpp
        LIBRARIES
            BobUI::CorePrivate
            BobUI::TestPrivate
    )
endfunction()

addTimerTest(tst_bobuiimer)

if(BOBUI_FEATURE_glib AND UNIX)
    addTimerTest(tst_bobuiimer_no_glib)
    bobui_internal_extend_target(tst_bobuiimer_no_glib
        DEFINES
            DISABLE_GLIB
            tst_BOBUIimer=tst_BOBUIimer_no_glib # Class name in the unittest
    )
endif()

