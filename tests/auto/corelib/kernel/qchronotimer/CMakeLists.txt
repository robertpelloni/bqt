# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qchronotimer LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if (NOT BOBUI_FEATURE_thread)
    return()
endif()

function(addChronoTimerTest test)
    bobui_internal_add_test(${test}
        SOURCES
            tst_qchronotimer.cpp
        LIBRARIES
            BobUI::CorePrivate
            BobUI::TestPrivate
    )
endfunction()

addChronoTimerTest(tst_qchronotimer)

if(BOBUI_FEATURE_glib AND UNIX)
    addChronoTimerTest(tst_qchronotimer_no_glib)
    bobui_internal_extend_target(tst_qchronotimer_no_glib
        DEFINES
            DISABLE_GLIB
            tst_QChronoTimer=tst_QChronoTimer_no_glib # Class name in the unittest
    )
endif()
