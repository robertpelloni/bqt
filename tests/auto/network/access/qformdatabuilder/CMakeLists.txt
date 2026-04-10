# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qformdatabuilder LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qformdatabuilder
    NO_BATCH # BOBUIBUG-121815
    DEFINES
        BOBUIEST_THROW_ON_FAIL
        BOBUIEST_THROW_ON_SKIP
    SOURCES
        tst_qformdatabuilder.cpp
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::Network
        BobUI::NetworkPrivate
    TESTDATA
        rfc3252.txt
        image1.jpg
        document.docx
        sheet.xlsx
)

if(BOBUI_FEATURE_sanitize_undefined)
    bobui_internal_extend_target(tst_qformdatabuilder
        DEFINES
            BOBUI_SANITIZE_UNDEFINED # GCC (in)famously doesn't provide a predefined macro for this
    )
endif()
