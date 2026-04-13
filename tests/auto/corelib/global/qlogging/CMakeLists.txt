# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qlogging LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_executable(qlogging_helper
    NO_INSTALL
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES qlogging_helper.cpp ../../../../shared/disablecoredumps.cpp
    DEFINES BOBUI_MESSAGELOGCONTEXT
    LIBRARIES BobUI::Core)

bobui_internal_add_executable(qlogging_race_helper
    NO_INSTALL
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES qlogging_race_helper.cpp ../../../../shared/disablecoredumps.cpp
    LIBRARIES BobUI::Core
)

# Fixes required for the backtrace stack to be correct
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" AND NOT MINGW)
    target_link_options(qlogging_helper PRIVATE -rdynamic)
endif()
set_target_properties(qlogging_helper PROPERTIES CXX_VISIBILITY_PRESET default)

bobui_internal_add_test(tst_qlogging SOURCES tst_qlogging.cpp
    DEFINES
        BOBUI_MESSAGELOGCONTEXT
)

add_dependencies(tst_qlogging qlogging_helper qlogging_race_helper)

bobui_internal_add_test(tst_qmessagelogger SOURCES tst_qmessagelogger.cpp
    DEFINES
        BOBUI_MESSAGELOGCONTEXT
)
