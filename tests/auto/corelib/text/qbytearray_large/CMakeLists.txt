# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT BOBUI_BUILD_STANDALONE_TESTS AND NOT BOBUI_BUILDING_BOBUI)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qbytearray_large LANGUAGES CXX)
    find_package(BobUI6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

bobui_internal_add_test(tst_qbytearray_large
    SOURCES
        tst_qbytearray_large.cpp
    LIBRARIES
        BobUI::Core
    TESTDATA "rfc3252.txt"
)

bobui_internal_is_in_test_batch(in_batch tst_qbytearray_large)
if(in_batch)
    _bobui_internal_test_batch_target_name(test_target)
else()
    set(test_target tst_qbytearray_large)
endif()

bobui_internal_set_exceptions_flags(${test_target} TRUE)
