# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qscreen2 Test:
#####################################################################

bobui_internal_add_test(tst_qscreen_xrandr
    SOURCES
        tst_qscreen_xrandr.cpp
    LIBRARIES
        BobUI::CorePrivate
        BobUI::Gui
        BobUI::GuiPrivate
)
