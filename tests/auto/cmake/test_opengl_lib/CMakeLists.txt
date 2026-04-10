# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


cmake_minimum_required(VERSION 3.16)

project(test_opengl_lib)

find_package(BobUI6Gui REQUIRED)
find_package(BobUI6OpenGL REQUIRED)

get_property(BobUIGui_enabled_features TARGET BobUI6::Gui PROPERTY BOBUI_ENABLED_PUBLIC_FEATURES)
list(LENGTH BobUIGui_enabled_features _BobUIGui_enabled_featuresSize)
if (_BobUIGui_enabled_featuresSize LESS 1)
    message(SEND_ERROR "The BOBUI_ENABLED_PUBLIC_FEATURES property of BobUI6::Gui is empty!")
endif()

add_executable(myobject main.cpp )
target_link_libraries(myobject BobUI::Gui BobUI::OpenGL)
