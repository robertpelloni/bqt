# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## bobuiablewidget-sort-indicator Binary:
#####################################################################

bobui_internal_add_manual_test(bobuiablewidget-sort-indicator
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::Widgets
)
