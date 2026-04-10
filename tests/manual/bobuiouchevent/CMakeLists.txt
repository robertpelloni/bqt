# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## bobuiouchevent Binary:
#####################################################################

bobui_internal_add_manual_test(bobuiouchevent
    SOURCES
        form.ui
        main.cpp
        touchwidget.cpp touchwidget.h
    LIBRARIES
        BobUI::Gui
        BobUI::Test
        BobUI::Widgets
    ENABLE_AUTOGEN_TOOLS
        uic
)
