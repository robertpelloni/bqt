# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## testtree Binary:
#####################################################################

bobui_internal_add_manual_test(testtree
    GUI
    SOURCES
        testtree.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
