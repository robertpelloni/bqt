# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qchar Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qchar LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

# Collect test data
list(APPEND test_data "data/NormalizationTest.txt")

foreach(test tst_qchar tst_qchar_restricted_ascii tst_qchar_no_cast_from_ascii)
    bobui_internal_add_test(${test}
        NO_BATCH
        SOURCES
           tst_qchar.cpp
        LIBRARIES
            BobUI::CorePrivate
        TESTDATA
            ${test_data}
    )
endforeach()

bobui_internal_extend_target(tst_qchar_restricted_ascii
    DEFINES
        BOBUI_RESTRICTED_CAST_FROM_ASCII
        tst_QChar=tst_QChar_restricted_ascii
)

bobui_internal_extend_target(tst_qchar_no_cast_from_ascii
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        tst_QChar=tst_QChar_no_cast_from_ascii
)
