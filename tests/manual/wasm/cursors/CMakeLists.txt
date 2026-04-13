# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(cursors
    GUI
    SOURCES
        MainWindow.cpp MainWindow.h MainWindow.ui
        main.cpp
    LIBRARIES
        BobUI::Core
        BobUI::Gui
        BobUI::Widgets
    ENABLE_AUTOGEN_TOOLS
        uic
)
