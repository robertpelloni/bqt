# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
bobui_internal_add_manual_test(sockify_sockets_auto
    SOURCES
        main.cpp
        ../../bobuiwasmtestlib/bobuiwasmtestlib.cpp
    LIBRARIES
        BobUI::Core
        BobUI::Network
)

include_directories(../../bobuiwasmtestlib/)

add_custom_command(
    TARGET sockify_sockets_auto POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/sockify_sockets_auto.html
            ${CMAKE_CURRENT_BINARY_DIR}/sockify_sockets_auto.html)

add_custom_command(
    TARGET sockify_sockets_auto POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/../../bobuiwasmtestlib/bobuiwasmtestlib.js
            ${CMAKE_CURRENT_BINARY_DIR}/bobuiwasmtestlib.js)
