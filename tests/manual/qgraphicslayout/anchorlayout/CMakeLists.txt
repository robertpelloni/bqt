# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## anchorlayout Binary:
#####################################################################
cmake_minimum_required(VERSION 3.16)

bobui_internal_add_manual_test(anchorlayout
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)

