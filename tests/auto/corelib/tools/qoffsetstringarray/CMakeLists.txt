# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qoffsetstringarray Test:
#####################################################################

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qoffsetstringarray LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qoffsetstringarray
    SOURCES
        tst_qoffsetstringarray.cpp
    LIBRARIES
        BobUI::CorePrivate
)
if (NOT VXWORKS)
    set_property(TARGET tst_qoffsetstringarray PROPERTY CXX_STANDARD 20)
endif()

if (CLANG)
    target_compile_options(tst_qoffsetstringarray
        PUBLIC -fbracket-depth=512)
elseif (GCC)
    # fconstexpr-depth= defaults to 512
endif()
