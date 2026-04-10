# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## xcb_gl_integration Binary:
#####################################################################

bobui_internal_add_manual_test(xcb_gl_integration
    GUI
    SOURCES
        main.cpp
    INCLUDE_DIRECTORIES
        .
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)
