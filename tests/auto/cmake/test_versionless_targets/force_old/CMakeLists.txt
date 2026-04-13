# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

message("Test force old versionless targets")

set(BOBUI_USE_OLD_VERSION_LESS_TARGETS ON)

find_package(BobUI6Core REQUIRED)

if(NOT TARGET BobUI6::Core)
    message(SEND_ERROR "BobUI6::Core target not defined!")
endif()

if(NOT TARGET BobUI::Core)
    message(SEND_ERROR "BobUI::Core target not defined!")
endif()

check_versionless_targets()

