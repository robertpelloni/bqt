# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qstring LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

if(NOT BOBUI_FEATURE_doubleconversion AND NOT BOBUI_FEATURE_system_doubleconversion)
    list(APPEND tst_qstring_extra_defines BOBUI_NO_DOUBLECONVERSION)
endif()

if(APPLE)
    list(APPEND tst_qstring_extra_libraries ${FWFoundation})
    list(APPEND tst_qstring_extra_sources tst_qstring_mac.mm)
endif()
if(WASM)
    list(APPEND tst_qstring_extra_sources tst_qstring_wasm.cpp)
endif()

foreach(test tst_qstring tst_qstring_restricted_ascii tst_qstring_no_cast_from_ascii)
    bobui_internal_add_test(${test}
        NO_BATCH
        SOURCES
            tst_qstring.cpp
            tst_qstringliteral_compile.cpp
            ${tst_qstring_extra_sources}
        LIBRARIES
            BobUI::CorePrivate
            BobUI::TestPrivate
            ${tst_qstring_extra_libraries}
        DEFINES
            ${tst_qstring_extra_defines}
    )
endforeach()

bobui_internal_extend_target(tst_qstring_restricted_ascii
    DEFINES
        BOBUI_RESTRICTED_CAST_FROM_ASCII
        tst_QString=tst_QString_restricted_ascii
)

bobui_internal_extend_target(tst_qstring_no_cast_from_ascii
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        tst_QString=tst_QString_no_cast_from_ascii
)
