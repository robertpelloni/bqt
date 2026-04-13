# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## bobui_on_cocoa Binary:
#####################################################################

bobui_internal_add_manual_test(bobui_on_cocoa
    GUI
    SOURCES
        main.mm
        rasterwindow.cpp rasterwindow.h
    LIBRARIES
        ${FWAppKit}
        BobUI::Gui
        BobUI::Quick
        BobUI::Widgets
)
