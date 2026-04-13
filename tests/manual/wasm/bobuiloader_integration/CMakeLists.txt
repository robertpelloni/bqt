# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(tst_bobuiloader_integration
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Core
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
)

set_target_properties(tst_bobuiloader_integration PROPERTIES BOBUI_WASM_EXTRA_EXPORTED_METHODS "ENV")

add_custom_command(
    TARGET tst_bobuiloader_integration POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/tst_bobuiloader_integration.html
            ${CMAKE_CURRENT_BINARY_DIR}/tst_bobuiloader_integration.html)

add_custom_command(
    TARGET tst_bobuiloader_integration POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/plugins/platforms/wasm/bobuiloader.js
            ${CMAKE_CURRENT_BINARY_DIR}/bobuiloader.js)

add_custom_command(
    TARGET tst_bobuiloader_integration POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/../shared/testrunner.js
            ${CMAKE_CURRENT_BINARY_DIR}/testrunner.js)

add_custom_command(
    TARGET tst_bobuiloader_integration POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/test_body.js
            ${CMAKE_CURRENT_BINARY_DIR}/test_body.js)

add_custom_command(
    TARGET tst_bobuiloader_integration POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/preload.json
            ${CMAKE_CURRENT_BINARY_DIR}/preload.json)
