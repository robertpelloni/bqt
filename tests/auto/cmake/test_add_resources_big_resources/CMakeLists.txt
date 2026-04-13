# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

project(test_add_resources_big_resources)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/FindPackageHints.cmake")
    include("${CMAKE_CURRENT_LIST_DIR}/FindPackageHints.cmake")
endif()

find_package(BobUI6 REQUIRED
    COMPONENTS Core Test
    HINTS ${BobUI6Tests_PREFIX_PATH}
)

bobui6_add_library(leaf_lib STATIC leaf_lib.cpp)
bobui6_add_resources(leaf_lib resources2
    FILES resource2.txt PREFIX "/" BIG_RESOURCES)
target_link_libraries(leaf_lib PRIVATE BobUI6::Core)

if(NOT TARGET leaf_lib_resources2_obj)
    message(FATAL_ERROR "Object library for resources2 was not created.")
endif()

bobui6_add_library(intermediate_lib STATIC intermediate_lib.cpp)
bobui6_add_resources(intermediate_lib resources3
    FILES resource3.txt PREFIX "/" BIG_RESOURCES
)
target_link_libraries(intermediate_lib PRIVATE BobUI6::Core leaf_lib)

if(NOT TARGET intermediate_lib_resources3_obj)
    message(FATAL_ERROR "Object library for resources3 was not created.")
endif()

set(CMAKE_AUTOMOC ON)

bobui6_add_executable(test_add_resources_big_resources main.cpp)
bobui6_add_resources(test_add_resources_big_resources resources1
    FILES resource1.txt PREFIX "/" BIG_RESOURCES)
target_link_libraries(test_add_resources_big_resources PRIVATE BobUI6::Test intermediate_lib)

if(NOT TARGET test_add_resources_big_resources_resources1_obj)
    message(FATAL_ERROR "Object library for resources1 was not created.")
endif()
