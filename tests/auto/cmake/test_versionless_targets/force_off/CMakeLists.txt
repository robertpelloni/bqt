# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

message("Test disabled versionless targets")

set(BOBUI_NO_CREATE_VERSIONLESS_TARGETS ON)

find_package(BobUI6Core REQUIRED)

if(NOT TARGET BobUI6::Core)
    message(SEND_ERROR "BobUI6::Core target not defined!")
endif()

if(TARGET BobUI::Core)
    message(SEND_ERROR "BobUI::Core target defined despite BOBUI_NO_CREATE_VERSIONLESS_TARGETS!")
endif()
