# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
project(echo_server)
cmake_minimum_required(VERSION 3.19)

find_package(BobUI6 COMPONENTS Core)
find_package(BobUI6 COMPONENTS network)

bobui_add_executable(echo_server
    main.cpp
)

target_link_libraries(echo_server PUBLIC
    BobUI::Core
    BobUI::Network
)
