# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

project(test_BOBUIP0003)

find_package(BobUI6 COMPONENTS Core REQUIRED)

if(BOBUI6_IS_SHARED_LIBS_BUILD)
    set(bobui_build_type "SHARED_LIBRARY")
else()
    set(bobui_build_type "STATIC_LIBRARY")
endif()

set(BUILD_SHARED_LIBS ON)
bobui6_add_library(MyLib source.cpp)
get_target_property(type MyLib TYPE)
if(NOT "${type}" STREQUAL "${bobui_build_type}")
    message(FATAL_ERROR "The library uses the default type different from BobUI build type when"
        "BOBUIP0003 is not set")
endif()

set(BUILD_SHARED_LIBS OFF)
bobui6_add_library(MyLib2 source.cpp)
get_target_property(type MyLib2 TYPE)
if(NOT "${type}" STREQUAL "${bobui_build_type}")
    message(FATAL_ERROR "The library uses the default type different from BobUI build type when"
        "BOBUIP0003 is not set")
endif()

set(BUILD_SHARED_LIBS ON)
bobui_policy(SET BOBUIP0003 OLD)
bobui6_add_library(MyLib3 source.cpp)
get_target_property(type MyLib3 TYPE)
if(NOT "${type}" STREQUAL "${bobui_build_type}")
    message(FATAL_ERROR "The library uses the default type different from BobUI build type when"
        "BOBUIP0003 is set to OLD")
endif()

set(BUILD_SHARED_LIBS OFF)
bobui_policy(SET BOBUIP0003 OLD)
bobui6_add_library(MyLib4 source.cpp)
get_target_property(type MyLib4 TYPE)
if(NOT "${type}" STREQUAL "${bobui_build_type}")
    message(FATAL_ERROR "The library uses the default type different from BobUI build type when"
        "BOBUIP0003 is set to OLD")
endif()

set(BUILD_SHARED_LIBS ON)
bobui_policy(SET BOBUIP0003 NEW)
bobui6_add_library(MyLib5 source.cpp)
get_target_property(type MyLib5 TYPE)
if(NOT "${type}" STREQUAL "SHARED_LIBRARY")
    message(FATAL_ERROR "The library doesn't consider the BUILD_SHARED_LIBS when"
        "BOBUIP0003 is set to NEW")
endif()

set(BUILD_SHARED_LIBS OFF)
bobui_policy(SET BOBUIP0003 NEW)
bobui6_add_library(MyLib6 source.cpp)
get_target_property(type MyLib6 TYPE)
if(NOT "${type}" STREQUAL "STATIC_LIBRARY")
    message(FATAL_ERROR "The library doesn't consider the BUILD_SHARED_LIBS when"
        "BOBUIP0003 is set to NEW")
endif()
