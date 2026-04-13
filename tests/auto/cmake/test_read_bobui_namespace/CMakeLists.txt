# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(bobui_namespace_property LANGUAGES CXX)

find_package(BobUI6 REQUIRED COMPONENTS Core)

get_property(namespace_property
    TARGET BobUI::Core
    PROPERTY BOBUI_NAMESPACE
)

add_executable(test main.cpp)
target_link_libraries(test PRIVATE BobUI::Core)

if(namespace_property)
    target_compile_definitions(test PRIVATE BOBUI_NAMESPACE_FROM_PROPERTY=${namespace_property})
else()
    target_compile_definitions(test PRIVATE BOBUI_NO_NAMESPACE)
endif()
