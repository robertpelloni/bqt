# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

project(test_wrap_cpp_moc_target)

find_package(BobUI6Core REQUIRED)

add_executable(example
    main.cpp
    myobject1.cpp
)
target_link_libraries(example PRIVATE BobUI::Core)

bobui_wrap_cpp(example
    main.cpp
    myobject1.h
)
