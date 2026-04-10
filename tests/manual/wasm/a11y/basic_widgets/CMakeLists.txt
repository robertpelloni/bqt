# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause
bobui_internal_add_manual_test(a11y_basic_widgets
    GUI
    SOURCES
        tabswidget.cpp
        tabswidget.h
        basica11ywidget.h
        basica11ywidget.cpp
        main.cpp
    LIBRARIES
        BobUI::Core
        BobUI::Gui
        BobUI::Widgets
)

add_custom_command(
    TARGET a11y_basic_widgets PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/basic_widgets.html
            ${CMAKE_CURRENT_BINARY_DIR}/basic_widgets.html
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/basic_widgets.html
)
