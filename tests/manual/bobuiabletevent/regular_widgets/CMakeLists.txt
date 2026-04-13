# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## regular_widgets Binary:
#####################################################################

bobui_internal_add_manual_test(regular_widgets
    GUI
    SOURCES
        main.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
        BobUI::Widgets
)
