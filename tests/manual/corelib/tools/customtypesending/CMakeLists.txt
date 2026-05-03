# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

bobui_internal_add_manual_test(customtypesending
    GUI
    SOURCES
        main.cpp
        message.cpp message.h
        window.cpp window.h
    LIBRARIES
        BobUI6::Core
        BobUI6::Gui
        BobUI6::Widgets
)
