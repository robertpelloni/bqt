# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## bobuireewidget Binary:
#####################################################################

bobui_internal_add_manual_test(bobuireewidget
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::Widgets
)
