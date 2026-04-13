# Copyright (C) 2026 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22)

project(test_wrap_cpp_target_in_subdir)

find_package(BobUI6Core REQUIRED)

add_executable(example main.cpp)
target_link_libraries(example PRIVATE BobUI::Core)
add_subdirectory(subdir)
