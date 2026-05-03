# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(bobuiextcursorinsert
    GUI
    SOURCES
        main.cpp
        widget.cpp widget.h widget.ui
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
    ENABLE_AUTOGEN_TOOLS
        uic
)
