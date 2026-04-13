# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# The test is simply testing that manual specification of BOBUI_TESTCASE_BUILDDIR works,
# despite the weird paths.
cmake_minimum_required(VERSION 3.16)

project(test_bobui_testcase_builddir)

find_package(BobUI6Test REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(test_bobui_testcase_builddir WIN32 main.cpp)

target_link_libraries(test_bobui_testcase_builddir BobUI::Test)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/testdata.txt"
    "${CMAKE_CURRENT_BINARY_DIR}/level1/level2/testdata_build.txt"
    COPYONLY
)
set_target_properties(test_bobui_testcase_builddir PROPERTIES
    BOBUI_TESTCASE_BUILDDIR "${CMAKE_CURRENT_BINARY_DIR}/level1/level2"
)
