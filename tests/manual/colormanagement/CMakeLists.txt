# Copyright (C) 2024 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

bobui_internal_add_manual_test(colormanagement
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
    GUI
)
