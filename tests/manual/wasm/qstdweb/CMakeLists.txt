# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
bobui_internal_add_manual_test(files_auto
    SOURCES
        files_main.cpp
        ../bobuiwasmtestlib/bobuiwasmtestlib.cpp
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::GuiPrivate
)

include_directories(../bobuiwasmtestlib/)

add_custom_command(
    TARGET files_auto POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/files_auto.html
            ${CMAKE_CURRENT_BINARY_DIR}/files_auto.html)

add_custom_command(
    TARGET files_auto POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/../bobuiwasmtestlib/bobuiwasmtestlib.js
            ${CMAKE_CURRENT_BINARY_DIR}/bobuiwasmtestlib.js)

bobui_internal_add_manual_test(qwasmcompositor_auto
    SOURCES
        qwasmcompositor_main.cpp
        ../bobuiwasmtestlib/bobuiwasmtestlib.cpp
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::GuiPrivate
)

include_directories(../bobuiwasmtestlib/)

add_custom_command(
    TARGET qwasmcompositor_auto POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/qwasmcompositor_auto.html
            ${CMAKE_CURRENT_BINARY_DIR}/qwasmcompositor_auto.html)

add_custom_command(
    TARGET qwasmcompositor_auto POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/../bobuiwasmtestlib/bobuiwasmtestlib.js
            ${CMAKE_CURRENT_BINARY_DIR}/bobuiwasmtestlib.js)

bobui_internal_add_manual_test(iodevices_auto
    SOURCES
        iodevices_main.cpp
        ../bobuiwasmtestlib/bobuiwasmtestlib.cpp
    LIBRARIES
        BobUI::Core
        BobUI::CorePrivate
        BobUI::GuiPrivate
)

add_custom_command(
    TARGET iodevices_auto POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/iodevices_auto.html
            ${CMAKE_CURRENT_BINARY_DIR}/iodevices_auto.html)

add_custom_command(
    TARGET iodevices_auto POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/../bobuiwasmtestlib/bobuiwasmtestlib.js
            ${CMAKE_CURRENT_BINARY_DIR}/bobuiwasmtestlib.js)
