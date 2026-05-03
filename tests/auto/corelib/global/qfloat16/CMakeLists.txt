# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qfloat16 Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qfloat16 LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qfloat16
    SOURCES
        tst_qfloat16.cpp
    LIBRARIES
        BobUI::TestPrivate
)

# To test std::format support, if possible
if(BOBUI_FEATURE_cxx20_format)
    set_target_properties(tst_qfloat16
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED OFF
    )
endif()
