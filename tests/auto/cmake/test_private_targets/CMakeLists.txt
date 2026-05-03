# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


cmake_minimum_required(VERSION 3.16)

project(test_private_targets)

find_package(BobUI6GuiPrivate REQUIRED)

add_executable(testapp main.cpp)
target_link_libraries(testapp BobUI::GuiPrivate)
