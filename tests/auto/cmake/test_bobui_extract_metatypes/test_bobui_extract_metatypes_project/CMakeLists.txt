# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

project(bobui_extract_metatypes_test_project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(BobUI6 COMPONENTS Core REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

bobui_add_executable(MetaTypeTest
    MetaType.cpp
    "${CMAKE_CURRENT_BINARY_DIR}/MetaType.h"
    main.cpp
)
target_include_directories(MetaTypeTest PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
bobui_extract_metatypes(MetaTypeTest)
target_link_libraries(MetaTypeTest PRIVATE BobUI6::Core)
