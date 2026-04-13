# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## bobui-client-widget Binary:
#####################################################################

bobui_internal_add_manual_test(bobui-client-widget
    GUI
    SOURCES
        main.cpp
        window.cpp window.h
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
