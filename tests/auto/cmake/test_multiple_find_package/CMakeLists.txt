# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


cmake_minimum_required(VERSION 3.16)

project(test_multiple_find_package)

find_package(BobUI6Core REQUIRED)

add_subdirectory(subdir1)

add_executable(exe1 main.cpp)
target_link_libraries(exe1 BobUI::Core)
