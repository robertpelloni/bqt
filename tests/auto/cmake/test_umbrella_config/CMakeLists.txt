# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause


cmake_minimum_required(VERSION 3.16)

project(test_umbrella_config)

add_subdirectory(components_found)

if (BobUI6_FOUND)
    message(SEND_ERROR "BobUI6_FOUND variable leaked!")
endif()

if (BobUI6Core_FOUND)
    message(SEND_ERROR "BobUI6Core_FOUND variable leaked!")
endif()

if (TARGET BobUI6::Core)
    message(SEND_ERROR "BobUI6::Core target leaked!")
endif()

add_subdirectory(components_not_found)
