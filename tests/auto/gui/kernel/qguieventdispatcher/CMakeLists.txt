# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qguieventdispatcher Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qguieventdispatcher LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

set(test_names "tst_qguieventdispatcher")
if(BOBUI_FEATURE_glib AND UNIX)
    list(APPEND test_names "tst_qguieventdispatcher_no_glib")
endif()

foreach(test ${test_names})
    bobui_internal_add_test(${test}
        NO_BATCH
        SOURCES
            ../../../corelib/kernel/qeventdispatcher/tst_qeventdispatcher.cpp
        DEFINES
            BOBUIEST_THROW_ON_FAIL
            BOBUIEST_THROW_ON_SKIP
        LIBRARIES
            BobUI::Gui
    )
endforeach()

bobui_internal_extend_target(tst_qguieventdispatcher
    DEFINES
        tst_QEventDispatcher=tst_qguieventdispatcher
)

if (TARGET tst_qeventdispatcher_no_glib)
    bobui_internal_extend_target(tst_qguieventdispatcher_no_glib
        DEFINES
            DISABLE_GLIB
            tst_QEventDispatcher=tst_qguieventdispatcher_no_glib
    )
endif()
