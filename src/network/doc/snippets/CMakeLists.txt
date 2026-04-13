# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

project(network_cppsnippets)

add_executable(mytarget
    network/tcpwait.cpp
)

# ![0]
find_package(BobUI6 REQUIRED COMPONENTS Network)
target_link_libraries(mytarget PRIVATE BobUI6::Network)
# ![0]
