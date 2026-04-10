# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## bobuiabbar Binary:
#####################################################################

bobui_internal_add_manual_test(bobuiabbar
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
