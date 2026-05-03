# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


cmake_minimum_required(VERSION 3.16)

project(test_wrap_cpp_and_resources)

find_package(BobUI6Core REQUIRED)

bobui_wrap_cpp(moc_files myobject.h)

bobui_add_resources(rcc_files "test_wrap_cpp_and_resources.qrc")

add_executable(myobject myobject.cpp ${moc_files} ${rcc_files})
target_link_libraries(myobject PRIVATE BobUI::Core)
