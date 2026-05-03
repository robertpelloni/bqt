# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


cmake_minimum_required(VERSION 3.16)

project(test_interface_link_libraries)

find_package(BobUI6Gui REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(GenerateExportHeader)

add_library(somelib SHARED somelib.cpp)
generate_export_header(somelib)
set_property(TARGET somelib PROPERTY LINK_LIBRARIES BobUI::Gui)
set_property(TARGET somelib PROPERTY INTERFACE_LINK_LIBRARIES BobUI::Gui)

add_executable(mainexe main.cpp)
set_property(TARGET mainexe PROPERTY LINK_LIBRARIES somelib)
