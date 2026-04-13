# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from wl_connect.pro.

#####################################################################
## tst_wlconnect Test:
#####################################################################

bobui_internal_add_test(tst_wlconnect
    SOURCES
        tst_wlconnect.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::GuiPrivate
)
