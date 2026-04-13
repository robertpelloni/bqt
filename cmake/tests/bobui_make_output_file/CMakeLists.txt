# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

project(BobUIMakeOutputFileTest
    VERSION 1.0.0
    DESCRIPTION "bobui_make_output_file test"
    HOMEPAGE_URL "https://bobui.io/"
    LANGUAGES CXX C
)

## Add some paths to check for cmake modules:
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../;${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/extra-cmake-modules/find-modules;${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/kwin")

include(BobUIBuild)

bobui_make_output_file("foo.cpp" "" ".mapped" "/tmp/foo" "/tmp/bar" outfile)
assert(outfile STREQUAL "/tmp/bar/foo.mapped")

bobui_make_output_file("../foo.cpp" "prefix_" ".cpp" "/tmp/foo" "/tmp/bar" outfile)
assert(outfile STREQUAL "/tmp/bar/__/prefix_foo.cpp")

bobui_make_output_file("/tmp/bar/foo.cpp" "prefix_" ".cpp" "/tmp/foo" "/tmp/bar" outfile)
assert(outfile STREQUAL "/tmp/bar/prefix_foo.cpp")
