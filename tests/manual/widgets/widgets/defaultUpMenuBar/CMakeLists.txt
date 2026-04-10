# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## defaultUpMenuBar Binary:
#####################################################################

bobui_internal_add_manual_test(defaultUpMenuBar
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
)
