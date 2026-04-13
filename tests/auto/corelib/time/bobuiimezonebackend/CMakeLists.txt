# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_bobuiimezonebackend Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_bobuiimezonebackend LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_bobuiimezonebackend
    SOURCES
        tst_bobuiimezonebackend.cpp
    DEFINES
        BOBUI_NO_CAST_FROM_ASCII
        BOBUI_NO_CAST_TO_ASCII
        BOBUI_NO_FOREACH
        BOBUI_NO_KEYWORDS
    LIBRARIES
        BobUI::CorePrivate
)

## Scopes:
#####################################################################

bobui_internal_extend_target(tst_bobuiimezonebackend CONDITION BOBUI_FEATURE_icu
    LIBRARIES
        ICU::i18n ICU::uc ICU::data
)
