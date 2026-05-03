# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## qheaderview Binary:
#####################################################################

bobui_internal_add_manual_test(qheaderview
    GUI
    SOURCES
        qheaderviewtest1.cpp
    LIBRARIES
        BobUI::Gui
        BobUI::Widgets
)
