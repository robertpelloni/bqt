# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_bench_qpainterpath LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_benchmark(tst_bench_qpainterpath
    SOURCES
        tst_bench_qpainterpath.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Test
)
