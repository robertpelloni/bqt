# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

add_library(network_cppsnippets OBJECT tcpwait.cpp)

target_link_libraries(network_cppsnippets PRIVATE
    BobUI::Network
)
