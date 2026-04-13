# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(fontloading
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Core
        BobUI::Gui
        BobUI::Widgets
)

add_custom_command(
    TARGET fontloading POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/fontloading.html
            ${CMAKE_CURRENT_BINARY_DIR}/fontloading.html)
#add_custom_target(html DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/fontloading.html)
#add_dependencies(fontloading html)
